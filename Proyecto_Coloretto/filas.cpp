#include "coloretto.h"
#include <iostream>

using namespace std;

Filas::Filas(int numFilas) : filas(numFilas), filaTomada(numFilas, false) {}

void Filas::agregarCartaAFila(int fila, string carta) {
    if (filaTomada[fila - 1])
    {
        cout << "No se pueden agregar cartas a una fila que ya fue tomada." << endl;
        return;
    }
    filas[fila - 1].push_back(carta);
}

void Filas::tomarFila(int fila, Jugador &jugador) {
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

void Filas::mostrarFilas() {
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