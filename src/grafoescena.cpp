#include "grafoescena.hpp"

// destructor
NodoGrafoEscena::~NodoGrafoEscena()
{
  Destruye();
}

// liberar de memoria
void NodoGrafoEscena::Libera()
{
  delete this;
}

// actualizar nodo
void NodoGrafoEscena::Procesa()
{
 // actualizar los hijos
 for( unsigned i = 0; i < listaHijos.size(); i++ )
 {
   listaHijos[i]->Procesa();
 }
}

// destruir hijos
void NodoGrafoEscena::Destruye()
{
 for( unsigned i = 0; i < listaHijos.size(); i++ )
    listaHijos[i]->Libera();

 listaHijos.clear();
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

NodoTransformacionParametrizado::NodoTransformacionParametrizado(Matriz4x4 * matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacionParametrizado::Procesa()
{
 glPushMatrix();
 glMultMatrixf( matrizTransformacion->data() );

 NodoGrafoEscena::Procesa();

 glPopMatrix();
}

NodoTransformacion::NodoTransformacion(Matriz4x4 matriz)
{
   this->matrizTransformacion = matriz;
}

void NodoTransformacion::Procesa()
{
 glPushMatrix();
 glMultMatrixf( matrizTransformacion.data() );

 NodoGrafoEscena::Procesa();

 glPopMatrix();
}

