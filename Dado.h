#include <cstdlib>
#include <iostream>
#include <string>
#include <stdlib.h>     
#include <time.h>  

/*#include "jugador.cpp"
#include "juego.cpp"*/
using namespace std;

class Dado{
    
    public:
    int lanzamiento; 
    int lanzar();
};

int Dado::lanzar(){
    srand (time(NULL));
    lanzamiento = rand()% 6 + 1 ;
    
    if(lanzamiento!=6){
        
        cout<< lanzamiento << endl ; 
        return lanzamiento;      
    }
    
    
    cout<<"Perdiste en la ronda" <<endl;
    return 0;
}