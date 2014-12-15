#ifndef GRAFO_ESCENA
#define GRAFO_ESCENA

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <vector>
#include <iostream>

#include "MallaTVT.hpp"
#include "Matriz.hpp"

using std::vector;

class NodoGrafoEscena
{
public:
  NodoGrafoEscena() {}
  virtual ~NodoGrafoEscena();

  // libera objeto de memoria
  void Libera();

  // actualiza el nodo
  virtual void Procesa();

  // destruye los hijos
  void Destruye();

  // añade un hijo al nodo
  void aniadeHijo( NodoGrafoEscena* nodo);

protected:
  // Lista de hijos
  vector<NodoGrafoEscena*> listaHijos;

};

class NodoTerminal: public NodoGrafoEscena
{
public:
  NodoTerminal(figuras figura);
  NodoTerminal(MallaTVT * malla);
  ~NodoTerminal() {}

  void Procesa();

private:
  figuras figura;
  MallaTVT * malla;
};

class NodoTransformacionParametrizado: public NodoGrafoEscena
{
public:
   NodoTransformacionParametrizado(Matriz4x4 * matriz);
  ~NodoTransformacionParametrizado() {}

  void Procesa();

private:
  Matriz4x4 * matrizTransformacion;
};

class NodoTransformacion: public NodoGrafoEscena
{
public:
   NodoTransformacion(Matriz4x4 matriz);
  ~NodoTransformacion() {}

  void Procesa();

private:
  Matriz4x4 matrizTransformacion;
};


#endif
