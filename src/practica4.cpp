// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** 
// ** Práctica 4  (implementación)
// **
// *********************************************************************

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "practica4.hpp"

#include "MallaTVT.hpp"
#include "file_ply_stl.hpp"
#include "Matriz.hpp"
#include "FuenteLuz.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

template <class T>
inline T signo (T valor)
{
   if (valor > 0)
      return 1;
   else if (valor < 0)
      return -1;
   else
      return 0;
}

Practica4::Practica4()
{
   modo_dibujo = ALAMBRE;

   fuente_posicional = nullptr;
   fuente_direccional = nullptr;
   peon_madera = peon_blanco = peon_negro = cuerpo_lata = tapa_sup = tapa_inf = nullptr;
   raiz = nullptr;
   material_peon_madera = material_peon_blanco = material_peon_negro =
         material_cuerpo_lata = material_tapa = nullptr;

}

Practica4::~Practica4()
{

   delete raiz;
   delete fuente_posicional;
   delete fuente_direccional;
   delete peon_madera;
   delete peon_blanco;
   delete peon_negro;
   delete cuerpo_lata;
   delete tapa_sup;
   delete tapa_inf;
   delete material_peon_madera;
   delete material_peon_blanco;
   delete material_peon_negro;
   delete material_cuerpo_lata;
   delete material_tapa;

}

void Practica4::Inicializar( int argc, char *argv[] )
{
   unsigned caras_revolucion = 100;

   // Luces, cámara y acción!

   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

   float alpha = 3 * M_PI / 4;
   float beta = M_PI / 6;
   Tupla3f luz_posicional_componente_ambiental(0.0,0.0,1.0);
   Tupla3f luz_posicional_componente_difusa(0.0,0.0,1.0);
   Tupla3f luz_posicional_componente_especular(0.0,0.0,1.0);
   Tupla3f luz_direccional_componente_ambiental(0.0,1.0,0.0);
   Tupla3f luz_direccional_componente_difusa(0.0,1.0,0.0);
   Tupla3f luz_direccional_componente_especular(0.0,1.0,0.0);
   Tupla3f luz_posicion(0.0,20.0,0.0);

   fuente_direccional = new FuenteLuzDireccional(alpha, beta, luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);
   fuente_posicional = new FuenteLuzPosicional(luz_posicion, luz_posicional_componente_ambiental, luz_posicional_componente_difusa, luz_posicional_componente_especular);
   fuentes.Agregar(fuente_direccional);
   fuentes.Agregar(fuente_posicional);

   // Cargar plys
   vector<GLfloat> vertices_ply_peon, vertices_ply_cuerpo_lata, vertices_ply_tapa_sup, vertices_ply_tapa_inf;
   ply::read_vertices("PLY/peon.ply", vertices_ply_peon);
   ply::read_vertices("PLY/lata-pcue.ply",vertices_ply_cuerpo_lata);
   ply::read_vertices("PLY/lata-psup.ply",vertices_ply_tapa_sup);
   ply::read_vertices("PLY/lata-pinf.ply",vertices_ply_tapa_inf);

   // Crear material del peon blanco (sin textura, material puramente difuso sin brillo especular)
   Tupla3f peon_blanco_componente_emision(0.8,0.8,0.8);  // Color blanco
   Tupla3f peon_blanco_componente_ambiental(0.0,0.0,0.0);
   Tupla3f peon_blanco_componente_difusa(0.4,0.4,0.4);
   Tupla3f peon_blanco_componente_especular(0.0,0.0,0.0);
   float peon_blanco_exponente_especular = 0.0;

   material_peon_blanco = new Material(peon_blanco_componente_emision, peon_blanco_componente_ambiental,
         peon_blanco_componente_difusa, peon_blanco_componente_especular, peon_blanco_exponente_especular);

   // Crear material del peón negro (sin textura, material especular sin apenas reflectividad difusa)
   Tupla3f peon_negro_componente_emision(0.0,0.0,0.0);  // Color negro
   Tupla3f peon_negro_componente_ambiental(0.0,0.0,0.0);
   Tupla3f peon_negro_componente_difusa(0.1,0.1,0.1);
   Tupla3f peon_negro_componente_especular(1.0,1.0,1.0);
   float peon_negro_exponente_especular = 30.0;

   material_peon_negro = new Material(peon_negro_componente_emision, peon_negro_componente_ambiental,
         peon_negro_componente_difusa, peon_negro_componente_especular, peon_negro_exponente_especular);

   // Crear material del peón de madera (con textura generada automáticamente, material difuso-especular)
   Tupla3f peon_madera_componente_emision(1.0,1.0,1.0);  // Color blanco puro
   Tupla3f peon_madera_componente_ambiental(0.0,0.0,0.0);
   Tupla3f peon_madera_componente_difusa(0.1,0.1,0.1);
   Tupla3f peon_madera_componente_especular(1.0,1.0,1.0);
   float peon_madera_exponente_especular = 30.0;

   float cs[4] = {1.0,0.0,0.0,0.0}; // Vector ex
   float ct[4] = {0.0,1.0,0.0,0.0}; // Vector ey

   Textura * textura_peon_madera = new Textura("img/text-madera.jpg",1,cs,ct);

   material_peon_madera = new Material(peon_madera_componente_emision, peon_madera_componente_ambiental,
         peon_madera_componente_difusa, peon_madera_componente_especular, peon_madera_exponente_especular,textura_peon_madera);

   // Crear peones

   peon_blanco = new MallaTVT(PERFIL,vertices_ply_peon);
   peon_blanco->Revolucion(caras_revolucion);
   peon_blanco->SetMaterial(material_peon_blanco);

   /*
   peon_negro = new MallaTVT(peon_blanco);
   //peon_negro = peon_negro->Revolucion(20);
   peon_negro->SetMaterial(material_peon_negro);

   peon_madera = new MallaTVT(peon_blanco);
   //peon_madera = peon_madera->Revolucion(20);
   peon_madera->SetMaterial(material_peon_madera);
   */

   peon_negro = new MallaTVT(PERFIL,vertices_ply_peon);
   peon_negro->Revolucion(20);
   peon_negro->SetMaterial(material_peon_negro);

   peon_madera = new MallaTVT(PERFIL,vertices_ply_peon);
   peon_madera->Revolucion(20);
   peon_madera->SetMaterial(material_peon_madera);

   // Crear lata
   Tupla3f cuerpo_lata_componente_emision(1.0,1.0,1.0);  // Color blanco puro
   Tupla3f cuerpo_lata_componente_ambiental(0.0,0.0,0.0);
   Tupla3f cuerpo_lata_componente_difusa(0.1,0.1,0.1);
   Tupla3f cuerpo_lata_componente_especular(1.0,1.0,1.0);
   float cuerpo_lata_exponente_especular = 30.0;

   Textura * textura_cuerpo_lata = new Textura("img/text-lata-1.jpg",0,cs,ct);

   material_cuerpo_lata = new Material(cuerpo_lata_componente_emision, cuerpo_lata_componente_ambiental,
         cuerpo_lata_componente_difusa, cuerpo_lata_componente_especular, cuerpo_lata_exponente_especular,textura_cuerpo_lata);

   cuerpo_lata = new MallaTVT(PERFIL,vertices_ply_cuerpo_lata);
   cuerpo_lata->SetMaterial(material_cuerpo_lata);
   cuerpo_lata->Revolucion(caras_revolucion, false);  // Para calcular las coordenadas de textura hay que tener ya el material

   // Crear tapas
   Tupla3f tapa_componente_emision(0.6,0.6,0.6);  // Color gris
   Tupla3f tapa_componente_ambiental(0.0,0.0,0.0);
   Tupla3f tapa_componente_difusa(0.0,0.0,0.0);
   Tupla3f tapa_componente_especular(1.0,1.0,1.0);
   float tapa_exponente_especular = 15.0;

   material_tapa = new Material(tapa_componente_emision, tapa_componente_ambiental,
            tapa_componente_difusa, tapa_componente_especular, tapa_exponente_especular);

   tapa_sup = new MallaTVT(PERFIL,vertices_ply_tapa_sup);
   tapa_sup->SetMaterial(material_tapa);
   tapa_sup->Revolucion(caras_revolucion,false);
   tapa_inf = new MallaTVT(PERFIL,vertices_ply_tapa_inf);
   tapa_inf->SetMaterial(material_tapa);
   tapa_inf->Revolucion(caras_revolucion,false);




   raiz = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_traslacion_peon_madera = new NodoTransformacion(Matriz4x4::Traslacion(-3.0,1.3,1.5));
   NodoGrafoEscena * nodo_traslacion_peon_blanco = new NodoTransformacion(Matriz4x4::Traslacion(0.0,1.3,2.5));
   NodoGrafoEscena * nodo_traslacion_peon_negro = new NodoTransformacion(Matriz4x4::Traslacion(3.0,1.3,3.3));
   NodoGrafoEscena * nodo_escalado_lata = new NodoTransformacion(Matriz4x4::Escalado(4.0,4.0,4.0));
   NodoGrafoEscena * nodo_peon_blanco = new NodoTerminal(peon_blanco);
   NodoGrafoEscena * nodo_peon_negro = new NodoTerminal(peon_negro);
   NodoGrafoEscena * nodo_peon_madera = new NodoTerminal(peon_madera);
   NodoGrafoEscena * nodo_cuerpo_lata = new NodoTerminal(cuerpo_lata);
   NodoGrafoEscena * nodo_tapa_sup = new NodoTerminal(tapa_sup);
   NodoGrafoEscena * nodo_tapa_inf = new NodoTerminal(tapa_inf);


   raiz->aniadeHijo(nodo_traslacion_peon_blanco);
      nodo_traslacion_peon_blanco->aniadeHijo(nodo_peon_blanco);

   raiz->aniadeHijo(nodo_traslacion_peon_negro);
      nodo_traslacion_peon_negro->aniadeHijo(nodo_peon_negro);


   raiz->aniadeHijo(nodo_traslacion_peon_madera);
      nodo_traslacion_peon_madera->aniadeHijo(nodo_peon_madera);


   raiz->aniadeHijo(nodo_escalado_lata);
      nodo_escalado_lata->aniadeHijo(nodo_cuerpo_lata);
      nodo_escalado_lata->aniadeHijo(nodo_tapa_sup);
      nodo_escalado_lata->aniadeHijo(nodo_tapa_inf);


}


void Practica4::DibujarObjetos()
{
   glEnable( GL_LIGHTING );
   glEnable( GL_NORMALIZE );
   glDisable( GL_COLOR_MATERIAL );

   // Dibujar aquí
   fuentes.Activar();
   raiz->Procesa();
   fuentes.Desactivar();

   glDisable( GL_LIGHTING );
   glDisable( GL_NORMALIZE );
   glEnable( GL_COLOR_MATERIAL );

}

void Practica4::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;
}

void Practica4::CambioModoNormales()
{
   this->cuerpo_lata->CambioModoNormales();
   this->tapa_inf->CambioModoNormales();
   this->tapa_sup->CambioModoNormales();
   this->peon_madera->CambioModoNormales();
   this->peon_blanco->CambioModoNormales();
   this->peon_negro->CambioModoNormales();
}


void Practica4::CambioGradoLibertad(int grado_libertad)
{

   if (grado_libertad != 1 && grado_libertad != -1
         && grado_libertad != 2 && grado_libertad != -2
         && grado_libertad != 3 && grado_libertad != -3
         && grado_libertad != 4 && grado_libertad != -4)
   {
      cout << "Grado de libertad inválido" << endl;
      exit(-4);
   }



}

void Practica4::CambioColorFijo()
{

}

bool Practica4::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   switch(toupper(tecla))
   {
      // Control de luz direccional
      case 'Z':
         fuente_direccional->ModificaBeta(1);
         break;
      case 'X':
         fuente_direccional->ModificaBeta(-1);
         break;
      case 'C':
         fuente_direccional->ModificaAlpha(1);
         break;
      case 'V':
         fuente_direccional->ModificaAlpha(-1);
         break;
      default:
         redisp = false;
         break;
   }
   return redisp;
}


void Practica4::Debug()
{

   vector<string> debug_strings;

   //debug_strings.push_back(string("Beta: " + to_string(fuente_direccional->getBeta())));
   //debug_strings.push_back(string("Alpha: " + to_string(fuente_direccional->getAlpha())));

   debug_strings.push_back(string("Modo de normales: " + enumToString(peon_madera->getModoNormales())));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(modo_dibujo)));
   debug_strings.push_back(string("Practica 4"));

   void * font = GLUT_BITMAP_9_BY_15;

   unsigned num_lineas = 0;
   for (auto &s: debug_strings)
   {
      glRasterPos2i(10, 10+15*num_lineas);
      for (auto &c: s)
      {
        glutBitmapCharacter(font, c);
      }
      num_lineas++;
   }
}

void Practica4::Ayuda(vector<string> & strings_control)
{

   strings_control.push_back("Z/X para modificar beta (angulo con eje -X)");
   strings_control.push_back("C/V para modificar alpha (angulo con eje Y)");


}
