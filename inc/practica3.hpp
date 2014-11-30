// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 3  (declaraciones públicas)
// **
// *********************************************************************

#ifndef __IG_PRACTICA3_HPP__
#define __IG_PRACTICA3_HPP__

#include "practica.hpp"
#include "grafoescena.hpp"

class Practica3 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();

   virtual ~Practica3() {};

   Practica3() { modo_dibujo = ALAMBRE; };
   Practica3(Practica3 const&);

private:
   visualizacion modo_dibujo;
   NodoGrafoEscena * raiz;
   MallaTVT * malla;

};

#endif
