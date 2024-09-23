#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

class Baraja {
private:
    vector<int> baraja;

public:
    Baraja() {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 9; ++j) {
                baraja.push_back(i);
            }
        }
        srand(time(0));
        random_shuffle(baraja.begin(), baraja.end());
    }

    int getSize() const {
        return baraja.size();
    }

    int tomarCarta() {
        if (baraja.empty()) {
            throw out_of_range("Fila vacía.");
        }
        int cart = baraja.back();
        baraja.pop_back();
        return cart;
    }
};

class Jugador {
public:
    string nombre;
    vector<int> mazo;

    Jugador(string nombre) : nombre(nombre) {}

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
private:
    vector<vector<int>> filas;  // Vector de vectores para manejar un número variable de filas

public:
    // Constructor que crea el número de filas basado en el número de jugadores
    Filas(int numFilas) {
        filas.resize(numFilas);
    }

    // Agregar carta a una fila específica
    void agregarCartaAFila(int fila, int carta) {
        if (fila >= 1 && fila <= filas.size()) {
            if (filas[fila - 1].size() < 3) {  // Máximo 3 cartas por fila
                filas[fila - 1].push_back(carta);
            } else {
                cout << "Error: La fila " << fila << " ya tiene 3 cartas." << endl;
            }
        } else {
            cout << "Fila inválida." << endl;
        }
    }

    // Mostrar todas las filas con su contenido
    void mostrarFilas() {
        for (int i = 0; i < filas.size(); i++) {
            cout << "Fila " << i + 1 << ": ";
            for (int carta : filas[i]) {
                cout << carta << " ";
            }
            cout << endl;
        }
    }

    // Obtener el número de filas
    int getNumeroFilas() const {
        return filas.size();
    }
};

class Juego {
private:
    Baraja baraja;
    Filas* filas;  // Apuntador a Filas para manejar dinámicamente
    vector<Jugador> jugadores;

public:
    Juego() : filas(nullptr) {}

    // Configurar el número de jugadores y filas dinámicas
    void configurarJugadores() {
        int numJugadores;
        do {
            cout << "Ingrese el número de jugadores (3 a 5): ";
            cin >> numJugadores;
        } while (numJugadores < 3 || numJugadores > 5);

        for (int i = 0; i < numJugadores; ++i) {
            string nombre;
            cout << "Ingrese el nombre del jugador " << i + 1 << ": ";
            cin >> nombre;
            jugadores.push_back(Jugador(nombre));
        }

        // Crear filas con el mismo número que el de jugadores
        filas = new Filas(numJugadores);
    }

    // Iniciar el juego con las reglas establecidas
    void iniciarJuego() {
        int turno = 0;
        while (baraja.getSize() > 15) {
            Jugador &jugadorActual = jugadores[turno % jugadores.size()];
            cout << "\nTurno de " << jugadorActual.nombre << endl;

            int carta = baraja.tomarCarta();
            cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;

            int filaElegida = eleccionFila();
            filas->agregarCartaAFila(filaElegida, carta);

            turno++;
            filas->mostrarFilas();
        }

        cout << "El juego ha terminado. Quedan 15 cartas en la baraja." << endl;
    }

    // Elección de fila por parte del jugador, con validación
    int eleccionFila() {
        int filaElegida;
        do {
            cout << "Elige una fila para colocar la carta (1 a " << filas->getNumeroFilas() << "): ";
            cin >> filaElegida;

            if (filaElegida < 1 || filaElegida > filas->getNumeroFilas()) {
                cout << "Error: Fila inválida. Elige una fila que exista." << endl;
            }
        } while (filaElegida < 1 || filaElegida > filas->getNumeroFilas()); // Repite hasta que la fila sea válida

        return filaElegida;
    }

    // Destructor para limpiar memoria
    ~Juego() {
        delete filas;
    }
};

int main() {
    Juego juego;
    juego.configurarJugadores();
    juego.iniciarJuego();
    return 0;
}
