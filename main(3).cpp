#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

class Baraja
{
private:
    vector<int> baraja;

    string obtenerColor(int numero)
    {
        switch (numero)
        {
        case 0:
            return "Naranjado";
        case 1:
            return "Azul";
        case 2:
            return "Cafe";
        case 3:
            return "Morado";
        case 4:
            return "Verde";
        case 5:
            return "Rojo";
        case 6:
            return "Amarillo";
        case 7:
            return "Sumar 2";
        case 8:
            return "Comodín";
        case 9:
            return "Comodín dorado";
        default:
            return "Error carta inexistente";
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
        srand(time(0));
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
    vector<string> mazo1;
    vector<string> mazo2;
    vector<string> mazo3;
    Jugador(string nombre) : nombre(nombre) {}

    void agregarCarta(int mazo, string carta)
    {
        switch (mazo)
        {
        case 1:
            mazo1.push_back(carta);
            break;
        case 2:
            mazo2.push_back(carta);
            break;
        case 3:
            mazo3.push_back(carta);
            break;
        default:
            cout << "Jugador no válido" << endl;
        }
    }
};

class Filas
{
public:
    vector<vector<string>> filas; // Filas dinámicas según la cantidad de jugadores
    vector<bool> filaTomada;

    // Constructor dinámico basado en la cantidad de filas
    Filas(int numFilas) : filas(numFilas), filaTomada(numFilas, false) {}

    void agregarCartaAFila(int fila, string carta)
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
            jugador.agregarCarta(1, carta); // Aquí puedes elegir a qué mazo va cada carta
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
        int eleccion = 0;
        int turno = 0;
        int primerTurno = 0;
        int ronda = 0;
        bool validacion = false;

        while (baraja.getSize() > 15)
        {
            Jugador &jugadorActual = jugadores[turno % jugadores.size()];
            if (jugadoresTomaronFila[turno % jugadores.size()])
            {
                cout << jugadorActual.nombre << " ya tomó una fila y no puede jugar en esta ronda." << endl;
                turno++;
                continue;
            }

            cout << "\nTurno de " << jugadorActual.nombre << endl;
            if (primerTurno == 0)
            {
                string carta = baraja.tomarCarta();
                cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
                int filaElegida = eleccionFila(*filas);
                filas->agregarCartaAFila(filaElegida, carta);
                primerTurno++;
            }
            else
            {
                do
                {
                    cout << "¿Desea tomar una carta o tomar una fila? \n1. Carta \n2. Fila" << endl;
                    cin >> eleccion;
                    if (eleccion == 1)
                    {
                        string carta = baraja.tomarCarta();
                        cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
                        int filaElegida = eleccionFila(*filas);
                        filas->agregarCartaAFila(filaElegida, carta);
                        validacion = true;
                    }
                    else if (eleccion == 2)
                    {
                        int filaElegida = eleccionFila2(*filas);
                        filas->tomarFila(filaElegida, jugadorActual);
                        cout << jugadorActual.nombre << " ha tomado la fila " << filaElegida << endl;
                        jugadoresTomaronFila[turno % jugadores.size()] = true;
                        validacion = true;
                    }
                    else
                    {
                        validacion = false;
                        cout << "Elija una opción correcta" << endl;
                    }
                } while (!validacion);
            }

            if (todosTomaronFila())
            {
                resetearRonda();
                ronda++;
                cout << "Todos los jugadores tomaron una fila. Inicia la siguiente ronda." << endl;
            }
            turno++;
            filas->mostrarFilas();
        }
        cout << "El juego ha terminado. Quedan 15 cartas en la baraja." << endl;
    }

    bool todosTomaronFila()
    {
        for (bool tomoFila : jugadoresTomaronFila)
        {
            if (!tomoFila)
            {
                return false;
            }
        }
        return true;
    }

    void resetearRonda()
    {
        fill(jugadoresTomaronFila.begin(), jugadoresTomaronFila.end(), false);
        fill(filas->filaTomada.begin(), filas->filaTomada.end(), false);
    }

    int eleccionFila(Filas &filas)
    {
        int filaElegida;
        bool entradaValida = false;
        do
        {
            cout << "Elige una fila para colocar la carta (1, 2, etc.): ";
            cin >> filaElegida;
            if (filaElegida >= 1 && filaElegida <= filas.filas.size() && filas.filas[filaElegida - 1].size() < 3 && !filas.filaTomada[filaElegida - 1])
            {
                entradaValida = true;
            }
            else
            {
                cout << "Error, no puedes agregar más cartas a esta fila." << endl;
            }
        } while (!entradaValida);
        return filaElegida;
    }

    int eleccionFila2(Filas &filas)
    {
        int filaElegida;
        bool entradaValida = false;
        do
        {
            cout << "Elige una fila para tomarla: ";
            cin >> filaElegida;

            if (filaElegida >= 1 && filaElegida <= filas.filas.size() && !filas.filas[filaElegida - 1].empty() && !filas.filaTomada[filaElegida - 1])
            {
                entradaValida = true;
            }
            else
            {
                cout << "Error: fila vacía o ya fue tomada." << endl;
            }
        } while (!entradaValida);
        return filaElegida;
    }
};

int main()
{
    Juego juego;
    juego.iniciarJuego();
    return 0;
}
