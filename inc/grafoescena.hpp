#ifndef GRAFO_ESCENA
#define GRAFO_ESCENA

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <vector>
#include <iostream>

#include "MallaTVT.hpp"
#include <glm/mat4x4.hpp>

using std::vector;
using glm::mat4;

class NodoGrafoEscena
{
public:
  NodoGrafoEscena() {}
  virtual ~NodoGrafoEscena();

  // actualiza el nodo
  virtual void Procesa();

  // destruye el nodo y sus hijos
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
  NodoTerminal(MallaTVT * malla);
  ~NodoTerminal();

  void Procesa();

private:
  figuras figura;
  MallaTVT * malla;
};

class NodoTransformacionParametrizado: public NodoGrafoEscena
{
public:
   NodoTransformacionParametrizado(mat4 * matriz);
  ~NodoTransformacionParametrizado() {}

  void Procesa();

private:
  mat4 * matrizTransformacion;
};

class NodoTransformacion: public NodoGrafoEscena
{
public:
   NodoTransformacion(mat4 matriz);
  ~NodoTransformacion() {}

  void Procesa();

private:
  mat4 matrizTransformacion;
};

class NodoShader : public NodoGrafoEscena
{
public:
   NodoShader(GLuint idProgShader);
   ~NodoShader() {}

   void Procesa();

private:
   GLuint idProg;
};


#endif
