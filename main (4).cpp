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
		return obtenerColor(cart); // Devuelve el color en lugar del número
	}
};

class Jugador
{
public:
	string nombre;
	vector<string> mazo;

	Jugador(string nombre) : nombre(nombre) {}

	void agregarCarta(string carta)
	{
		mazo.push_back(carta);
	}

	void mostrarMazo()
	{
		cout << "Mazo de " << nombre << ": ";
		for (string carta : mazo)
		{
			cout << carta << " ";
		}
		cout << endl;
	}
};

class Filas
{
public:
	vector<string> fila1;
	vector<string> fila2;
	vector<string> fila3;

	void agregarCartaAFila(int fila, string carta)
	{
		switch (fila)
		{
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

	void mostrarFilas()
	{
		cout << "Fila 1: ";
		for (string carta : fila1)
		{
			cout << carta << " ";
		}
		cout << endl;

		cout << "Fila 2: ";
		for (string carta : fila2)
		{
			cout << carta << " ";
		}
		cout << endl;

		cout << "Fila 3: ";
		for (string carta : fila3)
		{
			cout << carta << " ";
		}
		cout << endl;
	}
};

class Juego
{
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

			string carta = baraja.tomarCarta();
			cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
			int filaElegida = eleccionFila(filas);

			filas.agregarCartaAFila(filaElegida, carta);
			turno++;
			filas.mostrarFilas();

		}
		cout << "El juego ha terminado. Quedan 15 cartas en la baraja." << endl;

	}

	int eleccionFila(Filas& filas)
	{
		int filaElegida;
		bool entradaValida = false;
		do
		{
			cout << "Elige una fila para colocar la carta (1, 2 o 3): ";
			cin >> filaElegida;

			if (filaElegida == 1 && filas.fila1.size() < 3)
			{
				entradaValida = true;
			}
			else if (filaElegida == 2 && filas.fila2.size() < 3)
			{
				entradaValida = true;
			}
			else if (filaElegida == 3 && filas.fila3.size() < 3)
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
};

int main()
{
	Juego juego;
	juego.iniciarJuego();
	return 0;
}