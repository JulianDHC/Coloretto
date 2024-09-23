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
    vector <int> baraja;
    public:
    Baraja()
    {
        for (int i = 0; i < 7; ++i) 
        {
            for (int j = 0; j < 9; ++j) 
            {
                baraja.push_back(i);
            }
        }
        srand(time(0));
        random_shuffle(baraja.begin(), baraja.end());
    };
    
    int getSize() const {
        return baraja.size();
    };
    
    int tomarCarta()
    {
        if (baraja.empty()) {
            throw out_of_range("Fila vacía.");
        }
        int cart = baraja.back(); 
        baraja.pop_back();
        return cart;
    };
};

class Jugador {
public:
    string nombre;
    vector<int> mazo;

    Jugador(string nombre):nombre(nombre) {}

    void agregarCarta(int carta) {
        mazo.push_back(carta);
    }

    void mostrarMazo() {
        cout << "Mazo de " << nombre << ": ";
        for (int carta : mazo) {
            cout << carta << " ";
        }
        cout << endl;
    }
};

class Filas {
public:
    vector<int> fila1;
    vector<int> fila2;
    vector<int> fila3;

    void agregarCartaAFila(int fila, int carta) {
        switch (fila) {
            case 1:
                fila1.push_back(carta);
                break; 
            case 2:
                fila2.push_back(carta);
                break;
            case 3:
                fila3.push_back(carta);
                break;
            default:
                cout << "Fila inválida." << endl;
                break;
        }
    }

    void mostrarFilas() {
        cout << "Fila 1: ";
        for (int carta:fila1) {
            cout << carta << " ";
        }
        cout << endl;

        cout << "Fila 2: ";
        for (int carta : fila2) {
            cout << carta << " ";
        }
        cout << endl;

        cout << "Fila 3: ";
        for (int carta : fila3) {
            cout << carta << " ";
        }
        cout << endl;
    }
};

class Juego {
private:
    Baraja baraja;
    Filas filas;
    vector<Jugador> jugadores;

public:
    Juego() 
    {
        jugadores.push_back(Jugador("Juan"));
        jugadores.push_back(Jugador("Olman"));
        jugadores.push_back(Jugador("Julián"));
    }

    void iniciarJuego() 
    {
        int turno = 0; 
        while (baraja.getSize() > 15)
        {
            Jugador &jugadorActual = jugadores[turno % jugadores.size()];
            cout << "\nTurno de " << jugadorActual.nombre << endl;
            
            int carta = baraja.tomarCarta();
            cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
            
            int filaElegida = eleccionFila();
            switch (filaElegida) 
            {
            case 1:
                if (filas.fila1.size() < 3)
                {
                    filas.agregarCartaAFila(filaElegida, carta);
                    break;
                }
                else
                {
                    cout << "Error no se puede agregar más cartas, elija otra fila.";
                    filaElegida = eleccionFila();
                }
                break;
            case 2:
                if (filas.fila2.size() < 3)
                {
                    filas.agregarCartaAFila(filaElegida, carta);
                    break;
                }
                else
                {
                    cout << "Error no se puede agregar más cartas, elija otra fila.";
                    filaElegida = eleccionFila();
                }
                break; 
            case 3:
                if (filas.fila3.size() < 3)
                {
                    filas.agregarCartaAFila(filaElegida, carta);
                    break;
                }
                else
                {
                    cout << "Error no se puede agregar más cartas, elija otra fila.";
                    filaElegida = eleccionFila();
                }
                break; 
            default:
                cout << "Fila inválida." << endl;
                break;
                
            }
            turno++;
            filas.mostrarFilas();

        }
        cout << "El juego ha terminado. Quedan 15 cartas en la baraja." << endl;
        
    }
    
    int eleccionFila()
    {
        int filaElegida;
        cout << "Elige una fila para colocar la carta (1, 2 o 3): ";
        cin >> filaElegida;
        return filaElegida;
    }
};

int main() {
    Juego juego;
    juego.iniciarJuego();
    return 0;
}