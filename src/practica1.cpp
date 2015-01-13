// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 1  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "practica1.hpp"
#include "file_ply_stl.hpp"
#include "PilaMatrices.hpp"
#include "IDs_Shaders.hpp"
#include "grafoescena.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Practica1::Practica1()
{
   malla = nullptr;
}

Practica1::~Practica1()
{
   delete malla;
}

void Practica1::Inicializar( int argc, char *argv[] )
{
   if (argc == 1)
   {
      file = "./PLY/ant.ply";
   }
   else if (argc == 5)
   {
      file = string(argv[1]);
   }
   else
   {
      cout << "Argumentos mal (argc = " << argc << ")" << endl;
      exit(-1);
   }

   cout << "Archivo: " << file << endl;

   ply::read(file.c_str(),vertices_ply,caras_ply);
   malla = new MallaTVT(MALLA,vertices_ply,caras_ply);

   cout << "-----------------------------------" << endl;


   raiz = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_shader = new NodoShader(idProg_P1_P2);
   NodoGrafoEscena * nodo_malla = new NodoTerminal(malla);
   NodoGrafoEscena * nodo_transformacion = new NodoTransformacion(scale(glm::mat4(),glm::vec3(1.0,1.0,1.0)));

   raiz->aniadeHijo(nodo_shader);
      nodo_shader->aniadeHijo(nodo_transformacion);
         nodo_transformacion->aniadeHijo(nodo_malla);


}

void Practica1::DibujarObjetos()
{
   raiz->Procesa();
}

void Practica1::CambioModoDibujo(visualizacion modo_dibujo)
{
   malla->CambioModoDibujo(modo_dibujo);
}

void Practica1::CambioModoNormales()
{
   malla->CambioModoNormales();
}

void Practica1::CambioColorFijo()
{
   malla->CambioColorFijo();
}

bool Practica1::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;
   switch(toupper(tecla))
   {
   // Dejamos implementado el switch por si añadimos alguna tecla a esta práctica en particular
      default:
         redisp = false;
         break;
   }
   return redisp;
}
