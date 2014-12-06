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
   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);

   virtual ~Practica3() {};

   Practica3();
   Practica3(Practica3 const&);              // No se implementa para evitar copias
   Practica3& operator=(Practica3 const&);   // No se implementa para evitar asignaciones

private:
   visualizacion modo_dibujo;
   NodoGrafoEscena * raiz;
   MallaTVT * semiesfera, * cilindro;

   Matriz4x4 * rotacion_brazo_izquierdo, * rotacion_brazo_derecho, * rotacion_pierna_izquierda, * rotacion_pierna_derecha,
      * rotacion_cuerpo, * traslacion;
   Tupla3f parametros_rotacion_brazo_izquierdo, parametros_rotacion_brazo_derecho,
      parametros_rotacion_pierna_izquierda, parametros_rotacion_pierna_derecha, parametros_traslacion;

   float angulo_rotacion_cuerpo, angulo_rotacion_brazos, angulo_rotacion_piernas;
   float velocidad_angular_brazos, velocidad_angular_piernas;

};

#endif
