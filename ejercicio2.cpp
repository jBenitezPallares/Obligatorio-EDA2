#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
// Tabla de hash cerrada para los dominios y dentro de cada nodo de la tabla, otra tabla de hash cerrada para los paths.
// Preguntar profe si se borran doms y el tope de paths
// Preguntar funcion hash de strings;

bool esPrimo(int num){
    if(num<=1 || num%2==0 && num!=2) return false;
    if(num==2) return true;
    for (int i = 3; i < num/2; i+=2)
    {
        if(num%i==0)
        {
            return false;
        }
    }
    return true;    
}

int primoSupMinimo(int dato){
    while(!esPrimo(++dato)); 
    return dato;
}

int fhashPrincipal (int tope, string clave) {
    int ret = 0;
    int i = 0;
    while (clave[i] != '\0'){
        ret = (ret + int (clave[i])) * 37;
        i++;
    }
    return ret % tope;
}

int fHashColisiones (int tope, int n, int i) {
    return (n + i * 2) % tope;
}

struct nodoPath{
    string path;
    int tiempo;
    bool estaBorrado;

    nodoPath (string path, int tiempo): path(path), tiempo(tiempo), estaBorrado(false){}
};

struct nodoDominio {
    string dominio;
    int tope;
    nodoPath** tablaPath;
    int cantElementos;
    //bool estaBorrado; 

    nodoDominio (int topeInicial, string d){
        dominio = d;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial *2);
        tablaPath = new nodoPath* [tope];
        for (int i = 0; i < this -> tope; i++) tablaPath [i] = NULL;
        //estaBorrado = false;
    } 
};

struct representacionDominio{
    int cantElementos;
    int tope;
    nodoDominio** tablaDoms;

    representacionDominio (int topeInicial){
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial * 2);
        tablaDoms = new nodoDominio* [tope];
        for (int i = 0; i < this -> tope; i++) tablaDoms [i] = NULL;
    } 
};

int main()
{
    // TODO
    return 0;
}