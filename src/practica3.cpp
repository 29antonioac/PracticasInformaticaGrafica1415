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

Practica3::Practica3()
{
   modo_dibujo = ALAMBRE;
   raiz = NULL;
   semiesfera = NULL;
   cilindro = NULL;

   angulo_rotacion_cuerpo = M_PI;
   angulo_rotacion_brazos = M_PI;
   angulo_rotacion_piernas = M_PI;

   distancia_eje_Y = velocidad_angular_cuerpo = velocidad_angular_brazos = velocidad_angular_piernas = 0;

   rotacion_cuerpo = rotacion_brazo_izquierdo = rotacion_brazo_derecho = rotacion_pierna_izquierda = rotacion_pierna_derecha = traslacion = NULL;
}

Practica3::~Practica3()
{
   delete raiz, semiesfera, cilindro, rotacion_cuerpo,
      rotacion_brazo_izquierdo, rotacion_brazo_derecho, rotacion_pierna_izquierda, rotacion_pierna_derecha, traslacion;
}

void Practica3::Inicializar( int argc, char *argv[] )
{


   // ----------------- Creamos una semiesfera por revolución -----------------

   vector<Tupla3f> vertices_semiesfera;

   vertices_semiesfera.push_back(Tupla3f(1.0,0.0,0.0));

   unsigned vertices_perfil_semiesfera = 20;
   float incremento_perfil_semiesfera = (M_PI/2)/vertices_perfil_semiesfera;

   for (unsigned i = 1; i <= vertices_perfil_semiesfera; i++)
      vertices_semiesfera.push_back(Matriz4x4::RotacionEjeZ(incremento_perfil_semiesfera)*vertices_semiesfera[i-1]);

   semiesfera = new MallaTVT(vertices_semiesfera);
   semiesfera = semiesfera->Revolucion(20);

   // ----------------- Creamos un cilindro por revolución -----------------

   vector<Tupla3f> vertices_cilindro;

   vertices_cilindro.push_back(Tupla3f(1.0,0.0,0.0));

   unsigned vertices_perfil_cilindro = 20;
   float incremento_perfil_cilindro = 1.0/vertices_perfil_cilindro;

   for (unsigned i = 1; i <= vertices_perfil_cilindro; i++)
         vertices_cilindro.push_back(Matriz4x4::Traslacion(0.0,incremento_perfil_cilindro,0.0)*vertices_cilindro[i-1]);

   cilindro = new MallaTVT(vertices_cilindro);
   cilindro = cilindro->Revolucion(20);

   // ------------------------------------------------------------------------


   // Creamos un brazo del Android

   raiz = new NodoGrafoEscena;

   float proporcion_alto_cuerpo = 3.0;
   float proporcion_alto_pierna = 1.2;
   float proporcion_alto_brazo = 1.2;
   float proporcion_alto_antena = 1.0;
   float proporcion_alto_cabeza = proporcion_alto_cuerpo / 2;
   float proporcion_ancho_cuerpo = 2.0;
   float proporcion_ancho_pierna = 0.5;
   float proporcion_ancho_brazo = 0.5;
   float proporcion_ancho_antena = 0.1;
   float proporcion_ancho_alto_ojo = 0.3;

   float separacion_cuerpo_cabeza = 0.15;

   float proporcion_altura_brazo = proporcion_alto_cuerpo * 0.75;

   NodoGrafoEscena * pierna = new NodoGrafoEscena;
   NodoGrafoEscena * brazo = new NodoGrafoEscena;
   NodoGrafoEscena * cabeza = new NodoGrafoEscena;
   NodoGrafoEscena * Android = new NodoGrafoEscena;
   NodoGrafoEscena * nodo_cilindro = new NodoTerminal(cilindro);
   NodoGrafoEscena * nodo_semiesfera = new NodoTerminal(semiesfera);

   NodoGrafoEscena * nodo_escalado_cilindro_pierna = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_pierna,proporcion_alto_pierna,proporcion_ancho_pierna));
   NodoGrafoEscena * nodo_escalado_cilindro_cuerpo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_cuerpo,proporcion_alto_cuerpo,proporcion_ancho_cuerpo));
   NodoGrafoEscena * nodo_escalado_cilindro_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_alto_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_cilindro_antena = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_antena,proporcion_alto_antena,proporcion_ancho_antena));
   NodoGrafoEscena * nodo_escalado_semiesfera_cabeza = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_cuerpo,proporcion_alto_cabeza,proporcion_ancho_cuerpo));
   NodoGrafoEscena * nodo_escalado_semiesfera_superior_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_semiesfera_inferior_brazo = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo));
   NodoGrafoEscena * nodo_escalado_semiesfera_pierna = new NodoTransformacion(Matriz4x4::Escalado(proporcion_ancho_pierna,proporcion_ancho_pierna,proporcion_ancho_pierna));;



   NodoGrafoEscena * nodo_traslacion_pierna_izquierda = new NodoTransformacion(Matriz4x4::Traslacion(-(proporcion_ancho_cuerpo-1.3),0.0,0.0));
   NodoGrafoEscena * nodo_traslacion_pierna_derecha = new NodoTransformacion(Matriz4x4::Traslacion(proporcion_ancho_cuerpo-1.3,0.0,0.0));
   NodoGrafoEscena * nodo_traslacion_semiesfera_pierna = new NodoTransformacion(Matriz4x4::Traslacion(0.0,proporcion_alto_pierna,0.0)); // Esta no varía ya que el escalado se hará sobre todo el brazo
   NodoGrafoEscena * nodo_traslacion_semiesfera_brazo = new NodoTransformacion(Matriz4x4::Traslacion(0.0,proporcion_alto_brazo,0.0)); // Esta no varía ya que el escalado se hará sobre todo el brazo
   NodoGrafoEscena * nodo_traslacion_brazo_izquierdo = new NodoTransformacion(Matriz4x4::Traslacion(-(proporcion_ancho_cuerpo+proporcion_ancho_brazo),proporcion_altura_brazo,0.0));
   NodoGrafoEscena * nodo_traslacion_brazo_derecho = new NodoTransformacion(Matriz4x4::Traslacion(proporcion_ancho_cuerpo+proporcion_ancho_brazo,proporcion_altura_brazo,0.0));

   NodoGrafoEscena * nodo_traslacion_cabeza = new NodoTransformacion(Matriz4x4::Traslacion(0.0,proporcion_alto_cuerpo + separacion_cuerpo_cabeza, 0.0));

   NodoGrafoEscena * nodo_rotacion_pi = new NodoTransformacion(Matriz4x4::RotacionEjeX(M_PI));
   NodoGrafoEscena * nodo_rotacion_antena_izquierda = new NodoTransformacion(Matriz4x4::RotacionEjeZ(-M_PI/12));

   // Matrices de transformacion para las antenas

   Matriz4x4 matriz_rotacion_antena_izquierda = Matriz4x4::Identidad();
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * Matriz4x4::RotacionEjeZ(-M_PI/6);
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * Matriz4x4::Traslacion(0.0,proporcion_alto_cabeza,0.0);
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * Matriz4x4::Escalado(proporcion_ancho_antena,proporcion_alto_antena,proporcion_ancho_antena);

   Matriz4x4 matriz_rotacion_antena_derecha = Matriz4x4::Identidad();
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * Matriz4x4::RotacionEjeZ(M_PI/6);
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * Matriz4x4::Traslacion(0.0,proporcion_alto_cabeza,0.0);
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * Matriz4x4::Escalado(proporcion_ancho_antena,proporcion_alto_antena,proporcion_ancho_antena);

   NodoGrafoEscena * nodo_traslacion_antena_izquierda = new NodoTransformacion(matriz_rotacion_antena_izquierda);
   NodoGrafoEscena * nodo_traslacion_antena_derecha = new NodoTransformacion(matriz_rotacion_antena_derecha);

   // Matrices de transformacion para los ojos
   Matriz4x4 matriz_transformacion_ojo_izquierdo = Matriz4x4::Identidad();
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * Matriz4x4::RotacionEjeZ(-M_PI/4);
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * Matriz4x4::RotacionEjeX(-M_PI/5);
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * Matriz4x4::Traslacion(0.0,proporcion_alto_cabeza,0.0);
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * Matriz4x4::Escalado(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo);

   Matriz4x4 matriz_transformacion_ojo_derecho = Matriz4x4::Identidad();
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * Matriz4x4::RotacionEjeZ(M_PI/4);
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * Matriz4x4::RotacionEjeX(-M_PI/5);
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * Matriz4x4::Traslacion(0.0,proporcion_alto_cabeza,0.0);
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * Matriz4x4::Escalado(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo);

   NodoGrafoEscena * nodo_transformacion_ojo_izquierdo = new NodoTransformacion(matriz_transformacion_ojo_izquierdo);
   NodoGrafoEscena * nodo_transformacion_ojo_derecho = new NodoTransformacion(matriz_transformacion_ojo_derecho);

   // Nodos parametrizados (son iguales pero en vez de guardar la matriz directamente guardan un puntero

   //parametros_traslacion = Tupla3f(4*proporcion_ancho_cuerpo, 0.0, 0.0);

   rotacion_pierna_izquierda = new Matriz4x4(Matriz4x4::RotacionEjeZ(angulo_rotacion_piernas));
   rotacion_pierna_derecha = new Matriz4x4(Matriz4x4::RotacionEjeZ(angulo_rotacion_piernas));
   rotacion_brazo_izquierdo = new Matriz4x4(Matriz4x4::RotacionEjeZ(angulo_rotacion_brazos));
   rotacion_brazo_derecho = new Matriz4x4(Matriz4x4::RotacionEjeZ(angulo_rotacion_brazos));
   rotacion_cuerpo = new Matriz4x4(Matriz4x4::RotacionEjeY(angulo_rotacion_cuerpo));
   traslacion = new Matriz4x4(Matriz4x4::Traslacion(distancia_eje_Y,0.0,0.0));

   NodoGrafoEscena * nodo_parametrizado_rotacion_pierna_izquierda = new NodoTransformacionParametrizado(rotacion_pierna_izquierda);
   NodoGrafoEscena * nodo_parametrizado_rotacion_pierna_derecha = new NodoTransformacionParametrizado(rotacion_pierna_derecha);
   NodoGrafoEscena * nodo_parametrizado_rotacion_brazo_izquierdo = new NodoTransformacionParametrizado(rotacion_brazo_izquierdo);
   NodoGrafoEscena * nodo_parametrizado_rotacion_brazo_derecho = new NodoTransformacionParametrizado(rotacion_brazo_derecho);
   NodoGrafoEscena * nodo_parametrizado_rotacion_cuerpo = new NodoTransformacionParametrizado(rotacion_cuerpo);
   NodoGrafoEscena * nodo_parametrizado_traslacion = new NodoTransformacionParametrizado(traslacion);


   // Pierna del Android

   pierna->aniadeHijo(nodo_escalado_cilindro_pierna);
      nodo_escalado_cilindro_pierna->aniadeHijo(nodo_cilindro);
   pierna->aniadeHijo(nodo_traslacion_semiesfera_pierna);
      nodo_traslacion_semiesfera_pierna->aniadeHijo(nodo_escalado_semiesfera_pierna);
         nodo_escalado_semiesfera_pierna->aniadeHijo(nodo_semiesfera);

   // Brazo del Android

   brazo->aniadeHijo(nodo_escalado_cilindro_brazo);
      nodo_escalado_cilindro_brazo->aniadeHijo(nodo_cilindro);
   brazo->aniadeHijo(nodo_traslacion_semiesfera_brazo);
      nodo_traslacion_semiesfera_brazo->aniadeHijo(nodo_escalado_semiesfera_superior_brazo);
         nodo_escalado_semiesfera_superior_brazo->aniadeHijo(nodo_semiesfera);
   brazo->aniadeHijo(nodo_escalado_semiesfera_inferior_brazo);
      nodo_escalado_semiesfera_inferior_brazo->aniadeHijo(nodo_rotacion_pi);
         nodo_rotacion_pi->aniadeHijo(nodo_semiesfera);

   // Cabeza del Android

   cabeza->aniadeHijo(nodo_escalado_semiesfera_cabeza);
      nodo_escalado_semiesfera_cabeza->aniadeHijo(nodo_semiesfera);
   cabeza->aniadeHijo(nodo_traslacion_antena_izquierda);
      nodo_traslacion_antena_izquierda->aniadeHijo(nodo_cilindro);
   cabeza->aniadeHijo(nodo_traslacion_antena_derecha);
      nodo_traslacion_antena_derecha->aniadeHijo(nodo_cilindro);
   cabeza->aniadeHijo(nodo_transformacion_ojo_izquierdo);
      nodo_transformacion_ojo_izquierdo->aniadeHijo(nodo_semiesfera);
   cabeza->aniadeHijo(nodo_transformacion_ojo_derecho);
      nodo_transformacion_ojo_derecho->aniadeHijo(nodo_semiesfera);

   // Todo el Android

   // Cuerpo
   Android->aniadeHijo(nodo_escalado_cilindro_cuerpo);
      nodo_escalado_cilindro_cuerpo->aniadeHijo(nodo_cilindro);

   // Pierna izquierda
   Android->aniadeHijo(nodo_traslacion_pierna_izquierda);
      nodo_traslacion_pierna_izquierda->aniadeHijo(nodo_parametrizado_rotacion_pierna_izquierda);
         nodo_parametrizado_rotacion_pierna_izquierda->aniadeHijo(pierna);

   // Pierna derecha
   Android->aniadeHijo(nodo_traslacion_pierna_derecha);
      nodo_traslacion_pierna_derecha->aniadeHijo(nodo_parametrizado_rotacion_pierna_derecha);
         nodo_parametrizado_rotacion_pierna_derecha->aniadeHijo(pierna);

   // Cabeza
   Android->aniadeHijo(nodo_traslacion_cabeza);
      nodo_traslacion_cabeza->aniadeHijo(cabeza);

   // Brazo izquierdo
   Android->aniadeHijo(nodo_traslacion_brazo_izquierdo);
      nodo_traslacion_brazo_izquierdo->aniadeHijo(nodo_parametrizado_rotacion_brazo_izquierdo);
         nodo_parametrizado_rotacion_brazo_izquierdo->aniadeHijo(brazo);

   // Brazo derecho
   Android->aniadeHijo(nodo_traslacion_brazo_derecho);
      nodo_traslacion_brazo_derecho->aniadeHijo(nodo_parametrizado_rotacion_brazo_derecho);
         nodo_parametrizado_rotacion_brazo_derecho->aniadeHijo(brazo);

   raiz->aniadeHijo(nodo_parametrizado_rotacion_cuerpo);
      nodo_parametrizado_rotacion_cuerpo->aniadeHijo(nodo_parametrizado_traslacion);
         nodo_parametrizado_traslacion->aniadeHijo(Android);




}

void Practica3::DibujarObjetos()
{
   switch (modo_dibujo)
   {
      case ALAMBRE:
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         break;
      case PUNTOS:
         glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
         break;
      case SOLIDO:
      case AJEDREZ:
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
      this->semiesfera->CambioModoDibujo(this->modo_dibujo);
   if (this->cilindro != NULL)
      this->cilindro->CambioModoDibujo(this->modo_dibujo);
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
      cout << "Grado de libertad inválido" << endl;
      exit(-4);
   }

   if (grado_libertad == 1 || grado_libertad == -1)
   {
      float inc = incremento_angulo_rotacion_cuerpo * signo(grado_libertad);
      angulo_rotacion_cuerpo += inc;

      *rotacion_cuerpo = Matriz4x4::RotacionEjeY(angulo_rotacion_cuerpo);

   }
   else if (grado_libertad == 2 || grado_libertad == -2)
   {
      float inc = incremento_angulo_rotacion_brazos * signo(grado_libertad);
      float angulo_rotacion_brazos_futuro = angulo_rotacion_brazos + inc;

      if (angulo_rotacion_brazos_futuro <= 4*M_PI/3
            && angulo_rotacion_brazos_futuro >= 2*M_PI/3)
         angulo_rotacion_brazos = angulo_rotacion_brazos_futuro;

      *rotacion_brazo_izquierdo = Matriz4x4::RotacionEjeX(angulo_rotacion_brazos);
      *rotacion_brazo_derecho = Matriz4x4::RotacionEjeX(-angulo_rotacion_brazos);
   }
   else if (grado_libertad == 3 || grado_libertad == -3)
   {
      float inc = incremento_angulo_rotacion_piernas * signo(grado_libertad);
      float angulo_rotacion_piernas_futuro = angulo_rotacion_piernas + inc;

      if (angulo_rotacion_piernas_futuro <= 4*M_PI/3
            && angulo_rotacion_piernas_futuro >= 2*M_PI/3)
         angulo_rotacion_piernas = angulo_rotacion_piernas_futuro;

      *rotacion_pierna_izquierda = Matriz4x4::RotacionEjeX(angulo_rotacion_piernas);
      *rotacion_pierna_derecha = Matriz4x4::RotacionEjeX(-angulo_rotacion_piernas);
   }
   else if (grado_libertad == 4 || grado_libertad == -4)
   {
      float inc = incremento_traslacion_cuerpo * signo(grado_libertad);
      distancia_eje_Y += inc;

      *traslacion = Matriz4x4::Traslacion(distancia_eje_Y,0.0,0.0);
   }

}

void Practica3::CambioColorFijo()
{
   if (this->semiesfera != NULL)
      this->semiesfera->CambioColorFijo();
   if (this->cilindro != NULL)
      this->cilindro->CambioColorFijo();
}

bool Practica3::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   switch(tecla)
   {
      // Cambio de modo de dibujo (igual que en prácticas 1 y 2)
      case 'l':
         CambioModoDibujo(ALAMBRE);
         break;
      case 's':
         CambioModoDibujo(SOLIDO);
         break;
      case 'd':
         CambioModoDibujo(SOLIDO_CARAS);
         break;
      case 'p':
         CambioModoDibujo(PUNTOS);
         break;
      case 'a':
         CambioModoDibujo(AJEDREZ);
         break;
      /*case 'n':
         CambioModoNormales();
         break;*/
      case 'f':
         CambioColorFijo();
         break;

      // Control de grados de libertad
      case 'Z':
         CambioGradoLibertad(1);
         break;
      case 'z':
         CambioGradoLibertad(-1);
         break;
      case 'X':
         CambioGradoLibertad(2);
         break;
      case 'x':
         CambioGradoLibertad(-2);
         break;
      case 'C':
         CambioGradoLibertad(3);
         break;
      case 'c':
         CambioGradoLibertad(-3);
         break;
      case 'V':
         CambioGradoLibertad(4);
         break;
      case 'v':
         CambioGradoLibertad(-4);
         break;
      case 'b':
         velocidad_angular_cuerpo -= incremento_velocidad_rotacion_cuerpo;
         break;
      case 'B':
         velocidad_angular_cuerpo += incremento_velocidad_rotacion_cuerpo;
         break;
      case 'n':
         velocidad_angular_brazos -= incremento_velocidad_rotacion_brazos;
         break;
      case 'N':
         velocidad_angular_brazos += incremento_velocidad_rotacion_brazos;
         break;
      case 'm':
         velocidad_angular_piernas -= incremento_velocidad_rotacion_piernas;
         break;
      case 'M':
         velocidad_angular_piernas += incremento_velocidad_rotacion_piernas;
         break;
      default:
         redisp = false;
         break;
   }
   return redisp;
}

void Practica3::Animar()
{
   if (!(angulo_rotacion_brazos <= 4*M_PI/3
               && angulo_rotacion_brazos >= 2*M_PI/3))
            velocidad_angular_brazos *= -1;

   if (!(angulo_rotacion_piernas <= 4*M_PI/3
                  && angulo_rotacion_piernas >= 2*M_PI/3))
               velocidad_angular_piernas *= -1;

   angulo_rotacion_cuerpo  += velocidad_angular_cuerpo;
   angulo_rotacion_brazos  += velocidad_angular_brazos;
   angulo_rotacion_piernas += velocidad_angular_piernas;

   *rotacion_cuerpo = Matriz4x4::RotacionEjeY(angulo_rotacion_cuerpo);
   *rotacion_brazo_izquierdo = Matriz4x4::RotacionEjeX(angulo_rotacion_brazos);
   *rotacion_brazo_derecho = Matriz4x4::RotacionEjeX(-angulo_rotacion_brazos);
   *rotacion_pierna_izquierda = Matriz4x4::RotacionEjeX(angulo_rotacion_piernas);
   *rotacion_pierna_derecha = Matriz4x4::RotacionEjeX(-angulo_rotacion_piernas);
}
