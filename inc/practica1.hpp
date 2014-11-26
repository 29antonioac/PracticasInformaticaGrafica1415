// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 1  (declaraciones públicas)
// **
// *********************************************************************

#ifndef __IG_PRACTICA1_HPP__
#define __IG_PRACTICA1_HPP__

#include <vector>
#include "visualizacion.hpp"
#include "mallaTVT.hpp"
#include "practica.hpp"

class Practica1 : virtual public Practica
{
public:
   /*static Practica1 getInstance()
   {
      static Practica1 instance;
      return instance;
   }*/
   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();

   virtual ~Practica1() {};
//protected:
   Practica1() {};

   Practica1(Practica1 const&);
   //void operator=(Practica1 const&);

private:

   MallaTVT * malla;

   vector<float> vertices_ply;
   vector<int> caras_ply;
};

#endif
