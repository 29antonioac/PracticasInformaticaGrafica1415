// *********************************************************************
// **
// ** Informática Gráfica, curso 2014-15
// ** Carlos Ureña Almagro
// **
// ** Función 'main', inicialización y gestores de eventos
// **
// *********************************************************************

// includes de C/C++

#include <cctype>   // toupper
#include <string>   // std::string
#include <iostream> // std::cout
#include <fstream>  // ifstream
#include <cmath>    // fabs

// includes de OpenGL/glut

#include <GL/glew.h>
#include <GL/glut.h>  // freeglut standard (incluye cabecera opengl)
#include <GL/freeglut_ext.h> // extensiones freeglut (glutSolidCylinder)

// includes de archivos en el directorio de trabajo (de las prácticas)

#include "error-ogl.hpp"
#include "practica1.hpp"
#include "practica2.hpp"
#include "practica3.hpp"
#include "practica4.hpp"

using std::cout;
using std::endl;
using std::string;

// *********************************************************************
// **
// ** Variables globales
// ** (se inicializan en las funciones de inicialización)
// **
// *********************************************************************

// variables que definen la posicion de la camara en coordenadas polares

float 
   camara_angulo_x ,   // angulo de rotación entorno al eje X
   camara_angulo_y ;   // angulo de rotación entorno al eje Y

// ---------------------------------------------------------------------
// variables que definen el view-frustum (zona visible del mundo)

float 
   frustum_factor_escala ,  // factor de escala homogeneo que se aplica al frustum (sirve para alejar o acercar)
   frustum_ancho ,          // ancho, en coordenadas del mundo
   frustum_dis_del ,        // distancia al plano de recorte delantero (near)
   frustum_dis_tra ;        // distancia al plano de recorte trasero (far)

// ---------------------------------------------------------------------
// variables que determinan la posicion y tamaño inicial de la ventana 
// (el tamaño se actualiza al cambiar el tamaño durante la ejecución)

int 
   ventana_pos_x  = 50  ,
   ventana_pos_y  = 50  ,
   ventana_tam_x  = 1024,  // ancho inicial y actual de la ventana, en pixels
   ventana_tam_y  = 800 ;  // alto inicial actual de la ventana, en pixels


// Puntero a la práctica actual
Practica    * practicaActual;
Practica1   * practica1 = new Practica1;
Practica2   * practica2 = new Practica2;
Practica3   * practica3 = new Practica3;
Practica4   * practica4 = new Practica4;

bool debug = false;
bool ayuda = false;




// *********************************************************************
// **
// ** Funciones auxiliares:
// **
// *********************************************************************


// fija la transformación de proyeccion (zona visible del mundo == frustum)

void FijarProyeccion()
{
   const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );
   
   CError();
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // (3) proyectar en el plano de visión
   glFrustum
   (  -frustum_ancho,
      +frustum_ancho,
      -frustum_ancho*ratioYX,
      +frustum_ancho*ratioYX,
      +frustum_dis_del,
      +frustum_dis_tra
   );
   

   // (2) situar el origen (0,0,0), en mitad del view frustum 
   //     (en la rama negativa del eje Z)
   glTranslatef( 0.0,0.0,-0.5*(frustum_dis_del+frustum_dis_tra));
   
    // (1) aplicar factor de escala
   glScalef( frustum_factor_escala, frustum_factor_escala,  frustum_factor_escala );
   
   CError();
}


// ---------------------------------------------------------------------
// fijar viewport y proyección (viewport ocupa toda la ventana)

void FijarViewportProyeccion()
{
   glViewport( 0, 0, ventana_tam_x, ventana_tam_y );
   FijarProyeccion() ;
}

// ---------------------------------------------------------------------
// fija la transformación de vista (posiciona la camara)

void FijarCamara()
{
   
   CError();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glRotatef(camara_angulo_x,1,0,0);
   glRotatef(camara_angulo_y,0,1,0);
  
   //?
   //glScalef( factor_escala, factor_escala, factor_escala ); 
   CError();
}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjes()
{
   const float long_ejes = 10.0 ;
   
   // establecer modo de dibujo a lineas (podría estar en puntos)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   
   // dibujar tres segmentos
   glBegin(GL_LINES);
      // eje X, color rojo
      glColor3f( 1.0, 0.0, 0.0 );
      glVertex3f( -long_ejes, 0.0, 0.0 );
      glVertex3f( +long_ejes, 0.0, 0.0 );
      // eje Y, color verde
      glColor3f( 0.0, 1.0, 0.0 );
      glVertex3f( 0.0, -long_ejes, 0.0 );
      glVertex3f( 0.0, +long_ejes, 0.0 );
      // eje Z, color azul
      glColor3f( 0.0, 0.0, 1.0 );
      glVertex3f( 0.0, 0.0, -long_ejes );
      glVertex3f( 0.0, 0.0, +long_ejes );
   glEnd();
   
}

// ---------------------------------------------------------------------
// asigna el color de fondo actual a todos los pixels de la ventana

void LimpiarVentana()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Debug_Ayuda()
{

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, ventana_tam_x, 0.0, ventana_tam_y);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
   glColor3f(1.0f, 0.0f, 0.0f);

   void * font = GLUT_BITMAP_9_BY_15;
   vector<string> strings_ayuda;
   strings_ayuda.push_back("F6 activa/desactiva modo debug");
   strings_ayuda.push_back("F7 activa/desactiva mensaje con controles del programa");
   strings_ayuda.push_back("Ambos ralentizan en mayor o menor medida el renderizado");

   unsigned num_lineas = 0;
   for (auto &s: strings_ayuda)
   {
      glRasterPos2i(10, ventana_tam_y - 15*(num_lineas + 1));
      for (auto &c: s)
      {
        glutBitmapCharacter(font, c);
      }
      num_lineas++;
   }

   if (ayuda)
   {
      vector<string> strings_control;
      strings_control.push_back("Disponible control con flechas de teclado, +/- y raton ");
      practicaActual->Ayuda(strings_control);
      strings_control.push_back("F: Alternar entre color fijo (solo modo solido)");
      strings_control.push_back("H: Alternar modo dibujo normales");
      strings_control.push_back("A: Modo ajedrez");
      strings_control.push_back("P: Modo puntos");
      strings_control.push_back("D: Modo solido con atributos de caras");
      strings_control.push_back("S: Modo solido");
      strings_control.push_back("L: Modo alambre");
      strings_control.push_back("R: Resetear camara");
      strings_control.push_back("Q: Salir");

      float separacion = glutBitmapWidth(font, 'F') * strings_control[0].length();
      unsigned num_lineas = 0;
      for (auto &s: strings_control)
      {
         glRasterPos2i(ventana_tam_x - separacion - 5, 10+15*num_lineas);
         for (auto &c: s)
         {
           glutBitmapCharacter(font, c);
         }
         num_lineas++;
      }
   }

   if (debug)
      practicaActual->Debug();

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();

}


// *********************************************************************
// **
// ** Funciones gestoras de eventos
// **
// *********************************************************************

// F.G. del evento de redibujado (se produce cuando hay que volver a 
// dibujar la ventana, tipicamente tras producirse otros eventos)

void FGE_Redibujado()
{
   FijarViewportProyeccion() ; // necesario pues la escala puede cambiar
   FijarCamara();
   LimpiarVentana();
   DibujarEjes() ;
   practicaActual->DibujarObjetos();
   Debug_Ayuda();
   glFinish();
   glutSwapBuffers();
}

// ---------------------------------------------------------------------
// F.G. del evento de cambio de tamaño de la ventana

void FGE_CambioTamano( int nuevoAncho, int nuevoAlto )
{
   // guardar nuevo tamaño de la ventana
   ventana_tam_x  = nuevoAncho;
   ventana_tam_y  = nuevoAlto ;
   
   // reestablecer frustum, viewport y proyección
   FijarViewportProyeccion();
   
   // forzar un nuevo evento de redibujado, para actualizar ventana
   glutPostRedisplay();
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla normal
//
// parámetros:
//       tecla: carácter corresondiente a la tecla (minúscula)
//       x_raton, y_raton : posición del ratón al pulsar

void FGE_PulsarTeclaNormal( unsigned char tecla, int x_raton, int y_raton )
{

   bool redisp = true ;
   switch (tecla)
   {
      case 'q':
         exit( 0 );
         break ;
      case '+':
         frustum_factor_escala *= 1.05;
         break;
      case '-':
         frustum_factor_escala /= 1.05;
         break;
      case 'r':
         camara_angulo_x = 0.0 ;
         camara_angulo_y = 0.0 ;
         break;
      case 'l':
         practicaActual->CambioModoDibujo(ALAMBRE);
         break;
      case 's':
         practicaActual->CambioModoDibujo(SOLIDO);
         break;
      case 'd':
         practicaActual->CambioModoDibujo(SOLIDO_CARAS);
         break;
      case 'p':
         practicaActual->CambioModoDibujo(PUNTOS);
         break;
      case 'a':
         practicaActual->CambioModoDibujo(AJEDREZ);
         break;
      case 'h':
         practicaActual->CambioModoNormales();
         break;
      case 'f':
         practicaActual->CambioColorFijo();
         break;
      default:
         redisp = practicaActual->GestionarEvento(tecla) ;
         break ;
   }
   
   // si se ha cambiado algo, forzar evento de redibujado
   if (redisp)
      glutPostRedisplay();
}

// ---------------------------------------------------------------------
// F.G. del evento de pulsación de la tecla especial
//
// parámetros:
//       tecla: código GLUT de la tecla pulsada
//       x_raton, y_raton : posición del ratón al pulsar


void FGE_PulsarTeclaEspecial( int tecla, int x_raton, int y_raton )
{
   bool redisp = true ;
   const float da = 5.0 ; // incremento en grados de ángulos de camara
   
   switch ( tecla )
   {
      case GLUT_KEY_LEFT:
         camara_angulo_y = camara_angulo_y - da ;
         break;
      case GLUT_KEY_RIGHT:
         camara_angulo_y = camara_angulo_y + da ;
         break;
      case GLUT_KEY_UP:
         camara_angulo_x = camara_angulo_x - da ;
         break;
      case GLUT_KEY_DOWN:
         camara_angulo_x = camara_angulo_x + da ;
         break;
      case GLUT_KEY_PAGE_UP:
         frustum_factor_escala *= 1.05;
         break;
      case GLUT_KEY_PAGE_DOWN:
         frustum_factor_escala /= 1.05;
         break;
      case GLUT_KEY_F1:
         practicaActual = practica1;
         break;
      case GLUT_KEY_F2:
         practicaActual = practica2;
         break;
      case GLUT_KEY_F3:
         practicaActual = practica3;
         break;
      case GLUT_KEY_F4:
         practicaActual = practica4;
         break;
      case GLUT_KEY_F5:
         break;
      case GLUT_KEY_F6:
         if (debug) debug = false;
         else debug = true;
         break;
      case GLUT_KEY_F7:
         if (ayuda) ayuda = false;
         else ayuda = true;
         break;
      default:
         redisp = false ;
         break ;
	}

   // si se ha cambiado algo, forzar evento de redibujado
   if ( redisp )
      glutPostRedisplay();
}

/* Añadidos para mover ventana con ratón */

int origen[2] = {-1,-1};

void FGE_PulsarRaton(int boton, int estado, int x, int y)
{
   const float da = 5.0 ; // incremento en grados de ángulos de camara
   bool redisp = true;

   // Si pulsamos el botón izquierdo del ratón
   // nos preparamos para mover la cámara
   if (boton == GLUT_LEFT_BUTTON)
   {
      if (estado == GLUT_UP)  // Si levantamos el botón dejamos de girar
      {
         origen[0] = -1;
         origen[1] = -1;
      }
      else                    // Si pulsamos nos preparamos tomando el origen del movimiento
      {
         origen[0] = x;
         origen[1] = y;
      }
      redisp = false;
   }
   else if (boton == 3) // Rueda arriba aumenta el zoom
   {
      frustum_factor_escala *= 1.05;
   }
   else if (boton == 4) // Rueda abajo disminuye el zoom
   {
      frustum_factor_escala /= 1.05;
   }
   else if (boton == 5) // Llevar la rueda a la izquierda gira la cámara a la izquierda
   {
      camara_angulo_y -= da;
   }
   else if (boton == 6) // Llevar la rueda a la derecha gira la cámara a la derecha
   {
      camara_angulo_y += da;
   }

   if (redisp)
      glutPostRedisplay();
}

void FGE_MoverRaton(int x, int y)
{

   // Sólo movemos la cámara si el botón está abajo
   if (origen[0] >= 0 && origen[1] >= 0)
   {

      // Actualizar la dirección de la cámara
      camara_angulo_x += (y - origen[1])*0.25f;
      camara_angulo_y += (x - origen[0])*0.25f;

      origen[0] = x;
      origen[1] = y;

      // Redibujar
      glutPostRedisplay();
   }
}

void Animar()
{

   if (practicaActual == practica3)
      practica3->Animar();
   //else if (practicaActual == practica4)
   //   practica4->Animar();


   glutPostRedisplay();


}

// *********************************************************************
// **
// ** Funciones de inicialización
// **
// *********************************************************************

// inicialización de GLUT: creación de la ventana, designar FGEs

void Inicializa_GLUT( int argc, char * argv[] )
{
   
   // inicializa variables globales usadas en esta función (y otras)
   ventana_pos_x  = 50 ;
   ventana_pos_y  = 50  ;
   ventana_tam_x  = 1024 ;  // ancho inicial y actual de la ventana, en pixels
   ventana_tam_y  = 800 ;  // alto inicial actual de la ventana, en pixels
   
   // inicializa glut:
   glutInit( &argc, argv );
   
   // establece posicion inicial de la ventana:
   glutInitWindowPosition( ventana_pos_x, ventana_pos_y );
   
   // establece tamaño inicial de la ventana:
   glutInitWindowSize( ventana_tam_x, ventana_tam_y );
   
   // establece atributos o tipo de ventana:
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
   
   // crea y visualiza una ventana:
   glutCreateWindow("Practicas IG (14-15)");
   
   // establece función gestora del evento de redibujado:
   glutDisplayFunc( FGE_Redibujado );
   
   // establece función gestora del evento de cambio de tamaño:
   glutReshapeFunc( FGE_CambioTamano );
   
   // establece función gestora del evento de pulsación de tecla normal:
   glutKeyboardFunc( FGE_PulsarTeclaNormal );
    
   // establece función gestora del evento de pulsación de tecla especial:
   glutSpecialFunc( FGE_PulsarTeclaEspecial );

   // Añadido para mover la pantalla con ratón
   glutMouseFunc( FGE_PulsarRaton );
   glutMotionFunc( FGE_MoverRaton );

   // Añadido para animar
   glutIdleFunc(Animar);
   //glutTimerFunc(500,Animar,0);
}

// ---------------------------------------------------------------------
// inicialización de OpenGL

void Inicializa_OpenGL( )
{
   // borrar posibles errores anteriores
   CError();
   
   GLenum codigoError = glewInit();
   if ( codigoError != GLEW_OK ) // comprobar posibles errores
   {
      cout << "Imposible inicializar ’GLEW’, mensaje: "
            << glewGetErrorString(codigoError) << endl ;
      exit(1);
   }

   // comprobar si OpenGL ver 2.0 + está soportado (usando GLEW)
   if ( ! GLEW_VERSION_2_0 )
   {  cout << "OpenGL 2.0 no soportado." << endl  ;
      exit(1);
   }

   // habilitar test de comparación de profundidades para 3D (y 2D)
   // es necesario, no está habilitado por defecto:
   // https://www.opengl.org/wiki/Depth_Buffer
   glEnable( GL_DEPTH_TEST );
   
   // establecer color de fondo: (1,1,1) (blanco)
   glClearColor( 1.0, 1.0, 1.0, 1.0 ) ;
   
   // establecer color inicial para todas las primitivas
   glColor3f(1.0f,0.0f,0.0f);
   
   // establecer ancho de línea
   glLineWidth( 1.5 );
   
   // establecer tamaño de los puntos
   glPointSize( 3.0 );
   
   // establecer modo de visualización de prim.
   // (las tres posibilidades son: GL_POINT, GL_LINE, GL_FILL)
   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
   
   // inicializar parámetros del frustum
   frustum_dis_del         = 0.1 ;
   frustum_dis_tra         = 10.0;
   frustum_ancho           = 0.5*frustum_dis_del ;
   //frustum_ancho           = 1.0 ;
   frustum_factor_escala   = 1.0 ;
   
   // inicializar parámetros de la cámara
   camara_angulo_x = 0.0 ;
   camara_angulo_y = 0.0 ;

   // ??
   FijarViewportProyeccion() ;
   FijarCamara() ;
   
   
   // ya está
   CError();
}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] )
{
   // glut (crea la ventana)
   Inicializa_GLUT(argc, argv) ;
   
   // opengl: define proyección y atributos iniciales
   Inicializa_OpenGL() ;
   
   // inicializar prácticas
   practica1->Inicializar(argc, argv);
   practica2->Inicializar(argc, argv);
   practica3->Inicializar(argc, argv);
   practica4->Inicializar(argc, argv);

   practicaActual = practica4;
}

// *********************************************************************
// **
// ** Función principal
// **
// *********************************************************************


int main( int argc, char *argv[] )
{
   // incializar el programa
   Inicializar(argc, argv);
   
   // llamar al bucle de gestión de eventos de glut, tiene el 
   // control hasta el final de la aplicación
   glutMainLoop();
   
   // ya está
   return 0;
}
