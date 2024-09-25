#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>

//Las cosas consultadas van a estar comentadas con la explicación, para aprender a usarlas después.

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
		//srand y random_suffle son funciones importadas desde librerías. srand(time) sirve para que la aleatoriedad de los numeros 
		//sea distinta cada que ejecutemos el programa. Ya que con el rand normal se generan pero luego siguen una secuencia.
		//random_suffle: Reorganiza los elementos del rango (primera posicion , ultima posicion) de forma aleatoria.
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
    vector<string> fila1;
    vector<string> fila2;
    vector<string> fila3;
    vector<bool> filaTomada; 
    
    //constructor, inicializa un vector de booleanos que nos va a servir pa saber si la fila fue tomada.
    Filas():filaTomada(3, false) {}

    void agregarCartaAFila(int fila, string carta)
    {
        if (filaTomada[fila - 1]) {
            cout << "No se pueden agregar cartas a una fila que ya fue tomada." << endl;
            return;
        }
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
    
    void tomarFila(int fila, Jugador &jugador)
    {
        //nullptr significa que el apuntador no se dirige a ninguna dirección de la memoria.
        //Eso nos ayuda a que podamos dirigirlo a cualquiera de las 3 filas para, pues, tomarla.
        vector<string> *filaElegida = nullptr;
        
        switch (fila)
        {
        case 1:
            filaElegida = &fila1;
            break;
        case 2:
            filaElegida = &fila2;
            break;
        case 3:
            filaElegida = &fila3;
            break;
        default:
            cout << "Fila inválida." << endl;
            return;
        }
        
        //clear se usa para limpiar el vector, dejandolo vacío.
        filaElegida->clear();
        filaTomada[fila - 1] = true;
    }

    void mostrarFilas()
    {
        cout << "Fila 1: ";
        for (string carta : fila1)
        {
            cout << carta << " ";
        }
        cout << (filaTomada[0] ? "(Tomada)" : "") << endl;

        cout << "Fila 2: ";
        for (string carta : fila2)
        {
            cout << carta << " ";
        }
        cout << (filaTomada[1] ? "(Tomada)" : "") << endl;

        cout << "Fila 3: ";
        for (string carta : fila3)
        {
            cout << carta << " ";
        }
        cout << (filaTomada[2] ? "(Tomada)" : "") << endl;
    }
};


class Juego
{
private:
	Baraja baraja;
	Filas filas;
	vector<Jugador> jugadores;
	vector<bool> jugadoresTomaronFila; // Indica si cada jugador ya tomó una fila en la ronda actual.

public:
    Juego()
    {
        jugadores.push_back(Jugador("Juan"));
        jugadores.push_back(Jugador("Olman"));
        jugadores.push_back(Jugador("Julián"));
        jugadoresTomaronFila.resize(jugadores.size(), false); // Inicializa el vector a "false".
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
                //continue funciona para seguir en el siguiente ciclo, por lo que el jugador se saltaría.
                continue;
            }

            cout << "\nTurno de " << jugadorActual.nombre << endl;

            if (primerTurno == 0)
            {
                string carta = baraja.tomarCarta();
                cout << jugadorActual.nombre << " ha tomado la carta: " << carta << endl;
                int filaElegida = eleccionFila(filas);
                filas.agregarCartaAFila(filaElegida, carta);
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
                        int filaElegida = eleccionFila(filas);
                        filas.agregarCartaAFila(filaElegida, carta);
                        validacion = true;
                    }
                    else if (eleccion == 2)
                    {
                        int filaElegida = eleccionFila2(filas);
                        filas.tomarFila(filaElegida, jugadorActual);
                        cout << jugadorActual.nombre << " ha tomado la fila " << filaElegida << endl;
                        jugadoresTomaronFila[turno % jugadores.size()] = true; // Marcamos que el player tomó la fila.
                        validacion = true;
                    }
                    else
                    {
                        validacion = false;
                        cout << "Elija una opción correcta" << endl;
                    }
                } while (!validacion);
            }

            // Comprobar si todos los jugadores tomaron una fila
            if (todosTomaronFila())
            {
                resetearRonda(filas);
                ronda++;
                cout << "Todos los jugadores tomaron una fila. Inicia la siguiente ronda." << endl;
            }

            turno++;
            filas.mostrarFilas();
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

    void resetearRonda(Filas& filas)
    {
        // Fill sirve para llenar el vector de inicio a fin con el valor; en este caso: false.
        fill(jugadoresTomaronFila.begin(), jugadoresTomaronFila.end(), false);
        fill(filas.filaTomada.begin(), filas.filaTomada.end(), false);
    }

	int eleccionFila(Filas& filas)
	{
		int filaElegida;
		bool entradaValida = false;
		do
		{
			cout << "Elige una fila para colocar la carta (1, 2 o 3): ";
			cin >> filaElegida;

			if (filaElegida == 1 && filas.fila1.size() < 3 && filas.filaTomada[0] == false)
			{
				entradaValida = true;
			}
			else if (filaElegida == 2 && filas.fila2.size() < 3 && filas.filaTomada[1] == false)
			{
				entradaValida = true;
			}
			else if (filaElegida == 3 && filas.fila3.size() < 3 && filas.filaTomada[2] == false)
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
	int eleccionFila2(Filas& filas)
	{
		int filaElegida;
		bool entradaValida = false;
		do
		{
			cout << "Elige una fila para tomarla (1, 2 o 3): ";
			cin >> filaElegida;
			
			//.empty nos dice si el vector está vacío. Si está vacío pues no la puede tomar.
			if (filaElegida == 1 && filas.fila1.empty())
			{
				entradaValida = false;
				cout << "Error: fila vacía" << endl;
			}
			else if (filaElegida == 2 && filas.fila2.empty())
			{
				entradaValida = false;
				cout << "Error: fila vacía" << endl;

			}
			else if (filaElegida == 3 && filas.fila3.empty())
			{
				entradaValida = false;
				cout << "Error: fila vacía" << endl;
			}
			else
			{
			    entradaValida = true;
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