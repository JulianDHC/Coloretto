#include <map> 
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

  // Función que devuelve el nombre y el color de la carta usando códigos ANSI
    string obtenerColor(int numero)
    {
        switch (numero)
        {
        case 0:
            return "\033[38;5;214mNaranjado\033[0m";  // Naranjado (Amarillo)
        case 1:
            return "\033[34mAzul\033[0m";      // Azul
        case 2:
            return "\033[33mCafe\033[0m";      // Café (Amarillo)
        case 3:
            return "\033[35mMorado\033[0m";    // Morado
        case 4:
            return "\033[32mVerde\033[0m";     // Verde
        case 5:
            return "\033[31mRojo\033[0m";      // Rojo
        case 6:
            return "\033[33mAmarillo\033[0m";  // Amarillo
        case 7:
            return "\033[36mSumar 2\033[0m";   // Cyan (Sumar 2)
        case 8:
            return "\033[36mComodín\033[0m";   // Cyan (Comodín)
        case 9:
            return "\033[33mComodín dorado\033[0m";  // Dorado (Amarillo)
        default:
            return "\033[31mError carta inexistente\033[0m";  // Rojo (Error)
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
    vector<string> cartasRecolectadas; // Almacena todas las cartas recolectadas por el jugador

    Jugador(string nombre) : nombre(nombre) {}

    // Añadir una carta al vector de cartas recolectadas
    void agregarCarta(string carta)
    {
        cartasRecolectadas.push_back(carta);
    }

    // Calcular los puntos basados en las reglas de Coloretto
    int calcularPuntaje()
    {
        map<string, int> conteoColores;
        int puntaje = 0;
        int cartasNegativas = 0; // Para colores que empiezan a restar puntos

        // Contar cuántas cartas de cada color tiene el jugador
        for (const string &carta : cartasRecolectadas)
        {
            // Ignorar comodines para este cálculo
            if (carta != "Sumar 2" && carta != "Comodín" && carta != "Comodín dorado")
            {
                
                conteoColores[carta]++;
            }
        }

        // Calcular el puntaje por cada color
        for (auto const &entry : conteoColores)
        {
            int cantidad = entry.second;

            if (cantidad == 1)
            {
                puntaje += 1;
            }
            else if (cantidad == 2)
            {
                puntaje += 3;
            }
            else if (cantidad == 3)
            {
                puntaje += 6;
            }
            else if (cantidad == 4)
            {
                puntaje += 10;
            }
            else if (cantidad == 5)
            {
                puntaje += 15;
            }
            else if (cantidad >= 6)
            {
                puntaje += 21;
            }

            // Regla de más de 3 colores empiezan a restar
            if (conteoColores.size() > 3)
            {
                cartasNegativas += cantidad;
            }
        }

        // Restar puntos por exceso de colores
        if (cartasNegativas > 3)
        {
            puntaje -= (cartasNegativas - 3); // Por cada color adicional se resta
        }

        return puntaje;
    }

    // Mostrar las cartas recolectadas
    void mostrarCartas()
    {
        cout << "Cartas recolectadas por " << nombre << ": ";
        for (const string &carta : cartasRecolectadas)
        {
            cout << carta << " ";
        }
        cout << endl;
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
        jugador.agregarCarta(carta); // Agrega cada carta recolectada al vector del jugador
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
        bool Fin = true;
        
        while (Fin == true)
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
                    if (filas->filas1[0]==2 && filas->filas1[1] ==2 )
                    {
                        cout<<"Filas llenas. Debes elegir una fila"<<endl;
                        eleccion = 2;
                    }
                    
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
                if (baraja.getSize() < 60)
                {
                    Fin = false;
                }
                resetearRonda();
                ronda++;
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
        for (bool tomoFila : jugadoresTomaronFila)
        {
            if (!tomoFila)
            {
                return false;
            }
        }
        return true;
    }

    void mostrarPuntajes()
    {
    for (Jugador &jugador:jugadores)
        {
        jugador.mostrarCartas();
        int puntaje = jugador.calcularPuntaje();
        cout << "Puntaje de " << jugador.nombre << ": " << puntaje << endl;
        }
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