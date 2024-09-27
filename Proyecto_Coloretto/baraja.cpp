#include "coloretto.h"
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

string Baraja::obtenerColor(int numero) {
    if (colorMap.count(numero) > 0) {
            return colorMap[numero];
        } else {
            return "\033[31mError carta inexistente\033[0m";
        }
}

Baraja::Baraja() 
{
    inicializarColorMap();
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

int Baraja::getSize() const {
    return baraja.size();
}

string Baraja::tomarCarta() {
    if (baraja.empty())
        {
            return "\033[31mNo quedan cartas\033[0m";
        }
        int cart = baraja.back();
        baraja.pop_back();
        return obtenerColor(cart);
}