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
        ret = (ret + int (clave[i])) * 37; //Funcion de horner
        i++;
    }
    return ret % tope;
}

int fHashColisiones (int tope, int n, int i) {
    return (n + i * 2) % tope;
}

struct nodoPath{
    string path;
    string titulo;
    int tiempo;
    bool estaBorrado;

    nodoPath (string path, string titulo, int tiempo): path(path), titulo(titulo), tiempo(tiempo), estaBorrado(false){}
};

struct nodoDominio {
    string dominio;
    int tope;
    nodoPath** tablaPath;
    int cantElementos;
    bool estaBorrado; 

    nodoDominio (int topeInicial, string d){
        dominio = d;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial *2);
        tablaPath = new nodoPath* [tope];
        for (int i = 0; i < this -> tope; i++) tablaPath [i] = NULL;
        estaBorrado = false;
    } 
};

struct representacionTabla{
    int cantElementos;
    int tope;
    nodoDominio** tablaDoms;

    representacionTabla (int topeInicial){
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial * 2);
        tablaDoms = new nodoDominio* [tope];
        for (int i = 0; i < this -> tope; i++) tablaDoms [i] = NULL;
    } 
};

typedef representacionTabla * Tabla;


void PUT (Tabla &d, string dom, string path, string titulo, int tiempo){
    int pos = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while (d -> tablaDoms [pos] && d -> tablaDoms [pos] -> dominio != dom){
        pos = fHashColisiones (d ->tope, pos, i);
        i++;
    }

    if (!d -> tablaDoms [pos]){
        d -> tablaDoms [pos] = new nodoDominio(d -> tope, dom);
        d -> cantElementos++;
    }

    nodoDominio * domActual = d -> tablaDoms[pos];

    int posPath = fhashPrincipal(domActual -> tope, path);
    i = 1;
    while (domActual ->tablaPath [posPath] && domActual -> tablaPath [posPath] ->path != path){
        posPath = fHashColisiones(domActual -> tope, posPath,i);
        i++;
    }

    if (!domActual -> tablaPath[posPath]){
        domActual -> tablaPath [posPath] = new nodoPath(path, titulo, tiempo);
        domActual -> cantElementos++;
    }else{
        domActual -> tablaPath[posPath] -> titulo = titulo;
        domActual -> tablaPath[posPath] -> tiempo = tiempo;
    }
}


int main()
{
    // TODO
    return 0;
}