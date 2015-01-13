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
#include "IDs_Shaders.hpp"

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

Practica3::Practica3()
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

   mat4 matriz_rotacion_perfil_semiesfera = rotate(mat4(1.0),incremento_perfil_semiesfera,vec3(0.0f,0.0f,1.0f));

   for (unsigned i = 1; i <= vertices_perfil_semiesfera; i++)
   {
      vertices_semiesfera.push_back(vec3( matriz_rotacion_perfil_semiesfera * vec4(vertices_semiesfera[i-1],0.0)));
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
   NodoGrafoEscena * ojos = new NodoGrafoEscena;
   NodoGrafoEscena * todo_menos_ojos = new NodoGrafoEscena;
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
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * translate(mat4(1.0),vec3(0.0,proporcion_alto_cuerpo + separacion_cuerpo_cabeza, 0.0));

   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * rotate(mat4(1.0),-PI/4,vec3(0.0,0.0,1.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * rotate(mat4(1.0),-PI/5,vec3(1.0,0.0,0.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_transformacion_ojo_izquierdo = matriz_transformacion_ojo_izquierdo * scale(mat4(1.0),vec3(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo));

   mat4 matriz_transformacion_ojo_derecho = mat4(1.0);
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * translate(mat4(1.0),vec3(0.0,proporcion_alto_cuerpo + separacion_cuerpo_cabeza, 0.0));

   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * rotate(mat4(1.0),PI/4,vec3(0.0,0.0,1.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * rotate(mat4(1.0),-PI/5,vec3(1.0,0.0,0.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * translate(mat4(1.0),vec3(0.0,proporcion_alto_cabeza,0.0));
   matriz_transformacion_ojo_derecho = matriz_transformacion_ojo_derecho * scale(mat4(1.0),vec3(proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo,proporcion_ancho_alto_ojo));

   NodoGrafoEscena * nodo_transformacion_ojo_izquierdo = new NodoTransformacion(matriz_transformacion_ojo_izquierdo);
   NodoGrafoEscena * nodo_transformacion_ojo_derecho = new NodoTransformacion(matriz_transformacion_ojo_derecho);

   //NodoGrafoEscena * nodo_traslacion_cabeza = new NodoTransformacion(translate(mat4(1.0),vec3(0.0,proporcion_alto_cuerpo + separacion_cuerpo_cabeza, 0.0)));

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

   // Nodos Shaders
   NodoGrafoEscena * nodo_shader_android = new NodoShader(idProg_P3_Android);
   NodoGrafoEscena * nodo_shader_ojos = new NodoShader(idProg_P3_Ojos);


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

   // Ojos del Android
   ojos->aniadeHijo(nodo_transformacion_ojo_izquierdo);
      nodo_transformacion_ojo_izquierdo->aniadeHijo(nodo_semiesfera_ojo);
   ojos->aniadeHijo(nodo_transformacion_ojo_derecho);
      nodo_transformacion_ojo_derecho->aniadeHijo(nodo_semiesfera_ojo);

   // Todo el Android

   // Cuerpo
   todo_menos_ojos->aniadeHijo(nodo_escalado_cilindro_cuerpo);
      nodo_escalado_cilindro_cuerpo->aniadeHijo(nodo_cilindro);

   // Pierna izquierda
   todo_menos_ojos->aniadeHijo(nodo_traslacion_pierna_izquierda);
      nodo_traslacion_pierna_izquierda->aniadeHijo(nodo_parametrizado_rotacion_pierna_izquierda);
         nodo_parametrizado_rotacion_pierna_izquierda->aniadeHijo(pierna);

   // Pierna derecha
   todo_menos_ojos->aniadeHijo(nodo_traslacion_pierna_derecha);
      nodo_traslacion_pierna_derecha->aniadeHijo(nodo_parametrizado_rotacion_pierna_derecha);
         nodo_parametrizado_rotacion_pierna_derecha->aniadeHijo(pierna);

   // Cabeza
   todo_menos_ojos->aniadeHijo(nodo_traslacion_cabeza);
      nodo_traslacion_cabeza->aniadeHijo(cabeza);

   // Ojos
   Android->aniadeHijo(nodo_shader_ojos);
      nodo_shader_ojos->aniadeHijo(ojos);

   // Brazo izquierdo
   todo_menos_ojos->aniadeHijo(nodo_traslacion_brazo_izquierdo);
      nodo_traslacion_brazo_izquierdo->aniadeHijo(nodo_parametrizado_rotacion_brazo_izquierdo);
         nodo_parametrizado_rotacion_brazo_izquierdo->aniadeHijo(brazo);

   // Brazo derecho
   todo_menos_ojos->aniadeHijo(nodo_traslacion_brazo_derecho);
      nodo_traslacion_brazo_derecho->aniadeHijo(nodo_parametrizado_rotacion_brazo_derecho);
         nodo_parametrizado_rotacion_brazo_derecho->aniadeHijo(brazo);

   Android->aniadeHijo(nodo_shader_android);
      nodo_shader_android->aniadeHijo(todo_menos_ojos);

      // Vamos a ampliar un poco el muñeco

   NodoGrafoEscena * nodo_escalado_general = new NodoTransformacion(scale(mat4(),vec3(3.0,3.0,3.0)));
   raiz->aniadeHijo(nodo_escalado_general);
      nodo_escalado_general->aniadeHijo(nodo_parametrizado_rotacion_cuerpo);
         nodo_parametrizado_rotacion_cuerpo->aniadeHijo(nodo_parametrizado_traslacion);
            nodo_parametrizado_traslacion->aniadeHijo(Android);


   //IDLuz = glGetUniformLocation(idProg_P3_Android, "posicion_luz_coordenadas_mundo");
   //if (IDLuz == -1)
   //{
   ///   cout << "Error en IDLuz, saliendo";
    //  glutLeaveMainLoop();
  // }

}


void Practica3::DibujarObjetos()
{

   //glEnable( GL_LIGHTING );
   //glEnable( GL_NORMALIZE );
  //glDisable( GL_COLOR_MATERIAL );

   // Dibujar aquí
   //fuentes.Activar();
   //glUniform3f(IDLuz,10.0,0.0,10.0);
   raiz->Procesa();
   //UsarPrograma(idProg_P3_Android);
   //hormiga->Visualizar();

   //glDisable( GL_LIGHTING );
   //glDisable( GL_NORMALIZE );
   //glEnable( GL_COLOR_MATERIAL );

}

void Practica3::CambioModoDibujo(visualizacion modo_dibujo)
{
   /*
   this->modo_dibujo = modo_dibujo;
   if (this->semiesfera != nullptr)
      this->semiesfera->CambioModoDibujo(this->modo_dibujo);
   if (this->semiesfera_ojo != nullptr)
      this->semiesfera->CambioModoDibujo(this->modo_dibujo);
   if (this->cilindro != nullptr)
      this->cilindro->CambioModoDibujo(this->modo_dibujo);
   */
}

void Practica3::CambioModoNormales()
{
   /*
   if (this->semiesfera != nullptr)
      this->semiesfera->CambioModoNormales();
   if (this->cilindro != nullptr)
      this->cilindro->CambioModoNormales();
      */
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

