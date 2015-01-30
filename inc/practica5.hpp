// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// **
// **
// ** Práctica 5  (declaraciones públicas)
// **
// *********************************************************************

#ifndef __IG_PRACTICA5_HPP__
#define __IG_PRACTICA5_HPP__

#include "practica.hpp"
#include "tuplas.hpp"
#include "grafoescena.hpp"
#include "MallaTVT.hpp"

class Practica5 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void EnciendeLuces();
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();

   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);
   void ClickRaton(int x, int y);
   void Debug();
   void Ayuda(vector<string> & strings_control);

   void FijarCamara();
   void FijarProyeccion(float ventana_tam_x, float ventana_tam_y);
   void ModificaEjeXCamara(float nuevo);
   void ModificaEjeYCamara(float nuevo);
   void ModificarEscala(int signo);

   virtual ~Practica5();

   Practica5();
   Practica5(Practica5 const&);              // No se implementa para evitar copias
   Practica5& operator=(Practica5 const&);   // No se implementa para evitar asignaciones

private:
   visualizacion modo_dibujo;

   vector<MallaTVT *> mallas;

   Tupla3ub color_moto, color_peon, color_donut;
   MallaTVT * moto, * peon, * donut, * malla_marcada;
   NodoGrafoEscena * raiz;

   static constexpr unsigned num_camaras = 3;

   Camara * camaras[num_camaras];
   Camara * camaraActual;


};

#endif
