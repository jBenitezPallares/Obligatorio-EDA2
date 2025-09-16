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
    nodoPath* sigPath;

    nodoPath (string path, string titulo, int tiempo): path(path), titulo(titulo), tiempo(tiempo), sig(NULL), sigPath(NULL){}
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
    int posDom = fhashPrincipal(d->tope, dom);
    nodoDominio* actual = d->tablaDoms[posDom];

    while(actual && actual->dominio != dom) actual = actual->sig;

    if(!actual){
        actual = new nodoDominio(d->tope, dom);
        actual->sig = d->tablaDoms[posDom];
        d->tablaDoms[posDom] = actual;
    }

    int posPath = fhashPrincipal(actual->tope, path);
    nodoPath* pathActual = actual->tablaPath[posPath];

    while(pathActual && pathActual->path != path) pathActual = pathActual->sigPath;

    if(!pathActual){
        pathActual = new nodoPath(path, titulo, tiempo);
        pathActual->sigPath = actual->tablaPath[posPath];
        actual->tablaPath[posPath] = pathActual;
        d->cantElementos++;
        actual->cantElementos++;
    } else {
        pathActual->titulo = titulo;
        if(pathActual->tiempo != tiempo){
            pathActual->tiempo = tiempo;

            if(actual->primero == pathActual) actual->primero = pathActual->sig;
            else {
                nodoPath* iter = actual-> primero;
                while(iter->sig && iter->sig != pathActual) iter = iter->sig;
                if(iter->sig == pathActual) iter->sig = pathActual->sig;
            }
            pathActual->sig = NULL;

            if(!actual->primero || tiempo > actual->primero->tiempo){
                pathActual->sig = actual->primero;
                actual->primero = pathActual;
            } else {
                nodoPath* iter = actual->primero;
                while(iter->sig && iter->sig->tiempo > tiempo) iter = iter->sig;
                pathActual->sig = iter->sig;
                iter->sig = pathActual;
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
    int posDom = fhashPrincipal(d->tope, dom);
    nodoDominio* actual = d->tablaDoms[posDom];
    while(actual && actual->dominio != dom) actual = actual->sig;

    if(!actual) return;

    int posPath = fhashPrincipal(actual->tope, path);
    nodoPath* pathActual = actual->tablaPath[posPath];
    nodoPath* prevPath = NULL;

    while(pathActual && pathActual->path != path){
        prevPath = pathActual;
        pathActual = pathActual->sigPath;
    }
    if(!pathActual) return;
    if(prevPath) prevPath->sigPath = pathActual->sigPath;
    else actual->tablaPath[posPath] = pathActual->sigPath;
    if(actual->primero == pathActual) actual->primero = pathActual->sig;
    else {
        nodoPath* iter = actual->primero;
        while(iter->sig != pathActual) iter = iter->sig;
        iter->sig = pathActual->sig;
    }
    delete pathActual;
    d->cantElementos--;
    actual->cantElementos--;
}

bool CONTAINS (Tabla d, string dom, string path){
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d -> tablaDoms[pos];
    while(actual && actual ->dominio != dom) actual = actual -> sig;

    if (actual){
        pos = fhashPrincipal(actual -> tope, path);
        nodoPath* pathActual = actual ->tablaPath[pos];
        while (pathActual && pathActual -> path != path) pathActual = pathActual -> sig;

        if (pathActual) return true;
        else return false;
    }else return false;
}

int COUNT_DOMAIN (Tabla d, string dom){ 
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d ->tablaDoms[pos];
    while (actual && actual -> dominio != dom) actual = actual -> sig;

    if (actual) return actual ->cantElementos;
    else return 0;
}

// Imprime los elementos de la lista por recursion.
void listarRec (nodoPath* p){
    if (p){
        cout << p->path << " ";
        listarRec (p ->sig);
    }else cout << endl;
}
void LIST_DOMAIN (Tabla d, string dom){
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d -> tablaDoms[pos];
    while (actual && actual -> dominio != dom) actual = actual -> sig;

    if (actual && actual -> cantElementos != 0) listarRec (actual ->primero);
    else cout << "sin_informacion" << endl;
}

void CLEAR_DOMAIN(Tabla &d, string dom){
    int pos = fhashPrincipal(d -> tope, dom);
    nodoDominio* actual = d -> tablaDoms[pos];
    while (actual && actual -> dominio != dom) actual = actual -> sig;

    if (actual){
        for (int i = 0; i < actual->tope; i++){
            nodoPath* pathActual = actual->tablaPath[i];
            while(pathActual){
                nodoPath* aux = pathActual;
                pathActual = pathActual->sigPath;  
                delete aux;
            }
            actual->tablaPath[i] = NULL;
        }
        actual->primero = NULL;
        actual->cantElementos = 0;
    }
}

int SIZE (Tabla d){
    return d -> cantElementos;
}

void CLEAR(Tabla &d){
    for (int i = 0; i < d->tope; i++) {
        nodoDominio* actual = d->tablaDoms[i];
        while(actual) {
            nodoDominio* aux = actual;
            actual = actual->sig;
            for(int j = 0; j < aux->tope; j++){
                nodoPath* pathActual = aux->tablaPath[j];
                while(pathActual){
                    nodoPath* tmp = pathActual;
                    pathActual = pathActual->sigPath; 
                    delete tmp;
                }
            }
            delete[] aux->tablaPath;
            aux->primero = NULL; 
            delete aux;
        }
        d->tablaDoms[i] = NULL;
    }
    delete[] d->tablaDoms;
    d->tablaDoms = NULL;
    d->cantElementos = 0;
    d->tope = 0;
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
