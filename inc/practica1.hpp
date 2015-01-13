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

#include "MallaTVT.hpp"
#include "practica.hpp"
#include "grafoescena.hpp"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


using glm::vec3;
using glm::vec4;


class Practica1 : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();
   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);

   virtual ~Practica1();

   Practica1();
   Practica1(Practica1 const&);              // No se implementa para evitar copias
   Practica1& operator=(Practica1 const&);   // No se implementa para evitar asignaciones

private:

   MallaTVT * malla;

   vector<float> vertices_ply;
   vector<int> caras_ply;

   string file;



   NodoGrafoEscena * raiz;


   GLint IDModelado, IDVista, IDProyeccion;

   GLint IDvectorPosicionFuente;  // vector hacia la fuente de luz w = 0 direccional, w = 1 posicional
   GLint IDcolorFuente;
   GLint IDmaterial, IDexponenteEspecular;

   vec4 vectorPosicionLuz[2];
   vec3 colorFuente;

   vec3 colorEmision;
   vec3 colorAmbiental;
   vec3 colorDifuso;
   vec3 colorEspecular;
   float expontenteEspecular;

   vec3 material[4];


};

#endif
