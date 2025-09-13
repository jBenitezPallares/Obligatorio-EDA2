#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;
// Tabla de hash cerrada para los dominios y dentro de cada nodo de la tabla, otra tabla de hash cerrada para los paths.
// Preguntar profe si se borran doms y el tope de paths
// Preguntar profe precondicion put para usar bool estaborrado
//Preguntar orden countdomain
// cumplir orden listdomain

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
    nodoPath* sig;
    nodoPath* ant;

    nodoPath (string path, string titulo, int tiempo): path(path), titulo(titulo), tiempo(tiempo), estaBorrado(false), sig(NULL), ant(NULL){}
};

struct nodoDominio {
    string dominio;
    int tope;
    nodoPath** tablaPath;
    int cantElementos;
    nodoPath* primero;

    nodoDominio (int topeInicial, string d){
        dominio = d;
        cantElementos = 0;
        tope = primoSupMinimo (topeInicial *2);
        tablaPath = new nodoPath* [tope];
        for (int i = 0; i < this -> tope; i++) tablaPath [i] = NULL;
        primero = NULL;
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
    int i = 1;
    while (d -> tablaDoms [pos] && d -> tablaDoms [pos] -> dominio != dom){
        pos = fHashColisiones (d ->tope, pos, i);
        i++;
    }

    if (!d -> tablaDoms [pos]){
        nodoPath* nuevo = new nodoPath (path, titulo, tiempo);
        d -> tablaDoms [pos] = new nodoDominio(d -> tope, dom);
        d -> cantElementos++;
        int posPath = fhashPrincipal(d -> tablaDoms [pos] -> tope, path);
        d -> tablaDoms[pos] -> tablaPath[posPath] = nuevo;
        d -> tablaDoms [pos] -> cantElementos++;
        d -> tablaDoms[pos] -> primero = nuevo;
        return;
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

    nodoPath* insertar = NULL;

    if(!domActual->tablaPath[posPath]){
        if(primeroBorrado != -1) posPath = primeroBorrado;
        insertar = new nodoPath(path, titulo, tiempo);
        domActual->tablaPath[posPath] = insertar;
        domActual->cantElementos++;
        d -> cantElementos++;
    } else {
        insertar = domActual->tablaPath[posPath];
        if(insertar->estaBorrado) {
            insertar->estaBorrado = false;
            domActual ->cantElementos++;
            d->cantElementos++;
        }
        insertar->titulo = titulo;
        insertar->tiempo = tiempo;
    }

    if(!domActual->primero){
        domActual->primero = insertar;
        insertar->sig = NULL;
        insertar->ant = NULL;
    }else{
        nodoPath* auxAnt = insertar->ant;
        nodoPath* auxSig = insertar->sig;
        if(auxAnt) auxAnt->sig = auxSig;
        if(auxSig) auxSig->ant = auxAnt;
        if(domActual->primero == insertar) domActual->primero = auxSig;
        insertar->ant = NULL;
        insertar->sig = NULL;

        nodoPath* actual = domActual->primero;
        if(!actual || tiempo > actual->tiempo){
            insertar->sig = actual;
            if(actual) actual->ant = insertar;
            insertar->ant = NULL;
            domActual->primero = insertar;
        } else {
            while(actual->sig && actual->sig->tiempo > tiempo) actual = actual->sig;
            insertar->sig = actual->sig;
            if(actual->sig) {
                nodoPath* aux = insertar->sig;
                aux ->ant = insertar;
            }
            actual->sig = insertar;
            insertar->ant = actual;
        }
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
            d ->cantElementos--;
            nodoPath* auxAnt = pathActual->ant;
            nodoPath* auxSig = pathActual->sig;
            if (auxAnt) auxAnt->sig = auxSig;
            if (auxSig) auxSig->ant = auxAnt;
            if (domActual->primero == pathActual) domActual->primero = auxSig;
            pathActual->ant = NULL;
            pathActual->sig = NULL;
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

// Imprime los elementos de la lista por recursion.
void listarRec (nodoPath* p){
    if (p) {
        if (!p ->estaBorrado) cout << p -> path << " ";
        listarRec (p ->sig);
    }else cout << endl;

}
void listDomain (Tabla d, string dom){
    int posDom = fhashPrincipal (d -> tope, dom);
    int i = 1;
    while(d -> tablaDoms[posDom] && d -> tablaDoms[posDom] -> dominio != dom){
        posDom = fHashColisiones (d -> tope, posDom, i);
        i++;
    }

    nodoDominio* domActual = d -> tablaDoms [posDom];
    if (!domActual || domActual -> cantElementos == 0){
        cout << "sin_informacion" << endl;
    }else{
        listarRec(domActual -> primero);
    }
}

void CLEAR_DOMAIN(Tabla &d, string dom){
    int posDom = fhashPrincipal(d -> tope, dom);
    int i = 1;
    while(d ->tablaDoms[posDom] && d -> tablaDoms[posDom] ->dominio != dom){
        posDom = fHashColisiones(d -> tope, posDom, i);
        i++;
    }

    nodoDominio* domActual = d ->tablaDoms[posDom];

    if (domActual){
        nodoPath* actual = domActual-> primero;
        while(actual){
            nodoPath* aux = actual;
            actual = actual ->sig;
            delete aux;  
        }

        for (int i = 0; i < domActual->tope; i++) domActual->tablaPath[i] = NULL;
        domActual->primero = NULL;
        domActual->cantElementos = 0;
    }
}

int SIZE (Tabla d){
    return d ->cantElementos;
}

void CLEAR (Tabla &d){
    for (int i = 0; i < d->tope; i++) {
        nodoDominio* actual = d->tablaDoms[i];
        if (actual) {
            CLEAR_DOMAIN(d, actual->dominio);
            delete[] actual->tablaPath;   
            delete actual;                
            d->tablaDoms[i] = NULL;
        }
    }
    delete[] d->tablaDoms;  
    d->tablaDoms = NULL;
    d->cantElementos = 0;
    d->tope = 0;
}



int main()
{
    int cantAcciones;
    cin >> cantAcciones;
    Tabla datos = crear(cantAcciones);
    string accion;

    for (int i = 0; i < cantAcciones; i++){
        cin >> accion;
        if (accion == "PUT"){
            string dom, path, titulo;
            int tiempo;
            cin >> dom, path, titulo, tiempo;
            PUT (datos, dom, path, titulo, tiempo);
        }else if (accion == "GET"){
            
        }
    }
    return 0;
}