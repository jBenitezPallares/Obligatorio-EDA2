#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

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

struct nodoPath{
    string path;
    string titulo;
    int tiempo;
    nodoPath* sig;
    nodoPath* ant;
    nodoPath* sigPath;

    nodoPath (string path, string titulo, int tiempo): path(path), titulo(titulo), tiempo(tiempo), sig(NULL), ant(NULL), sigPath(NULL){}
};

struct nodoDominio {
    string dominio;
    int tope;
    nodoPath** tablaPath;
    int cantElementos;
    nodoPath* primero;
    nodoDominio* sig;

    nodoDominio (int topeInicial, string d){
        dominio = d;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial *2);
        tablaPath = new nodoPath* [tope];
        for (int i = 0; i < this -> tope; i++) tablaPath [i] = NULL;
        primero = NULL;
        sig = NULL;
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

Tabla crear(int tope){
    return new representacionTabla(tope);
}

void PUT (Tabla &d, string dom, string path, string titulo, int tiempo){
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d ->tablaDoms[pos];
    while (actual && actual ->dominio != dom) actual = actual -> sig;
    if(!actual){
        nodoDominio* nuevoDom = new nodoDominio(d->tope, dom);
        nuevoDom->sig = d->tablaDoms[pos];
        d->tablaDoms[pos] = nuevoDom;     
        actual = nuevoDom;
        actual ->primero = new nodoPath (path, titulo, tiempo); 
        actual ->tablaPath[pos] = actual -> primero; 
        d -> cantElementos++;
        actual->cantElementos++;  
        return;    
    }

    pos = fhashPrincipal(actual -> tope, path);
    nodoPath* pathActual = actual ->tablaPath[pos];
    while (pathActual && pathActual ->path != path) pathActual = pathActual -> sig;
    if (!pathActual){
        nodoPath* nuevoPath = new nodoPath(path, titulo, tiempo);
        nuevoPath -> sig = actual -> tablaPath[pos];
        actual ->tablaPath[pos] = nuevoPath;
        d -> cantElementos++;
        actual -> cantElementos++;
    }else{
        pathActual ->titulo = titulo;
        pathActual -> tiempo = tiempo;
    }

    if (!actual -> primero){
        actual -> primero = pathActual;
        pathActual ->ant = NULL;
        pathActual-> sig = NULL;
    }else {
        nodoPath* auxAnt = pathActual -> ant;
        nodoPath* auxSig = pathActual -> sig;
        if(auxAnt) auxAnt -> sig = auxSig;
        if(auxSig) auxSig -> ant = auxAnt;
        pathActual -> sig = NULL;
        pathActual -> ant = NULL;
        if (actual -> primero -> tiempo <= tiempo) {
            pathActual -> sig = actual -> primero;
            actual -> primero -> ant = pathActual;
            actual -> primero = pathActual;
        }else{
            nodoPath* iterador = actual -> primero;
            while(iterador -> sig && iterador ->sig -> tiempo > tiempo) iterador = iterador -> sig;

            if (!iterador -> sig){
                iterador -> sig = pathActual;
                pathActual -> ant = iterador;
            }else{
                nodoPath* aux = iterador->sig;
                iterador->sig = pathActual;
                pathActual->ant = iterador;
                pathActual->sig = aux;
                aux->ant = pathActual;
            }
        }
         
    }
}

nodoPath* GET (Tabla d,string dom, string path){
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d ->tablaDoms[pos];
    while (actual && actual -> dominio != dom) actual = actual ->sig;

    if (!actual) return NULL;
    else {
        pos = fhashPrincipal (actual -> tope, path);
        nodoPath* pathActual = actual -> tablaPath[pos];
        while (pathActual && pathActual ->path != path) pathActual = pathActual ->sig;

        if (!pathActual) return NULL;
        else return pathActual;
    }
}

void REMOVE (Tabla &d, string dom, string path){
    
}

bool CONTAINS (Tabla d, string dom, string path){
    return false;
}

int COUNT_DOMAIN (Tabla d, string dom){ 
    return 0;
}

// Imprime los elementos de la lista por recursion.
void listarRec (nodoPath* p){

}
void LIST_DOMAIN (Tabla d, string dom){
    
}

void CLEAR_DOMAIN(Tabla &d, string dom){
    
}

int SIZE (Tabla d){
    return 0;
}

void CLEAR (Tabla &d){
    
}

string getTitulo(nodoPath* n){
    return n ->titulo;
}

int getTiempo (nodoPath* n){
    return n -> tiempo;
}

int main()
{
    int cantAcciones;
    cin >> cantAcciones;
    Tabla datos = crear(cantAcciones);
    string accion;

    for (int i = 0; i < cantAcciones; i++){
        cin >> accion;
        if (accion == "PUT") {
            string dom, path, titulo;
            int tiempo;
            cin >> dom >> path >> titulo >> tiempo;
            PUT(datos, dom, path, titulo, tiempo);
        }
        else if (accion == "GET") {
            string dom, path;
            cin >> dom >> path;
            nodoPath* nodo = GET(datos, dom, path);
            if (nodo) cout << getTitulo(nodo) << " " << getTiempo(nodo) << endl;
            else cout << "recurso_no_encontrado" << endl;
        }
        else if (accion == "REMOVE") {
            string dom, path;
            cin >> dom >> path;
            REMOVE(datos, dom, path);
        }
        else if (accion == "CONTAINS") {
            string dom, path;
            cin >> dom >> path;
            if (CONTAINS(datos, dom, path)) cout << "true" << endl;
            else cout << "false" << endl;
        }
        else if (accion == "COUNT_DOMAIN") {
            string dom;
            cin >> dom;
            cout << COUNT_DOMAIN(datos, dom) << endl;
        }
        else if (accion == "LIST_DOMAIN") {
            string dom;
            cin >> dom;
            LIST_DOMAIN(datos, dom);
        }
        else if (accion == "CLEAR_DOMAIN") {
            string dom;
            cin >> dom;
            CLEAR_DOMAIN(datos, dom);
        }
        else if (accion == "SIZE") {
            cout << SIZE(datos) << endl;
        }
        else { 
            CLEAR(datos);
        }
    }
    return 0;
}
