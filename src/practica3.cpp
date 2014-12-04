// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 3  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "practica3.hpp"
#include "file_ply_stl.hpp"
#include "mallaTVT.hpp"
#include "Matriz.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Practica3::Practica3()
{
   modo_dibujo = ALAMBRE;
   raiz = NULL;
   semiesfera = NULL;
   cilindro = NULL;

   rotacion_cabeza = rotacion_brazos = rotacion_piernas = traslacion = NULL;
}

void Practica3::Inicializar( int argc, char *argv[] )
{

   /*
   vector<float> vertices_ply;
   vector<int> caras_ply;

   ply::read("PLY/big_atc.ply",vertices_ply,caras_ply);
   */

   //malla = new MallaTVT(vertices_ply,caras_ply);

   //raiz = new NodoGrafoEscena;

   /*float alpha = 2*M_PI/20;
   float inc = alpha;

   Matriz4x4 * matriz_traslacion = new Matriz4x4(Matriz4x4::Traslacion(0.5,1.0,5.0));
   Matriz4x4 * matriz_escalado   = new Matriz4x4(Matriz4x4::Escalado(1.5,1.5,1.5));
   Matriz4x4 * otra_matriz_traslacion = new Matriz4x4(Matriz4x4::Traslacion(5.5,1.0,5.0));

   NodoGrafoEscena * traslacion = new NodoTransformacion(matriz_traslacion);
   raiz->aniadeHijo(traslacion);

   NodoGrafoEscena * cilindro = new NodoTerminal(CILINDRO);
   NodoGrafoEscena * moto = new NodoTerminal(malla);
   NodoGrafoEscena * otramoto = new NodoTerminal(malla);
   NodoGrafoEscena * escalado = new NodoTransformacion(matriz_escalado);
   NodoGrafoEscena * otratraslacion = new NodoTransformacion(otra_matriz_traslacion);

   traslacion->aniadeHijo(cilindro);
   raiz->aniadeHijo(escalado);
   escalado->aniadeHijo(moto);
   raiz->aniadeHijo(otratraslacion);
   otratraslacion->aniadeHijo(otramoto);*/


   /* Creamos una semiesfera por revolución */

   vector<Tupla3f> vertices_semiesfera;

   vertices_semiesfera.push_back(Tupla3f(1.0,0.0,0.0));

   unsigned vertices_perfil_semiesfera = 20;
   float incremento_perfil_semiesfera = (M_PI/2)/vertices_perfil_semiesfera;

   for (unsigned i = 1; i <= vertices_perfil_semiesfera; i++)
      vertices_semiesfera.push_back(Matriz4x4::RotacionEjeZ(incremento_perfil_semiesfera)*vertices_semiesfera[i-1]);

   semiesfera = new MallaTVT(vertices_semiesfera);
   semiesfera = semiesfera->Revolucion(20);

   /* Creamos un cilindro por revolución */

   vector<Tupla3f> vertices_cilindro;

   vertices_cilindro.push_back(Tupla3f(1.0,0.0,0.0));

   unsigned vertices_perfil_cilindro = 20;
   float incremento_perfil_cilindro = 1.0/vertices_perfil_cilindro;

   for (unsigned i = 1; i <= vertices_perfil_cilindro; i++)
         vertices_cilindro.push_back(Matriz4x4::Traslacion(0.0,incremento_perfil_cilindro,0.0)*vertices_cilindro[i-1]);

   cilindro = new MallaTVT(vertices_cilindro);
   cilindro = cilindro->Revolucion(20);

   // -------------------------------------------------------------


   /* Creamos un brazo del Android */

   raiz = new NodoGrafoEscena;

   float proporcion_ancho_cuerpo = 2.0;
   float proporcion_alto_cuerpo = 3.0;
   float proporcion_alto_pierna = 1.2;
   float proporcion_alto_brazo = 1.2;
   float proporcion_ancho_pierna = 0.5;
   float proporcion_ancho_brazo = 0.5;
   float proporcion_altura_cabeza = proporcion_alto_cuerpo / 2;
   float proporcion_altura_brazo = proporcion_alto_cuerpo / 3;

   NodoGrafoEscena * pierna = new NodoGrafoEscena;
   NodoGrafoEscena * brazo = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_cilindro = new NodoTerminal(cilindro);
   NodoGrafoEscena * nodo_semiesfera = new NodoTerminal(semiesfera);

   NodoGrafoEscena * nodo_escalado_cilindro_pierna = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_pierna,proporcion_alto_pierna,proporcion_ancho_pierna));
   NodoGrafoEscena * nodo_escalado_cilindro_cuerpo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_cuerpo,proporcion_alto_cuerpo,proporcion_ancho_cuerpo));
   NodoGrafoEscena * nodo_escalado_cilindro_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_alto_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_semiesfera_cabeza = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_cuerpo,proporcion_altura_cabeza,proporcion_ancho_cuerpo));
   NodoGrafoEscena * nodo_escalado_semiesfera_superior_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_semiesfera_inferior_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_semiesfera_pierna = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_pierna,proporcion_ancho_pierna,proporcion_ancho_pierna));;

   NodoGrafoEscena * nodo_traslacion_pierna_izquierda = new NodoTransformacion(Matriz4x4::Traslacion(-(proporcion_ancho_cuerpo-1.3),-proporcion_alto_pierna,0.0));
   NodoGrafoEscena * nodo_traslacion_pierna_derecha = new NodoTransformacion(Matriz4x4::Traslacion(proporcion_ancho_cuerpo-1.3,-proporcion_alto_pierna,0.0));
   NodoGrafoEscena * nodo_traslacion_semiesfera_brazo = new NodoTransformacion(Matriz4x4::Traslacion(0.0,proporcion_alto_brazo,0.0)); // Esta no varía ya que el escalado se hará sobre todo el brazo
   NodoGrafoEscena * nodo_traslacion_brazo_izquierdo = new NodoTransformacion(Matriz4x4::Traslacion(-(proporcion_ancho_cuerpo+proporcion_ancho_brazo),proporcion_altura_brazo,0.0));
   NodoGrafoEscena * nodo_traslacion_brazo_derecho = new NodoTransformacion(Matriz4x4::Traslacion(proporcion_ancho_cuerpo+proporcion_ancho_brazo,proporcion_altura_brazo,0.0));

   NodoGrafoEscena * nodo_traslacion_cabeza = new NodoTransformacion(Matriz4x4::Traslacion(0.0,proporcion_alto_cuerpo + 0.15, 0.0));

   NodoGrafoEscena * nodo_rotacion_pi = new NodoTransformacion(Matriz4x4::RotacionEjeX(M_PI));

   /* Pierna del Android */

   pierna->aniadeHijo(nodo_escalado_cilindro_pierna);
      nodo_escalado_cilindro_pierna->aniadeHijo(nodo_cilindro);
   pierna->aniadeHijo(nodo_escalado_semiesfera_pierna);
      nodo_escalado_semiesfera_pierna->aniadeHijo(nodo_rotacion_pi);
         nodo_rotacion_pi->aniadeHijo(nodo_semiesfera);

   /* Brazo del Android */

   brazo->aniadeHijo(nodo_escalado_cilindro_brazo);
      nodo_escalado_cilindro_brazo->aniadeHijo(nodo_cilindro);

   brazo->aniadeHijo(nodo_traslacion_semiesfera_brazo);
      nodo_traslacion_semiesfera_brazo->aniadeHijo(nodo_escalado_semiesfera_superior_brazo);
         nodo_escalado_semiesfera_superior_brazo->aniadeHijo(nodo_semiesfera);
   brazo->aniadeHijo(nodo_escalado_semiesfera_inferior_brazo);
      nodo_escalado_semiesfera_inferior_brazo->aniadeHijo(nodo_rotacion_pi);  // Esta rotación ya tiene una semiesfera (la de la pierna)

   /* Todo el Android */

   // Cuerpo
   raiz->aniadeHijo(nodo_escalado_cilindro_cuerpo);
      nodo_escalado_cilindro_cuerpo->aniadeHijo(nodo_cilindro);

   // Pierna izquierda
   raiz->aniadeHijo(nodo_traslacion_pierna_izquierda);
      nodo_traslacion_pierna_izquierda->aniadeHijo(pierna);

   // Pierna derecha
   raiz->aniadeHijo(nodo_traslacion_pierna_derecha);
      nodo_traslacion_pierna_derecha->aniadeHijo(pierna);

   // Cabeza
   raiz->aniadeHijo(nodo_traslacion_cabeza);
      nodo_traslacion_cabeza->aniadeHijo(nodo_escalado_semiesfera_cabeza);
         nodo_escalado_semiesfera_cabeza->aniadeHijo(nodo_semiesfera);

   // Brazo izquierdo
   raiz->aniadeHijo(nodo_traslacion_brazo_izquierdo);
      nodo_traslacion_brazo_izquierdo->aniadeHijo(brazo);
   raiz->aniadeHijo(nodo_traslacion_brazo_derecho);
      nodo_traslacion_brazo_derecho->aniadeHijo(brazo);

   // Ojos



   // Antenas





}

// ---------------------------------------------------------------------
// función a implementar en la práctica 1  dibujar los objetos

void Practica3::DibujarObjetos()
{
   switch (modo_dibujo)
   {
      case ALAMBRE:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case SOLIDO:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      case PUNTOS:
         glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
         break;
      case AJEDREZ:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case SOLIDO_CARAS:
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         break;
      default:
         cout << "Enumerado inválido para modo_dibujo" << endl;
         exit(-3);
         break;

   }

   raiz->Procesa();
}

void Practica3::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;
   if (this->semiesfera != NULL)
      this->semiesfera->CambioModoDibujo(modo_dibujo);
   if (this->cilindro != NULL)
         this->cilindro->CambioModoDibujo(modo_dibujo);
}

void Practica3::CambioModoNormales()
{
   if (this->semiesfera != NULL)
      this->semiesfera->CambioModoNormales();
   if (this->cilindro != NULL)
         this->cilindro->CambioModoNormales();
}

void Practica3::CambioGradoLibertad(int grado_libertad)
{

   if (grado_libertad != 1 && grado_libertad != -1
         && grado_libertad != 2 && grado_libertad != -2
         && grado_libertad != 3 && grado_libertad != -3
         && grado_libertad != 4 && grado_libertad != -4)
   {
      exit(-4);
   }

   float vel = 0.5 * grado_libertad;

   parametros_traslacion = parametros_traslacion+vel;
   *traslacion = Matriz4x4::Traslacion(parametros_traslacion[0],parametros_traslacion[1],parametros_traslacion[1]);



}
