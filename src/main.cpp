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

#include "PilaMatrices.hpp"
#include "IDs_Shaders.hpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

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
Practica1   * practica1;
Practica2   * practica2;
Practica3   * practica3;
Practica4   * practica4;

bool debug = false;
bool ayuda = false;
bool msaa = true;
//bool custom_shader = false;

GLuint id_VAO, id_VBO_Vertices_Ejes, id_VBO_Colores_Ejes;


const float long_ejes = 10.0 ;

using glm::vec3;
using glm::mat4;
using glm::lookAt;
using glm::perspective;


float ejes[] = {-long_ejes,0.0,0.0,
               +long_ejes,0.0,0.0,
               0.0,-long_ejes,0.0,
               0.0,+long_ejes,0.0,
               0.0,0.0,-long_ejes,
               0.0,0.0,+long_ejes
               };

float color_ejes[] = {
      1.0,0.0,0.0,
      1.0,0.0,0.0,
      0.0,1.0,0.0,
      0.0,1.0,0.0,
      0.0,0.0,1.0,
      0.0,0.0,1.0

   };





// *********************************************************************
// **
// ** Funciones auxiliares:
// **
// *********************************************************************


// fija la transformación de proyeccion (zona visible del mundo == frustum)

void FijarProyeccion()
{
   /*
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
   */
   const GLfloat ratioYX = GLfloat( ventana_tam_y )/GLfloat( ventana_tam_x );

   Proyeccion = mat4(1.0);
   Proyeccion *= glm::frustum(-frustum_ancho,
            +frustum_ancho,
            -frustum_ancho*ratioYX,
            +frustum_ancho*ratioYX,
            +frustum_dis_del,
            +frustum_dis_tra);
      Proyeccion *= glm::translate(mat4(1.0), vec3(0.0,0.0,-0.5*(frustum_dis_del+frustum_dis_tra)));
      Proyeccion *= glm::scale(mat4(1.0), vec3(frustum_factor_escala, frustum_factor_escala,  frustum_factor_escala ));

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
   /*
   CError();
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   glRotatef(camara_angulo_x,1,0,0);
   glRotatef(camara_angulo_y,0,1,0);
  
   //?
   //glScalef( factor_escala, factor_escala, factor_escala ); 
   CError();
   */

   Vista = mat4(1.0);
   Vista *= glm::rotate(mat4(1.0), camara_angulo_x, vec3(1.0,0.0,0.0));
   Vista *= glm::rotate(mat4(1.0), camara_angulo_y, vec3(0.0,1.0,0.0));
   Vista *= glm::scale(mat4(1.0),vec3(frustum_factor_escala, frustum_factor_escala, frustum_factor_escala));

   Modelado = mat4(1.0);
   MVP = Proyeccion * Vista * Modelado;
}

// ---------------------------------------------------------------------
// dibuja los ejes utilizando la primitiva grafica de lineas

void DibujarEjes()
{

   UsarPrograma(idProg_Ejes);
   cout << "idProg_Ejes: " << idProg_Ejes << endl;

   // Get a handle for our "MVP" uniform
   GLint IDModelado = glGetUniformLocation(idProg_Ejes, "Modelado");
   GLint IDVista = glGetUniformLocation(idProg_Ejes, "Vista");
   GLint IDProyeccion = glGetUniformLocation(idProg_Ejes, "Proyeccion");

   if (IDModelado == -1 || IDVista == -1 || IDProyeccion == -1)
   {
      cout << "Algún error con los ID de uniform, saliedo.." << endl;
      glutLeaveMainLoop();
   }


   glUniformMatrix4fv(IDModelado, 1, GL_FALSE, &Modelado[0][0]);
   glUniformMatrix4fv(IDVista, 1, GL_FALSE, &Vista[0][0]);
   glUniformMatrix4fv(IDProyeccion, 1, GL_FALSE, &Proyeccion[0][0]);

   glBindVertexArray(id_VAO);

   glEnableVertexAttribArray(0);
   glBindBuffer(GL_ARRAY_BUFFER, id_VBO_Vertices_Ejes);
   glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
   );

   glEnableVertexAttribArray(1);
   glBindBuffer(GL_ARRAY_BUFFER, id_VBO_Colores_Ejes);
   glVertexAttribPointer(
      1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
   );

   glDrawArrays(GL_LINES,0,6);

   glDisableVertexAttribArray(0);
   glDisableVertexAttribArray(1);

   glBindVertexArray(0);

   
   
}

// ---------------------------------------------------------------------
// asigna el color de fondo actual a todos los pixels de la ventana

void LimpiarVentana()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
   // Actualizamos nuestra pila interna
   pila_opengl.top() = Modelado;
   LimpiarVentana();
   //DibujarEjes() ;
   //cout << "Shader antes de dibujar objeto " << idProg_actual << endl;
   practicaActual->DibujarObjetos();
   //cout << "Shader despues de dibujar objeto " << idProg_actual << endl;
   //Debug_Ayuda();
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
         glutLeaveMainLoop();
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
      case 'e':
         msaa = !msaa;
         break;
      //case 'w':
         //custom_shader = !custom_shader;
      //   break;
      default:
         redisp = practicaActual->GestionarEvento(tecla) ;
         break ;
   }

   if (msaa)
   {
      glEnable(GL_MULTISAMPLE_ARB);
      //cout << "MSAA on" << endl;
   }
   else
   {
      glDisable(GL_MULTISAMPLE_ARB);
      //cout << "MSAA off" << endl;
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
   const float da = 0.3 ; // incremento en grados de ángulos de camara
   
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
   const float da = 0.3 ; // incremento en grados de ángulos de camara
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
      camara_angulo_x += (y - origen[1])*0.01f;
      camara_angulo_y += (x - origen[0])*0.01f;

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

/* Añadidos de shaders */

/* Añadidos shader */

char * LeerArchivo( const char * nombreArchivo )
{
   using namespace std;
   // intentar abrir stream, si no se puede informar y abortar
   ifstream file( nombreArchivo, ios::in|ios::binary|ios::ate );
   if ( ! file.is_open() )
   {
      std::cout << "imposible abrir archivo para lectura ("
            << nombreArchivo << ")" << std::endl ;
      exit(1);
   }
   //reservar memoria para guardar archivo completo
   size_t numBytes = file.tellg(); // leer tamaño total en bytes
   char * bytes = new char [numBytes+1]; // reservar memoria dinámica
   // leer bytes:
   file.seekg( 0, ios::beg ); // posicionar lectura al inicio
   file.read( bytes, numBytes ); // leer el archivo completo
   file.close(); // cerrar stream de lectura
   bytes[numBytes] = 0 ; // añadir cero al final
   // devolver puntero al primer elemento
   return bytes ;
}

void VerErroresCompilar( GLuint idShader )
{
   using namespace std ;
   const GLsizei maxt = 1024L*10L ;
   GLsizei tam ;
   GLchar buffer[maxt] ;
   GLint ok ;
   glGetShaderiv( idShader, GL_COMPILE_STATUS, &ok);
   if (ok == GL_TRUE)
      return;

   glGetShaderInfoLog(idShader, maxt, &tam, buffer); // leer log de errores
   cout  << "Error al compilar: " << endl
         << buffer
         << "programa abortado" << endl;

   exit(1); // abortar
}

void VerErroresEnlazar( GLuint idPrograma )
{
   using namespace std ;
   const GLsizei maxt = 1024L*10L ;
   GLsizei tam ;
   GLchar buffer[maxt] ;
   GLint ok ;
   glGetProgramiv( idPrograma, GL_COMPILE_STATUS, &ok);
   if (ok == GL_TRUE)
      return;

   glGetProgramInfoLog(idPrograma, maxt, &tam, buffer); // leer log de errores
   cout  << "Error al compilar: " << endl
         << buffer
         << "programa abortado" << endl;

   exit(1); // abortar
}

GLuint CompilarShader( const char * nombreArchivo, GLenum tipoShader )
{
   // crear shader nuevo, obtener identificador (tipo GLuint)
   const GLuint idShader = glCreateShader( tipoShader );
   // leer archivo fuente de shader en memoria, asociar fuente al shader
   const GLchar * fuente = LeerArchivo( nombreArchivo );
   glShaderSource( idShader, 1, &fuente, NULL );
   delete [] fuente ; fuente = NULL ; // libera memoria del fuente
   // compilar y comprobar errores
   glCompileShader( idShader );
   //VerErroresCompilar( idShader ); // opcional, muy conveniente
   // devolver identificador de shader como resultado
   return idShader ;
}






GLuint CrearPrograma( const char * archFrag, const char * archVert )
{
   // crear y compilar shaders, crear el programa
   const GLuint
   idFragShader = CompilarShader( archFrag, GL_FRAGMENT_SHADER ),
   idVertShader = CompilarShader( archVert, GL_VERTEX_SHADER ),
   idProg = glCreateProgram();
   // asociar shaders al programa
   glAttachShader( idProg, idFragShader );
   glAttachShader( idProg, idVertShader );
   // enlazar programa y comprobar errores
   glLinkProgram( idProg );
   //VerErroresEnlazar( idProg ); // opcional, muy conveniente
   // devolver identificador de programa
   return idProg ;
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
 
   // Create the shaders
   GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
   GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

   // Read the Vertex Shader code from the file
   std::string VertexShaderCode;
   std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
   if(VertexShaderStream.is_open())
   {
     std::string Line = "";
     while(getline(VertexShaderStream, Line))
         VertexShaderCode += "\n" + Line;
     VertexShaderStream.close();
   }

   // Read the Fragment Shader code from the file
   std::string FragmentShaderCode;
   std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
   if(FragmentShaderStream.is_open()){
     std::string Line = "";
     while(getline(FragmentShaderStream, Line))
         FragmentShaderCode += "\n" + Line;
     FragmentShaderStream.close();
   }

   GLint Result = GL_FALSE;
   int InfoLogLength;

   // Compile Vertex Shader
   //printf("Compiling shader : %s\n", vertex_file_path);
   cout << "########################################" << endl;
   cout << "Compilando vertex shader: " << vertex_file_path << endl;
   char const * VertexSourcePointer = VertexShaderCode.c_str();
   glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
   glCompileShader(VertexShaderID);

   // Check Vertex Shader
   glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
   glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
   std::vector<char> VertexShaderErrorMessage(InfoLogLength);
   glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
   cout << "Error vertex shader: ";
   for (auto c: VertexShaderErrorMessage)
      cout << c;
   cout << endl;
   //fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

   // Compile Fragment Shader
   //printf("Compiling shader : %s\n", fragment_file_path);
   cout << "Compilando fragment shader: " << fragment_file_path << endl;
   char const * FragmentSourcePointer = FragmentShaderCode.c_str();
   glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
   glCompileShader(FragmentShaderID);

   // Check Fragment Shader
   glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
   glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
   std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
   glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
   cout << "Error fragment shader: ";
   for (auto c: FragmentShaderErrorMessage)
      cout << c;
   cout << endl;
   //fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

   // Link the program
   //fprintf(stdout, "Linking program\n");
   cout << "Enlazando" << endl;
   GLuint ProgramID = glCreateProgram();
   glAttachShader(ProgramID, VertexShaderID);
   glAttachShader(ProgramID, FragmentShaderID);
   glLinkProgram(ProgramID);

   // Check the program
   glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
   glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
   std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
   glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
   cout << "Error programa: ";
   for (auto c: ProgramErrorMessage)
      cout << c;
   cout << endl;
   //fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

   glDeleteShader(VertexShaderID);
   glDeleteShader(FragmentShaderID);

   cout << "########################################" << endl;

   return ProgramID;
}

void LimpiarTodo()
{
   cout << "Borrar practica1" << endl;
   delete practica1;
   cout << "Borrar practica2" << endl;
   delete practica2;
   cout << "Borrar practica3" << endl;
   delete practica3;
   cout << "Borrar practica4" << endl;
   delete practica4;
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
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE );
   
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

   glutCloseFunc(LimpiarTodo);

   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                 GLUT_ACTION_GLUTMAINLOOP_RETURNS);
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
   if ( ! GLEW_VERSION_3_3 )
   {  cout << "OpenGL 3.3 no soportado." << endl  ;
      exit(1);
   }


   idProg_Ejes             = CrearPrograma("src/shaders/Ejes_fragment.glsl","src/shaders/Ejes_vertex.glsl");
   idProg_P1_P2            = CrearPrograma("src/shaders/P1_P2_fragment.glsl","src/shaders/P1_P2_vertex.glsl");
   idProg_P3_Android       = CrearPrograma("src/shaders/P3_Android_fragment.glsl","src/shaders/P3_Android_vertex.glsl");
   idProg_P3_Ojos          = CrearPrograma("src/shaders/P3_Ojos_fragment.glsl","src/shaders/P3_Ojos_vertex.glsl");
   idProg_P4_lata          = CrearPrograma("src/shaders/P4_lata_fragment.glsl","src/shaders/P4_lata_vertex.glsl");
   idProg_P4_tapas         = CrearPrograma("src/shaders/P4_tapas_fragment.glsl","src/shaders/P4_tapas_vertex.glsl");
   idProg_P4_peon_madera   = CrearPrograma("src/shaders/P4_peon_madera_fragment.glsl","src/shaders/P4_peon_madera_vertex.glsl");
   idProg_P4_peon_blanco   = CrearPrograma("src/shaders/P4_peon_blanco_fragment.glsl","src/shaders/P4_peon_blanco_vertex.glsl");
   idProg_P4_peon_negro    = CrearPrograma("src/shaders/P4_peon_negro_fragment.glsl","src/shaders/P4_peon_negro_vertex.glsl");

/*
   idProg_Ejes             = LoadShaders("src/shaders/Ejes_fragment.glsl","src/shaders/Ejes_vertex.glsl");
   idProg_P1_P2            = LoadShaders("src/shaders/P1_P2_fragment.glsl","src/shaders/P1_P2_vertex.glsl");
   idProg_P3_Android       = LoadShaders("src/shaders/P3_Android_fragment.glsl","src/shaders/P3_Android_vertex.glsl");
   idProg_P3_Ojos          = LoadShaders("src/shaders/P3_Ojos_fragment.glsl","src/shaders/P3_Ojos_vertex.glsl");
   idProg_P4_lata          = LoadShaders("src/shaders/P4_lata_fragment.glsl","src/shaders/P4_lata_vertex.glsl");
   idProg_P4_tapas         = LoadShaders("src/shaders/P4_tapas_fragment.glsl","src/shaders/P4_tapas_vertex.glsl");
   idProg_P4_peon_madera   = LoadShaders("src/shaders/P4_peon_madera_fragment.glsl","src/shaders/P4_peon_madera_vertex.glsl");
   idProg_P4_peon_blanco   = LoadShaders("src/shaders/P4_peon_blanco_fragment.glsl","src/shaders/P4_peon_blanco_vertex.glsl");
   idProg_P4_peon_negro    = LoadShaders("src/shaders/P4_peon_negro_fragment.glsl","src/shaders/P4_peon_negro_vertex.glsl");
*/

   // habilitar test de comparación de profundidades para 3D (y 2D)
   // es necesario, no está habilitado por defecto:
   // https://www.opengl.org/wiki/Depth_Buffer
   glEnable( GL_DEPTH_TEST );
   glDepthFunc(GL_LESS);
   
   // establecer color de fondo: (1,1,1) (blanco)
   glClearColor( 1.0, 1.0, 1.0, 1.0 ) ;
   
   
   // inicializar parámetros del frustum
   frustum_dis_del         = 0.1 ;
   frustum_dis_tra         = 10.0;
   frustum_ancho           = 0.5*frustum_dis_del ;
   frustum_factor_escala   = 1.0 ;
   
   // inicializar parámetros de la cámara
   camara_angulo_x = 0.0 ;
   camara_angulo_y = 0.0 ;

   // ??
   FijarViewportProyeccion() ;
   FijarCamara() ;
   pila_opengl.push(Modelado);
   
   
   // ya está
   CError();
}

void Inicializar_Practicas(int argc, char *argv[])
{
   practica1 = new Practica1();
   practica2 = new Practica2();
   practica3 = new Practica3();
   practica4 = new Practica4();


   practica1->Inicializar(argc, argv);
   practica2->Inicializar(argc, argv);
   practica3->Inicializar(argc, argv);
   practica4->Inicializar(argc, argv);

   practicaActual = practica1;
}

void Inicializa_VAO()
{
   glGenVertexArrays(1, &id_VAO);
   glBindVertexArray(id_VAO);

   glGenBuffers(1, &id_VBO_Vertices_Ejes);
   glBindBuffer(GL_ARRAY_BUFFER,id_VBO_Vertices_Ejes);
   glBufferData(GL_ARRAY_BUFFER, sizeof(ejes), ejes, GL_STATIC_DRAW);

   glGenBuffers(1, &id_VBO_Colores_Ejes);
   glBindBuffer(GL_ARRAY_BUFFER,id_VBO_Colores_Ejes);
   glBufferData(GL_ARRAY_BUFFER, sizeof(color_ejes), color_ejes, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

}

// ---------------------------------------------------------------------
// Código de inicialización (llama a los dos anteriores, entre otros)

void Inicializar( int argc, char *argv[] )
{
   cout << "Inicializo GLUT" << endl;
   // glut (crea la ventana)
   Inicializa_GLUT(argc, argv) ;
   

   cout << "Inicializo OpenGL" << endl;
   // opengl: define proyección y atributos iniciales
   Inicializa_OpenGL() ;
   
   cout << "Inicializo VAO" << endl;
   // Inicializar VAO (contendrá ejes y cada cosa que requieran las prácticas)
   Inicializa_VAO();

   cout << "Inicializado todo" << endl;

   // inicializar prácticas
   Inicializar_Practicas(argc, argv);
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
