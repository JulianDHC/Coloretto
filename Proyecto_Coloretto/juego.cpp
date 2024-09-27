#include "coloretto.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

Juego::Juego() {
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

Juego::~Juego() {
    delete filas;
}

void Juego::iniciarJuego() {
    int eleccion = 0;
        int turno = 0;
        int primerTurno = 0;
        int ronda = 0;
        bool validacion = false;
        bool Fin = true;
        bool filasLlenasCond = true;
        int condFilasLlenas = 0;

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
                    if ( filasLlenasCond == true)
                    {
                    cout << "¿Desea tomar una carta o tomar una fila? \n1. Carta \n2. Fila" << endl;
                    cin >> eleccion;
                    }
                    else
                    {
                        eleccion == 2;
                    }
                    bool filasLlenas = true;
                    for (const auto &fila : filas->filas)
                    {
                        if (fila.size() < 3)
                        {
                            filasLlenas = false;
                            break;
                        }
                    }
                    if (filasLlenas)
                    {
                        cout << "Todas las filas están llenas. Debes elegir una fila." << endl;
                        eleccion = 2;
                        filasLlenasCond = false;
                    }

                    if (eleccion == 1 && !filasLlenas)
                    {
                        string carta = baraja.tomarCarta();
                        cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
                        int filaElegida = eleccionFila(*filas);
                        filas->agregarCartaAFila(filaElegida, carta);
                        validacion = true;
                    }
                    else if (eleccion == 1 && filasLlenas)
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
                filasLlenasCond = true;
                cout << "Todos los jugadores tomaron una fila." << endl;
            }
            turno++;
            filas->mostrarFilas();
        }
    cout << "El juego ha terminado. Quedan pocas cartas en la baraja." << endl;

    mostrarPuntajes();
}

bool Juego::todosTomaronFila() {
    for (bool tomoFila : jugadoresTomaronFila)
    {
        if (!tomoFila)
        {
            return false;
        }
    }
    return true;
}

void Juego::mostrarPuntajes() {
    int maxPuntaje = 0;
    vector<Jugador*> ganadores;

    for (Jugador &jugador : jugadores)
    {
        jugador.mostrarCartas();
        int puntaje = jugador.calcularPuntaje();
        cout << "Puntaje de " << jugador.nombre << ": " << puntaje << endl;

        if (puntaje > maxPuntaje) {
            maxPuntaje = puntaje;
            ganadores.clear();
            ganadores.push_back(&jugador);
        } else if (puntaje == maxPuntaje) {
            ganadores.push_back(&jugador);
        }
    }

    cout << "\n--- Resultados Finales ---\n";
    if (ganadores.size() == 1) {
        cout << "El ganador es: " << ganadores[0]->nombre << " con " << maxPuntaje << " puntos!" << endl;
    } else {
        cout << "Hay un empate! Los ganadores son:\n";
        for (const auto& ganador : ganadores) {
            cout << "- " << ganador->nombre << " con " << maxPuntaje << " puntos\n";
        }
    }
}

void Juego::resetearRonda() {
    fill(jugadoresTomaronFila.begin(), jugadoresTomaronFila.end(), false);
    fill(filas->filaTomada.begin(), filas->filaTomada.end(), false);
}

int Juego::eleccionFila(Filas &filas) {
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

int Juego::eleccionFila2(Filas &filas) 
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