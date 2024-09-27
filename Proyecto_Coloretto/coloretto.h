#ifndef COLORETTO_H
#define COLORETTO_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Baraja {
private:
    vector<int> baraja;
    map<int, string> colorMap;

    void inicializarColorMap() {
        colorMap = {
            {0, "\033[38;5;214mNaranjado\033[0m"},
            {1, "\033[34mAzul\033[0m"},
            {2, "\033[33mCafe\033[0m"},
            {3, "\033[35mMorado\033[0m"},
            {4, "\033[32mVerde\033[0m"},
            {5, "\033[31mRojo\033[0m"},
            {6, "\033[33mAmarillo\033[0m"},
            {7, "\033[36mSumar 2\033[0m"},
            {8, "\033[36mComodín\033[0m"},
            {9, "\033[33mComodín dorado\033[0m"}
        };
    }

public:
    Baraja();
    string obtenerColor(int numero);
    int getSize() const;
    string tomarCarta();
};

class Jugador {
public:
    string nombre;
    vector<string> cartasRecolectadas;

    Jugador(string nombre);
    void agregarCarta(string carta);
    int calcularPuntaje();
    void mostrarCartas();
};

class Filas {
public:
    vector<vector<string>> filas;
    vector<bool> filaTomada;

    Filas(int numFilas);
    void agregarCartaAFila(int fila, string carta);
    void tomarFila(int fila, Jugador &jugador);
    void mostrarFilas();
};

class Juego {
private:
    Baraja baraja;
    Filas *filas;
    vector<Jugador> jugadores;
    vector<bool> jugadoresTomaronFila;
    
    bool todosTomaronFila();
    void resetearRonda();
    int eleccionFila(Filas &filas);
    int eleccionFila2(Filas &filas);

public:
    Juego();
    ~Juego();
    void iniciarJuego();
    void mostrarPuntajes();
};

#endif // COLORETTO_H