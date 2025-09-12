#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
// Tabla de hash cerrada para los dominios y dentro de cada nodo de la tabla, otra tabla de hash cerrada para los paths.
// Preguntar profe si se borran doms y el tope de paths
// Preguntar profe precondicion put para usar bool estaborrado

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

    nodoDominio (int topeInicial, string d){
        dominio = d;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial *2);
        tablaPath = new nodoPath* [tope];
        for (int i = 0; i < this -> tope; i++) tablaPath [i] = NULL;
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
    int primeroBorrado = -1;
    while (domActual ->tablaPath [posPath] && domActual -> tablaPath [posPath] ->path != path){
        if (domActual -> tablaPath[posPath] ->estaBorrado && primeroBorrado == -1) primeroBorrado = posPath;
        posPath = fHashColisiones(domActual -> tope, posPath,i);
        i++;
    }

    if (!domActual -> tablaPath[posPath]){
        if (primeroBorrado == -1) posPath = primeroBorrado;
        domActual -> tablaPath [posPath] = new nodoPath(path, titulo, tiempo);
        domActual -> cantElementos++;
    }else{
        if (domActual -> tablaPath[posPath] -> estaBorrado) domActual -> tablaPath[posPath] -> estaBorrado = false;
        domActual -> tablaPath[posPath] -> titulo = titulo;
        domActual -> tablaPath[posPath] -> tiempo = tiempo;
    }
}

nodoPath* GET (Tabla d,string dom, string path){
    int posDom = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while (d -> tablaDoms[posDom] && d -> tablaDoms [posDom] -> dominio != dom){
        posDom = fHashColisiones (d -> tope, posDom,i);
        i++;
    }

    if (!d -> tablaDoms[posDom]) return NULL;
    
    nodoDominio* domActual = d -> tablaDoms [posDom];

    int posPath = fhashPrincipal(domActual -> tope, path);
    i = 1;
    while(domActual -> tablaPath[posPath] && 
        (domActual -> tablaPath[posPath] -> path != path || domActual -> tablaPath[posPath] ->estaBorrado)){ 
        posPath = fHashColisiones(domActual -> tope, posPath, i);
        i++;
    }

    if (!domActual -> tablaPath[posPath]) return NULL;
    
    return domActual -> tablaPath[posPath];
}

void REMOVE (Tabla &d, string dom, string path){
    int posDom = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while (d -> tablaDoms [posDom] && d -> tablaDoms [posDom] ->dominio != dom){
        posDom = fHashColisiones (d -> tope, posDom, i);
        i++;
    }

    nodoDominio* domActual = d -> tablaDoms [posDom];

    if (domActual){
        int posPath = fhashPrincipal(domActual -> tope, path);
        i = 1;
        while (domActual ->tablaPath[posPath] && domActual -> tablaPath[posPath] ->path != path){
            posPath = fHashColisiones(domActual -> tope, posPath, i);
            i++;
        }

        nodoPath* pathActual = domActual ->tablaPath [posPath];

        if (pathActual && !pathActual ->estaBorrado){
            pathActual->estaBorrado = true;
            domActual -> cantElementos--;
        } 
    }
}

bool CONTAINS (Tabla d, string dom, string path){
    int posDom = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while (d -> tablaDoms[posDom] && d -> tablaDoms [posDom] ->dominio != dom){
        posDom = fHashColisiones(d -> tope, posDom, i);
        i++;
    }

    nodoDominio* domActual = d -> tablaDoms[posDom];
    
    if (domActual){
        int posPath = fhashPrincipal (domActual -> tope, path);
        i = 1;
        while (domActual -> tablaPath[posPath] && domActual -> tablaPath[posPath] -> path != path){
            posPath = fHashColisiones(domActual -> tope, posPath, i);
            i++;
        }

        nodoPath* pathActual = domActual -> tablaPath [posPath];

        if (pathActual && !pathActual -> estaBorrado){
            return true;
        }else return false;
    }else return false;
}

int COUNT_DOMAIN (Tabla d, string dom){
    int posDom = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while (d -> tablaDoms[posDom] && d -> tablaDoms[posDom] -> dominio != dom){
        posDom = fHashColisiones(d -> tope, posDom, i);
        i++;
    }

    nodoDominio* domActual = d -> tablaDoms[posDom];

    if (domActual) return domActual -> cantElementos;
    else return 0;
}

int main()
{
    // TODO
    return 0;
}