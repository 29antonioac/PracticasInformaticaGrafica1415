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


   }
   listaHijos.clear();

   //delete this;
}

// añadir un hijo al nodo
void NodoGrafoEscena::aniadeHijo( NodoGrafoEscena* nodo )
{
  listaHijos.push_back(nodo);
}

NodoTerminal::NodoTerminal(MallaTVT * malla)
{
   this->malla = malla;
}

NodoTerminal::~NodoTerminal()
{
   delete malla;
}

void NodoTerminal::Procesa()
{
   malla->Visualizar();

   NodoGrafoEscena::Procesa();
}

NodoTransformacionParametrizado::NodoTransformacionParametrizado(mat4 * matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacionParametrizado::Procesa()
{
 //glPushMatrix();
 //glMultMatrixf( glm::value_ptr(*matrizTransformacion) );

   pila_opengl.push(pila_opengl.top());
   pila_opengl.top() *= *matrizTransformacion;

   GLint modelViewLocation = glGetUniformLocation(idProg_actual,
                                                  "MVP");
   glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, value_ptr(pila_opengl.top()));

   NodoGrafoEscena::Procesa();

   pila_opengl.pop();

 //glPopMatrix();
}

NodoTransformacion::NodoTransformacion(mat4 matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacion::Procesa()
{
 //glPushMatrix();
 //glMultMatrixf( glm::value_ptr(matrizTransformacion) );

   pila_opengl.push(pila_opengl.top());
   pila_opengl.top() *= matrizTransformacion;

   GLint IDModelado = glGetUniformLocation(idProg_actual,"Modelado");
   GLint IDVista = glGetUniformLocation(idProg_actual, "Vista");
   GLint IDProyeccion = glGetUniformLocation(idProg_actual, "Proyeccion");

   glUniformMatrix4fv(IDModelado, 1, GL_FALSE, value_ptr(pila_opengl.top()));
   glUniformMatrix4fv(IDVista, 1, GL_FALSE, value_ptr(Vista));
   glUniformMatrix4fv(IDProyeccion, 1, GL_FALSE, value_ptr(Proyeccion));

   NodoGrafoEscena::Procesa();

   pila_opengl.pop();

 //glPopMatrix();
}

NodoShader::NodoShader(GLuint idProgShader)
{
   idProg = idProgShader;
}

void NodoShader::Procesa()
{
   UsarPrograma(idProg);
   NodoGrafoEscena::Procesa();
}

