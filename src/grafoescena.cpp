#include "grafoescena.hpp"
#include "PilaMatrices.hpp"
#include "IDs_Shaders.hpp"

#include <glm/gtc/type_ptr.hpp>

// destructor
NodoGrafoEscena::~NodoGrafoEscena()
{
  Destruye();
}

// procesar nodo
void NodoGrafoEscena::Procesa()
{
   // procesar los hijos
   for( auto &hijo : listaHijos )
       hijo->Procesa();
}

// destruir nodo y sus hijos
void NodoGrafoEscena::Destruye()
{
   if (!listaHijos.empty())
   {
       for( auto &hijo : listaHijos )
          hijo->Destruye();

       listaHijos.clear();
   }

   delete this;
}

// añadir un hijo al nodo
void NodoGrafoEscena::aniadeHijo( NodoGrafoEscena* nodo )
{
  listaHijos.push_back(nodo);
}

NodoTerminal::NodoTerminal(figuras figura)
{
   this->figura = figura;
   this->malla = nullptr;
}

NodoTerminal::NodoTerminal(MallaTVT * malla)
{
   this->figura = NINGUNA;
   this->malla = malla;
}

NodoTerminal::~NodoTerminal()
{
   delete malla;
}

void NodoTerminal::Procesa()
{
   if (malla == nullptr)
   {
      if (figura == ICOSAEDRO)
         glutSolidIcosahedron();
      else if (figura == CILINDRO)
         glutSolidCylinder(0.5,2.0,20,10);
      else
         glutSolidDodecahedron();
   }
   else
      malla->Visualizar();

   NodoGrafoEscena::Procesa();
}

NodoTransformacionParametrizado::NodoTransformacionParametrizado(glm::mat4 * matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacionParametrizado::Procesa()
{
 //glPushMatrix();
 //glMultMatrixf( glm::value_ptr(*matrizTransformacion) );

   pila_opengl.push(pila_opengl.top());
   pila_opengl.top() *= *matrizTransformacion;

   GLint modelViewLocation = glGetUniformLocation(idProg_P3_P4,
                                                  "MVP");
   glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(pila_opengl.top()));

   NodoGrafoEscena::Procesa();

   pila_opengl.pop();

 //glPopMatrix();
}

NodoTransformacion::NodoTransformacion(glm::mat4 matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacion::Procesa()
{
 //glPushMatrix();
 //glMultMatrixf( glm::value_ptr(matrizTransformacion) );

   pila_opengl.push(pila_opengl.top());
   pila_opengl.top() *= matrizTransformacion;

   GLint modelViewLocation = glGetUniformLocation(idProg_P3_P4,
                                                  "MVP");
   glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(pila_opengl.top()));

   NodoGrafoEscena::Procesa();

   pila_opengl.pop();

 //glPopMatrix();
}

