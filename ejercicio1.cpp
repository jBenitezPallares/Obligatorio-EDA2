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

NodoAVL* top1 (AVL a){
    return a->raiz;
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