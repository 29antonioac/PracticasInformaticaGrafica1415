// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// **
// ** Práctica 3  (declaraciones públicas)
// **
// *********************************************************************

#ifndef __IG_PRACTICA4_HPP__
#define __IG_PRACTICA4_HPP__

#include "practica.hpp"
#include "tuplas.hpp"
#include "grafoescena.hpp"
#include "FuenteLuz.hpp"
#include "Material.hpp"

class Practica4 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();
   void CambioGradoLibertad(int grado_libertad);

   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);
   void Debug();
   void Ayuda(vector<string> & strings_control);

   virtual ~Practica4();

   Practica4();
   Practica4(Practica4 const&);              // No se implementa para evitar copias
   Practica4& operator=(Practica4 const&);   // No se implementa para evitar asignaciones

private:
   visualizacion modo_dibujo;


   FuenteLuzPosicional * fuente1, * fuente2;
   ColeccionFuentesLuz fuentes;

};

#endif
