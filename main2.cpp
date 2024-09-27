#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <unordered_map>
#include <cstdlib>
#include <string>
using namespace std;

class Baraja
{
private:
    vector<int> baraja;

    // Función que devuelve el nombre y el color de la carta usando códigos ANSI
    string obtenerColor(int numero)
    {
        switch (numero)
        {
        case 0:
            return "\033[38;5;214mNaranjado\033[0m"; // Naranjado (Amarillo)
        case 1:
            return "\033[34mAzul\033[0m"; // Azul
        case 2:
            return "\033[33mCafe\033[0m"; // Café (Amarillo)
        case 3:
            return "\033[35mMorado\033[0m"; // Morado
        case 4:
            return "\033[32mVerde\033[0m"; // Verde
        case 5:
            return "\033[31mRojo\033[0m"; // Rojo
        case 6:
            return "\033[33mAmarillo\033[0m"; // Amarillo
        case 7:
            return "\033[36mSumar 2\033[0m"; // Cyan (Sumar 2)
        case 8:
            return "\033[36mComodín\033[0m"; // Cyan (Comodín)
        case 9:
            return "\033[33mComodín dorado\033[0m"; // Dorado (Amarillo)
        default:
            return "\033[31mError carta inexistente\033[0m"; // Rojo (Error)
        }
    }

public:
    Baraja()
    {
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                baraja.push_back(i);
            }
        }
        baraja.push_back(7);
        baraja.push_back(8);
        baraja.push_back(8);
        baraja.push_back(9);
        srand(static_cast<unsigned int>(time(0)));
        random_shuffle(baraja.begin(), baraja.end());
    }

    int getSize() const
    {
        return baraja.size();
    }

    string tomarCarta()
    {
        int cart = baraja.back();
        baraja.pop_back();
        return obtenerColor(cart);
    }
};

class Jugador
{
public:
    string nombre;
    vector<string> cartasRecolectadas;

    Jugador(string nombre) : nombre(nombre) {}

    void agregarCarta(const string &carta)
    {
        cartasRecolectadas.push_back(carta);
    }

    int calcularPuntaje() const
    {
        unordered_map<string, int> conteoColores;
        int puntaje = 0;
        int cartasNegativas = 0;

        for (const string &carta : cartasRecolectadas)
        {
            if (carta != "Sumar 2" && carta != "Comodín" && carta != "Comodín dorado")
            {
                conteoColores[carta]++;
            }
        }

        for (const auto &entry : conteoColores)
        {
            int cantidad = entry.second;

            if (cantidad == 1) puntaje += 1;
            else if (cantidad == 2) puntaje += 3;
            else if (cantidad == 3) puntaje += 6;
            else if (cantidad == 4) puntaje += 10;
            else if (cantidad == 5) puntaje += 15;
            else if (cantidad >= 6) puntaje += 21;

            if (conteoColores.size() > 3)
            {
                cartasNegativas += cantidad;
            }
        }

        if (cartasNegativas > 3)
        {
            puntaje -= (cartasNegativas - 3);
        }

        return puntaje;
    }

    void mostrarCartas() const
    {
        cout << "Cartas recolectadas por " << nombre << ": ";
        for (const string &carta : cartasRecolectadas)
        {
            cout << carta << " ";
        }
        cout << endl;
    }

    void colocarComodines()
    {
        unordered_map<string, int> conteoColores;
        for (const string &carta : cartasRecolectadas)
        {
            if (carta != "Sumar 2" && carta != "Comodín" && carta != "Comodín dorado")
            {
                conteoColores[carta]++;
            }
        }

        string colorConMasCartas;
        int maxCantidad = 0;
        for (const auto &entry : conteoColores)
        {
            if (entry.second > maxCantidad)
            {
                maxCantidad = entry.second;
                colorConMasCartas = entry.first;
            }
        }

        if (!colorConMasCartas.empty())
        {
            cout << "Colocando Comodín en " << colorConMasCartas << endl;
            cartasRecolectadas.push_back("Comodín");

            cout << "Colocando Comodín dorado en " << colorConMasCartas << endl;
            cartasRecolectadas.push_back("Comodín dorado");
        }
        else
        {
            cout << "No hay cartas para colocar comodines." << endl;
        }
    }
};

class Filas
{
public:
    vector<vector<string>> filas;
    vector<bool> filaTomada;

    Filas(int numFilas) : filas(numFilas), filaTomada(numFilas, false) {}

    void agregarCartaAFila(int fila, const string &carta)
    {
        if (filaTomada[fila - 1])
        {
            cout << "No se pueden agregar cartas a una fila que ya fue tomada." << endl;
            return;
        }
        filas[fila - 1].push_back(carta);
    }

    void tomarFila(int fila, Jugador &jugador)
    {
        if (filaTomada[fila - 1])
        {
            cout << "La fila ya fue tomada." << endl;
            return;
        }

        vector<string> &filaElegida = filas[fila - 1];
        for (const string &carta : filaElegida)
        {
            jugador.agregarCarta(carta);
        }
        filaElegida.clear();
        filaTomada[fila - 1] = true;
    }

    void mostrarFilas()
    {
        for (int i = 0; i < filas.size(); ++i)
        {
            cout << "Fila " << i + 1 << ": ";
            for (const string &carta : filas[i])
            {
                cout << carta << " ";
            }
            cout << (filaTomada[i] ? "(Tomada)" : "") << endl;
        }
    }
};

class Juego
{
private:
    Baraja baraja;
    Filas *filas;
    vector<Jugador> jugadores;
    vector<bool> jugadoresTomaronFila;

public:
    Juego()
    {
        int numJugadores;
        do
        {
            cout << "Ingrese el número de jugadores (mínimo 3, máximo 5): ";
            cin >> numJugadores;
            if (numJugadores < 3 || numJugadores > 5)
            {
                cout << "Número de jugadores no válido. Deben ser entre 3 y 5." << endl;
            }
        } while (numJugadores < 3 || numJugadores > 5);

        for (int i = 0; i < numJugadores; ++i)
        {
            string nombre;
            cout << "Ingrese el nombre del jugador " << i + 1 << ": ";
            cin >> nombre;
            jugadores.push_back(Jugador(nombre));
        }

        filas = new Filas(numJugadores);
        jugadoresTomaronFila.resize(numJugadores, false);
    }

    ~Juego()
    {
        delete filas;
    }

    void iniciarJuego()
    {
        int turno = 0;
        while (true)
        {
            Jugador &jugadorActual = jugadores[turno % jugadores.size()];
            if (jugadoresTomaronFila[turno % jugadores.size()])
            {
                cout << jugadorActual.nombre << " ya tomó una fila y no puede jugar en esta ronda." << endl;
                turno++;
                continue;
            }

            cout << "\nTurno de " << jugadorActual.nombre << endl;
            string carta = baraja.tomarCarta();
            cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
            int filaElegida = eleccionFila(*filas);
            filas->agregarCartaAFila(filaElegida, carta);

            if (todosTomaronFila())
            {
                for (Jugador &jugador : jugadores)
                {
                    jugador.colocarComodines();
                }

                if (baraja.getSize() < 60)
                {
                    cout << "Fin del juego." << endl;
                    break;
                }
                resetearRonda();
                cout << "Todos los jugadores tomaron una fila." << endl;
            }
            turno++;
            filas->mostrarFilas();
        }
        cout << "El juego ha terminado. Quedan pocas cartas en la baraja." << endl;

        mostrarPuntajes();
    }

    bool todosTomaronFila()
    {
        for (bool tomado : jugadoresTomaronFila)
        {
            if (!tomado) return false;
        }
        return true;
    }

    void resetearRonda()
    {
        fill(jugadoresTomaronFila.begin(), jugadoresTomaronFila.end(), false);
    }

    void mostrarPuntajes()
    {
        cout << "\nPuntajes finales:\n";
        for (Jugador &jugador : jugadores)
        {
            jugador.mostrarCartas();
            cout << jugador.nombre << " tiene " << jugador.calcularPuntaje() << " puntos." << endl;
        }
    }

    int eleccionFila(Filas &filas)
    {
        int filaElegida;
        do
        {
            cout << "Seleccione una fila (1-" << filas.filas.size() << "): ";
            cin >> filaElegida;
        } while (filaElegida < 1 || filaElegida > filas.filas.size() || filas.filaTomada[filaElegida - 1]);

        return filaElegida;
    }
};

int main()
{
    Juego juego;
    juego.iniciarJuego();
    return 0;
}
