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
#include "tuplas.hpp"
#include "grafoescena.hpp"

class Practica3 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();
   void CambioGradoLibertad(int grado_libertad);

   virtual ~Practica3() {};

   Practica3();
   Practica3(Practica3 const&);              // No se implementa para evitar copias
   Practica3& operator=(Practica3 const&);   // No se implementa para evitar asignaciones

private:
   visualizacion modo_dibujo;
   NodoGrafoEscena * raiz;
   MallaTVT * semiesfera, * cilindro;

   Matriz4x4 * rotacion_brazos, * rotacion_piernas, * rotacion_cabeza, * traslacion;
   Tupla3f parametros_rotacion_brazos, parametros_rotacion_piernas, parametros_rotacion_cabeza, parametros_traslacion;

};

#endif
