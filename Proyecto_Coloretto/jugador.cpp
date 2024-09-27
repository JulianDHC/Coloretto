#include "coloretto.h"
#include <iostream>
#include <algorithm>

using namespace std;

Jugador::Jugador(string nombre) : nombre(nombre) {}

void Jugador::agregarCarta(string carta) {
    cartasRecolectadas.push_back(carta);
}

int Jugador::calcularPuntaje() {
    map<string, int> conteoColores;
        int puntaje = 0;
        int comodinesDorado = 0;
        int sumar2 = 0;
        int comodinesArcoiris = 0;
        
        for (const string &carta : cartasRecolectadas)
        {
            if (carta == "\033[36mSumar 2\033[0m") // Carta "Sumar 2"
            {
                sumar2++;
            }
            else if (carta == "\033[33mComodín dorado\033[0m") // Comodín dorado
            {
                comodinesDorado++;
            }
            else if (carta == "\033[36mComodín\033[0m") //Comodin comun
            {
                comodinesArcoiris++;
            }
            else if (carta != "\033[36mComodín\033[0m")
            {
                conteoColores[carta]++;
            }
        }

        puntaje += 2 * sumar2;

        vector<pair<string, int>> coloresOrdenados(conteoColores.begin(), conteoColores.end());

        sort(coloresOrdenados.begin(), coloresOrdenados.end(), [](const pair<string, int> &a, const pair<string, int> &b) {
            return a.second > b.second;
        });

        for (int i = 0; i < comodinesDorado; ++i)
        {
            if (!coloresOrdenados.empty())
            {
                coloresOrdenados[0].second++;
            }
        }
    
        for (int i = 0; i < comodinesArcoiris; ++i)
        {
            if (!coloresOrdenados.empty())
            {
                coloresOrdenados[0].second++;
            }
        }

        int cartasNegativas = 0;
        for (size_t i = 0; i < coloresOrdenados.size(); ++i)
        {
            int cantidad = coloresOrdenados[i].second;

            if (i < 3)
            {
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
            }
            else
            {
                cartasNegativas += cantidad;
            }
        }
        puntaje -= cartasNegativas;

        return puntaje;
}

void Jugador::mostrarCartas() {
    cout << "Cartas recolectadas por " << nombre << ": ";
    for (const string &carta : cartasRecolectadas) {
        cout << carta << " ";
    }
    cout << endl;
}
