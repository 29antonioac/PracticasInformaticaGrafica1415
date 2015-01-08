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

#include "MallaTVT.hpp"
#include "file_ply_stl.hpp"

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp> // vec3
#include <glm/vec4.hpp> // vec4, ivec4
#include <glm/mat4x4.hpp> // mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp> // value_ptr

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::translate;
using glm::rotate;
using glm::scale;
using glm::perspective;

const float PI = (float)M_PI;




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

Practica3::Practica3(GLuint idProg) : Practica(idProg)
{
   modo_dibujo = ALAMBRE;
   raiz = nullptr;
   semiesfera = semiesfera_ojo = nullptr;
   cilindro = nullptr;

   angulo_rotacion_cuerpo = M_PI;
   angulo_rotacion_brazos = M_PI;
   angulo_rotacion_piernas = M_PI;

   direccion_rotacion_brazos = direccion_rotacion_piernas = 1;
   distancia_eje_Y = velocidad_angular_cuerpo = velocidad_angular_brazos = velocidad_angular_piernas = 0;

   rotacion_cuerpo = rotacion_brazo_izquierdo = rotacion_brazo_derecho = rotacion_pierna_izquierda = rotacion_pierna_derecha = traslacion = nullptr;

   fuente1 = fuente2 = fuente3 = fuente4 = nullptr;
   material_android = material_ojo = nullptr;


}

Practica3::~Practica3()
{
   //raiz->Destruye();

   delete semiesfera;
   delete cilindro;
   delete rotacion_cuerpo;
   delete rotacion_brazo_izquierdo;
   delete rotacion_brazo_derecho;
   delete rotacion_pierna_izquierda;
   delete rotacion_pierna_derecha;

   delete traslacion;

}

void Practica3::Inicializar( int argc, char *argv[] )
{


   // ----------------- Creamos una semiesfera por revolución -----------------

   //vector<vec3> vertices_semiesfera;

   vertices_semiesfera.push_back(vec3(1.0,0.0,0.0));

   unsigned vertices_perfil_semiesfera = 20;
   //float incremento_perfil_semiesfera = (M_PI/2)/vertices_perfil_semiesfera;
   float incremento_perfil_semiesfera = (PI/2)/vertices_perfil_semiesfera;
   cout << "inc: " << incremento_perfil_semiesfera << endl;

   mat4 matriz_rotacion_perfil_semiesfera = rotate(mat4(1.0),incremento_perfil_semiesfera,vec3(0.0f,0.0f,1.0f));

   for (unsigned i = 0; i < 4; i++)
   {
      for (unsigned j = 0; j < 4; j++)
      {
         cout << "\t" << matriz_rotacion_perfil_semiesfera[i][j] << " ";
      }
      cout << endl;
   }

   for (unsigned i = 1; i <= vertices_perfil_semiesfera; i++)
   {
      vertices_semiesfera.push_back(vec3( matriz_rotacion_perfil_semiesfera * vec4(vertices_semiesfera[i-1],0.0)));
      cout << "(" << vertices_semiesfera[i][0] << "," << vertices_semiesfera[i][1] << "," << vertices_semiesfera[i][2] << ")" << endl;
   }

   semiesfera = new MallaTVT(PERFIL,vertices_semiesfera);
   semiesfera->Revolucion(20);

   semiesfera_ojo = new MallaTVT(semiesfera);

   // ----------------- Creamos un cilindro por revolución -----------------

   vector<vec3> vertices_cilindro;

   vertices_cilindro.push_back(vec3(1.0,0.0,0.0));

   unsigned vertices_perfil_cilindro = 20;
   float incremento_perfil_cilindro = 1.0/vertices_perfil_cilindro;

   for (unsigned i = 1; i <= vertices_perfil_cilindro; i++)
      vertices_cilindro.push_back(vec3(translate(mat4(1.0),vec3(0.0,incremento_perfil_cilindro,0.0)) * vec4(vertices_cilindro[i-1],1.0)));
         //vertices_cilindro.push_back(mat4::Traslacion(0.0,incremento_perfil_cilindro,0.0)*vertices_cilindro[i-1]);

   cilindro = new MallaTVT(PERFIL,vertices_cilindro);
   cilindro->Revolucion(20);

   // ------------------------------------------------------------------------

   this->modo_dibujo = semiesfera->getModoDibujo();

   // Creamos un brazo del Android

   raiz = new NodoGrafoEscena;

   float proporcion_alto_cuerpo = 3.0;
   float proporcion_alto_pierna = 1.2;
   float proporcion_alto_brazo = 1.2;
   float proporcion_alto_antena = 0.7;
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
   NodoGrafoEscena * nodo_semiesfera_ojo = new NodoTerminal(semiesfera_ojo);

   NodoGrafoEscena * nodo_escalado_cilindro_pierna = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_pierna,proporcion_alto_pierna,proporcion_ancho_pierna)));
   NodoGrafoEscena * nodo_escalado_cilindro_cuerpo = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_cuerpo,proporcion_alto_cuerpo,proporcion_ancho_cuerpo)));
   NodoGrafoEscena * nodo_escalado_cilindro_brazo = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_brazo,proporcion_alto_brazo,proporcion_ancho_brazo)));

   NodoGrafoEscena * nodo_escalado_semiesfera_cabeza = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_cuerpo,proporcion_alto_cabeza,proporcion_ancho_cuerpo)));
   NodoGrafoEscena * nodo_escalado_semiesfera_superior_brazo = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo)));
   NodoGrafoEscena * nodo_escalado_semiesfera_inferior_brazo = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_brazo,proporcion_ancho_brazo,proporcion_ancho_brazo)));
   NodoGrafoEscena * nodo_escalado_semiesfera_pierna = new NodoTransformacion(scale(mat4(1.0),vec3(proporcion_ancho_pierna,proporcion_ancho_pierna,proporcion_ancho_pierna)));;

   //translate(mat4(1.0),vec3(

   NodoGrafoEscena * nodo_traslacion_pierna_izquierda = new NodoTransformacion(translate(mat4(1.0),vec3(-(proporcion_ancho_cuerpo-1.3),0.0,0.0)));
   NodoGrafoEscena * nodo_traslacion_pierna_derecha = new NodoTransformacion(translate(mat4(1.0),vec3(proporcion_ancho_cuerpo-1.3,0.0,0.0)));
   NodoGrafoEscena * nodo_traslacion_semiesfera_pierna = new NodoTransformacion(translate(mat4(1.0),vec3(0.0,proporcion_alto_pierna,0.0))); // Esta no varía ya que el escalado se hará sobre todo el brazo
   NodoGrafoEscena * nodo_traslacion_semiesfera_brazo = new NodoTransformacion(translate(mat4(1.0),vec3(0.0,proporcion_alto_brazo,0.0))); // Esta no varía ya que el escalado se hará sobre todo el brazo
   NodoGrafoEscena * nodo_traslacion_brazo_izquierdo = new NodoTransformacion(translate(mat4(1.0),vec3(-(proporcion_ancho_cuerpo+proporcion_ancho_brazo),proporcion_altura_brazo,0.0)));
   NodoGrafoEscena * nodo_traslacion_brazo_derecho = new NodoTransformacion(translate(mat4(1.0),vec3(proporcion_ancho_cuerpo+proporcion_ancho_brazo,proporcion_altura_brazo,0.0)));

   NodoGrafoEscena * nodo_traslacion_cabeza = new NodoTransformacion(translate(mat4(1.0),vec3(0.0,proporcion_alto_cuerpo + separacion_cuerpo_cabeza, 0.0)));

   NodoGrafoEscena * nodo_rotacion_pi = new NodoTransformacion(rotate(mat4(1.0),PI,vec3(1.0,0.0,0.0)));

   // Matrices de transformacion para las antenas

   mat4 matriz_rotacion_antena_izquierda = mat4(1.0);
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * rotate(mat4(1.0),-PI/6,vec3(0.0,0.0,1.0));
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_rotacion_antena_izquierda = matriz_rotacion_antena_izquierda * scale(mat4(1.0),vec3(proporcion_ancho_antena,proporcion_alto_antena,proporcion_ancho_antena));

   mat4 matriz_rotacion_antena_derecha = mat4(1.0);
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * rotate(mat4(1.0),PI/6,vec3(0.0,0.0,1.0));
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_rotacion_antena_derecha = matriz_rotacion_antena_derecha * scale(mat4(1.0),vec3(proporcion_ancho_antena,proporcion_alto_antena,proporcion_ancho_antena));

   NodoGrafoEscena * nodo_transformacion_antena_izquierda = new NodoTransformacion(matriz_rotacion_antena_izquierda);
   NodoGrafoEscena * nodo_transformacion_antena_derecha = new NodoTransformacion(matriz_rotacion_antena_derecha);

   // Matrices de transformacion para los ojos
   mat4 matriz_transformacion_ojo_izquierdo = mat4(1.0);
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * rotate(mat4(1.0),-PI/4,vec3(0.0,0.0,1.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * rotate(mat4(1.0),-PI/5,vec3(1.0,0.0,0.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * scale(mat4(1.0),vec3(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo));

   mat4 matriz_transformacion_ojo_derecho = mat4(1.0);
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * rotate(mat4(1.0),PI/4,vec3(0.0,0.0,1.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * rotate(mat4(1.0),-PI/5,vec3(1.0,0.0,0.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * scale(mat4(1.0),vec3(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo));

   NodoGrafoEscena * nodo_transformacion_ojo_izquierdo = new NodoTransformacion(matriz_transformacion_ojo_izquierdo);
   NodoGrafoEscena * nodo_transformacion_ojo_derecho = new NodoTransformacion(matriz_transformacion_ojo_derecho);

   // Nodos parametrizados (son iguales pero, en vez de guardar la matriz directamente, guardan un puntero)

   rotacion_pierna_izquierda = new mat4(rotate(mat4(1.0),angulo_rotacion_piernas,vec3(0.0,0.0,1.0)));
   rotacion_pierna_derecha = new mat4(rotate(mat4(1.0),angulo_rotacion_piernas,vec3(0.0,0.0,1.0)));
   rotacion_brazo_izquierdo = new mat4(rotate(mat4(1.0),angulo_rotacion_brazos,vec3(0.0,0.0,1.0)));
   rotacion_brazo_derecho = new mat4(rotate(mat4(1.0),angulo_rotacion_brazos,vec3(0.0,0.0,1.0)));
   rotacion_cuerpo = new mat4(rotate(mat4(1.0),angulo_rotacion_cuerpo,vec3(0.0,1.0,0.0)));
   traslacion = new mat4(translate(mat4(1.0),vec3(distancia_eje_Y,0.0,0.0)));

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
   cabeza->aniadeHijo(nodo_transformacion_antena_izquierda);
      nodo_transformacion_antena_izquierda->aniadeHijo(nodo_cilindro);
   cabeza->aniadeHijo(nodo_transformacion_antena_derecha);
      nodo_transformacion_antena_derecha->aniadeHijo(nodo_cilindro);
   cabeza->aniadeHijo(nodo_transformacion_ojo_izquierdo);
      nodo_transformacion_ojo_izquierdo->aniadeHijo(nodo_semiesfera_ojo);
   cabeza->aniadeHijo(nodo_transformacion_ojo_derecho);
      nodo_transformacion_ojo_derecho->aniadeHijo(nodo_semiesfera_ojo);

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

   // Luces y material (extensión de la práctica 4)

   glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
   glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR ) ;
   glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

   vec3 luz_direccional_componente_ambiental(1.0,1.0,1.0);
   vec3 luz_direccional_componente_difusa(0.8,0.8,0.8);
   vec3 luz_direccional_componente_especular(0.8,0.8,0.8);

   fuente1 = new FuenteLuzPosicional(vec3(20.0,0.0,20.0), luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);
   fuente2 = new FuenteLuzPosicional(vec3(20.0,0.0,-20.0), luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);
   fuente3 = new FuenteLuzPosicional(vec3(-20.0,0.0,20.0), luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);
   fuente4 = new FuenteLuzPosicional(vec3(-20.0,0.0,-20.0), luz_direccional_componente_ambiental, luz_direccional_componente_difusa, luz_direccional_componente_especular);


   fuentes.Agregar(fuente1);
   fuentes.Agregar(fuente2);
   fuentes.Agregar(fuente3);
   fuentes.Agregar(fuente4);

   vec3 android_componente_emision(0.0,1.0,0.0);  // Color verde
   vec3 android_componente_ambiental(0.0,0.0,0.0);
   vec3 android_componente_difusa(0.3,0.3,0.3);
   vec3 android_componente_especular(0.8,0.8,0.8);
   float android_exponente_especular = 2.0;

   vec3 ojo_componente_emision(0.0,0.0,0.0);  // Color negro
   vec3 ojo_componente_ambiental(0.0,0.0,0.0);
   vec3 ojo_componente_difusa(0.0,0.0,0.0);
   vec3 ojo_componente_especular(0.0,0.0,0.0);
   float ojo_exponente_especular = 0.0;

   material_android = new Material(android_componente_emision, android_componente_ambiental,
         android_componente_difusa, android_componente_especular, android_exponente_especular);

   material_ojo = new Material(ojo_componente_emision, ojo_componente_ambiental,
            ojo_componente_difusa, ojo_componente_especular, ojo_exponente_especular);


   semiesfera->SetMaterial(material_android);
   cilindro->SetMaterial(material_android);
   semiesfera_ojo->SetMaterial(material_ojo);

   //vec3 ej(1.1,2.2,3.3);

   //cout << "Ejemplo" << ej << " -> " << AniadeW(ej,1.0) << endl;



}

#define DEBUG

void Practica3::DibujarObjetos()
{
   /*
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
   */
   glUseProgram(idProg);

   glEnable( GL_LIGHTING );
   glEnable( GL_NORMALIZE );
   glDisable( GL_COLOR_MATERIAL );

   // Dibujar aquí
   fuentes.Activar();
   raiz->Procesa();

   glDisable( GL_LIGHTING );
   glDisable( GL_NORMALIZE );
   glEnable( GL_COLOR_MATERIAL );

}

void Practica3::CambioModoDibujo(visualizacion modo_dibujo)
{
   this->modo_dibujo = modo_dibujo;
   if (this->semiesfera != nullptr)
      this->semiesfera->CambioModoDibujo(this->modo_dibujo);
   if (this->cilindro != nullptr)
      this->cilindro->CambioModoDibujo(this->modo_dibujo);
}

void Practica3::CambioModoNormales()
{
   if (this->semiesfera != nullptr)
      this->semiesfera->CambioModoNormales();
   if (this->cilindro != nullptr)
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

      if (angulo_rotacion_cuerpo > 2*M_PI) angulo_rotacion_cuerpo -= 2*M_PI;
      if (angulo_rotacion_cuerpo < 0) angulo_rotacion_cuerpo += 2*M_PI;

      *rotacion_cuerpo = rotate(mat4(1.0),angulo_rotacion_cuerpo,vec3(0.0,1.0,0.0));

   }
   else if (grado_libertad == 2 || grado_libertad == -2)
   {
      float inc = incremento_angulo_rotacion_brazos * signo(grado_libertad);
      float angulo_rotacion_brazos_futuro = angulo_rotacion_brazos + inc;

      if (angulo_rotacion_brazos_futuro <= 4*M_PI/3
            && angulo_rotacion_brazos_futuro >= 2*M_PI/3)
         angulo_rotacion_brazos = angulo_rotacion_brazos_futuro;

      *rotacion_brazo_izquierdo = rotate(mat4(1.0),angulo_rotacion_brazos,vec3(1.0,0.0,0.0));
      *rotacion_brazo_derecho = rotate(mat4(1.0),-angulo_rotacion_brazos,vec3(1.0,0.0,0.0));
   }
   else if (grado_libertad == 3 || grado_libertad == -3)
   {
      float inc = incremento_angulo_rotacion_piernas * signo(grado_libertad);
      float angulo_rotacion_piernas_futuro = angulo_rotacion_piernas + inc;

      if (angulo_rotacion_piernas_futuro <= 4*M_PI/3
            && angulo_rotacion_piernas_futuro >= 2*M_PI/3)
         angulo_rotacion_piernas = angulo_rotacion_piernas_futuro;

      *rotacion_pierna_izquierda = rotate(mat4(1.0),angulo_rotacion_piernas,vec3(1.0,0.0,0.0));
      *rotacion_pierna_derecha = rotate(mat4(1.0),-angulo_rotacion_piernas,vec3(1.0,0.0,0.0));
   }
   else if (grado_libertad == 4 || grado_libertad == -4)
   {
      float inc = incremento_traslacion_cuerpo * signo(grado_libertad);
      distancia_eje_Y += inc;

      if (distancia_eje_Y < 0) distancia_eje_Y = 0;
      else if (distancia_eje_Y > 10) distancia_eje_Y = 10;

      *traslacion = translate(mat4(1.0),vec3(distancia_eje_Y,0.0,0.0));
   }

}

void Practica3::CambioColorFijo()
{
   if (this->semiesfera != nullptr)
      this->semiesfera->CambioColorFijo();
   if (this->cilindro != nullptr)
      this->cilindro->CambioColorFijo();
}

bool Practica3::GestionarEvento(unsigned char tecla)
{
   bool redisp = true;

   switch(tecla)
   {
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
         if (velocidad_angular_cuerpo < 0) velocidad_angular_cuerpo = 0;
         break;
      case 'B':
         velocidad_angular_cuerpo += incremento_velocidad_rotacion_cuerpo;
         if (velocidad_angular_cuerpo > M_PI/30) velocidad_angular_cuerpo = M_PI/30;
         break;
      case 'n':
         velocidad_angular_brazos -= incremento_velocidad_rotacion_brazos;
         if (velocidad_angular_brazos < 0) velocidad_angular_brazos = 0;
         break;
      case 'N':
         velocidad_angular_brazos += incremento_velocidad_rotacion_brazos;
         if (velocidad_angular_brazos > M_PI/30) velocidad_angular_brazos = M_PI/30;
         break;
      case 'm':
         velocidad_angular_piernas -= incremento_velocidad_rotacion_piernas;
         if (velocidad_angular_piernas < 0) velocidad_angular_piernas = 0;
         break;
      case 'M':
         velocidad_angular_piernas += incremento_velocidad_rotacion_piernas;
         if (velocidad_angular_piernas > M_PI/30) velocidad_angular_piernas = M_PI/30;
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
            direccion_rotacion_brazos *= -1;

   if (!(angulo_rotacion_piernas <= 4*M_PI/3
                  && angulo_rotacion_piernas >= 2*M_PI/3))
               direccion_rotacion_piernas *= -1;

   if (angulo_rotacion_cuerpo > 2*M_PI) angulo_rotacion_cuerpo -= 2*M_PI;

   angulo_rotacion_cuerpo  += velocidad_angular_cuerpo;
   angulo_rotacion_brazos  += velocidad_angular_brazos * direccion_rotacion_brazos;
   angulo_rotacion_piernas += velocidad_angular_piernas * direccion_rotacion_piernas;


   *rotacion_cuerpo = rotate(mat4(1.0),angulo_rotacion_cuerpo,vec3(0.0,1.0,0.0));
   *rotacion_brazo_izquierdo = rotate(mat4(1.0),angulo_rotacion_brazos,vec3(1.0,0.0,0.0));
   *rotacion_brazo_derecho = rotate(mat4(1.0),-angulo_rotacion_brazos,vec3(1.0,0.0,0.0));
   *rotacion_pierna_izquierda = rotate(mat4(1.0),angulo_rotacion_piernas,vec3(1.0,0.0,0.0));
   *rotacion_pierna_derecha = rotate(mat4(1.0),-angulo_rotacion_piernas,vec3(1.0,0.0,0.0));
}

void Practica3::Debug()
{
   string str_color_fijo;
   if (semiesfera->ColorFijo())
      str_color_fijo = "Si";
   else
      str_color_fijo = "No";

   string cauce;

   if (idProg == 0)
      cauce = "Fijo";
   else
      cauce = "Programable";

   vector<string> debug_strings;
   debug_strings.push_back(string("Velocidad angular piernas: " + to_string(velocidad_angular_piernas)));
   debug_strings.push_back(string("Velocidad angular brazos: " + to_string(velocidad_angular_brazos)));
   debug_strings.push_back(string("Velocidad angular cuerpo: " + to_string(velocidad_angular_cuerpo)));
   debug_strings.push_back(string("Angulo de rotacion piernas: " + to_string(angulo_rotacion_piernas)));
   debug_strings.push_back(string("Angulo de rotacion brazos: " + to_string(angulo_rotacion_brazos)));
   debug_strings.push_back(string("Angulo de rotacion cuerpo: " + to_string(angulo_rotacion_cuerpo)));
   debug_strings.push_back(string("Distancia al eje Y: " + to_string(distancia_eje_Y)));
   debug_strings.push_back(string("Modo de normales: " + enumToString(semiesfera->getModoNormales())));
   debug_strings.push_back(string("Color fijo: " + str_color_fijo));
   debug_strings.push_back(string("Modo de dibujo: " + enumToString(modo_dibujo)));
   debug_strings.push_back(string("Cauce: " + cauce));
   debug_strings.push_back(string("Practica 3"));
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

void Practica3::Ayuda(vector<string> & strings_control)
{
   strings_control.push_back("M/m para modificar velocidad de rotacion de las piernas");
   strings_control.push_back("N/n para modificar velocidad de rotacion de los brazos");
   strings_control.push_back("B/b para modificar velocidad de rotacion del cuerpo");
   strings_control.push_back("V/v para modificar distancia al eje Y");
   strings_control.push_back("C/c para modificar rotacion de las piernas");
   strings_control.push_back("X/x para modificar rotacion de los brazos");
   strings_control.push_back("Z/z para modificar rotacion del cuerpo");


}
