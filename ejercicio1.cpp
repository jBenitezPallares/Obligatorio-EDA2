#include <cassert>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

struct NodoAVL
{
    int id;
    int puntaje;
    string nombre;
    NodoAVL *izq;
    NodoAVL *der;
    int fVal;

    NodoAVL(int id, int puntaje, string nombre): id(id), puntaje(puntaje), nombre(nombre),izq(NULL), der(NULL), fVal(0) {}
};

struct RepresentacionAVL
{
  NodoAVL *raiz;
  int cantElementos;
  bool (*comparador)(NodoAVL*, NodoAVL*);

  RepresentacionAVL(bool (*comparador) (NodoAVL*, NodoAVL*)): raiz(NULL), cantElementos(0), comparador(comparador){}
};
typedef RepresentacionAVL *AVL;

// inserta el nodo en el AVL utilizando recursion
bool insertarAVLRec(NodoAVL *&pa, int id, int puntaje, string nombre, bool &varioAltura, bool (*comparador)(NodoAVL*, NodoAVL*))
{
    if (pa == NULL)
    {
        pa = new NodoAVL(id, puntaje, nombre);
        varioAltura = true;
        return true;
    }
    if (id == pa->id) {
        varioAltura = false;
        return false;
    }
    NodoAVL nuevo(id, puntaje, nombre);
    bool comparacion = comparador (&nuevo, pa);
    bool insertado;
    if (comparacion)
    {
        insertado = insertarAVLRec(pa -> izq, id, puntaje, nombre, varioAltura, comparador);
        if (varioAltura)
        {
            switch (pa->fVal)
            {
                case 1:
                    pa->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    pa->fVal = -1;
                    break;
                case -1:
                    NodoAVL *p1 = pa->izq;
                    if(p1->fVal == -1){
                        pa->izq = p1->der;
                        p1->der = pa;
                        pa->fVal = 0;
                        pa = p1;
                    } else {
                        NodoAVL *p2 = p1->der;
                        p1->der = p2->izq;
                        p2->izq = p1;
                        pa->izq = p2->der;
                        p2->der = pa;
                        pa->fVal = p2->fVal == -1 ? 1 : 0;
                        p1->fVal = p2->fVal == 1 ? -1 : 0;
                        pa = p2;
                    }
                    pa->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return insertado;
    }
    else 
    {
        insertado = insertarAVLRec(pa->der, id, puntaje, nombre, varioAltura, comparador);
        if (varioAltura)
        {
            switch (pa->fVal)
            {
                case -1:
                    pa->fVal = 0;
                    varioAltura = false;
                    break;
                case 0:
                    pa->fVal = 1;
                    break;
                case 1:
                    NodoAVL *p1 = pa->der;
                    if(p1->fVal == 1){
                        pa->der = p1->izq;
                        p1->izq = pa;
                        pa->fVal = 0;
                        pa = p1;
                    } else {
                        NodoAVL *p2 = p1->izq;
                        p1->izq = p2->der;
                        p2->der = p1;
                        pa->der = p2->izq;
                        p2->izq = pa;
                        pa->fVal = p2->fVal == 1 ? -1 : 0;
                        p1->fVal = p2->fVal == -1 ? 1 : 0;
                        pa = p2;
                        return insertado;
                    }
                    pa->fVal = 0;
                    varioAltura = false;
                    break;
            }
            varioAltura = false;
        }
        return insertado;
    } 
}

// Metodo principal para insertar en el AVL
void ADD(AVL a, int id, int puntaje, string nombre)
{
    bool varioAltura = false;
    bool insertado = insertarAVLRec(a->raiz, id, puntaje, nombre, varioAltura, a->comparador);
    if (insertado)
    {
        a->cantElementos++;
    }
}

// Busca el nodo en el AVL utilizando recursion
NodoAVL* findRecursivo (NodoAVL* a, int id){
    if (!a) return NULL;
    else{
        if(a->id == id) return a;
        else if(a-> id < id) return findRecursivo(a -> der, id);
        else return findRecursivo(a->izq, id);
    }
}

NodoAVL* FIND (AVL a, int id){
    return findRecursivo(a ->raiz, id);
}

NodoAVL* TOP1 (AVL a){
    return a->raiz;
}

int COUNT (AVL a){
    return a->cantElementos;
}

bool compararId(NodoAVL* nuevo, NodoAVL* actual){
    return nuevo->id > actual->id;
}

bool compararPuntaje(NodoAVL* nuevo, NodoAVL* actual){
    if (nuevo->puntaje == actual->puntaje) return compararId (nuevo, actual);
    else return nuevo->puntaje > actual->puntaje;
}

int main()
{
    return 0;
}