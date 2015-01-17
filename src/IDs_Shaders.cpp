#include "IDs_Shaders.hpp"

#include <iostream>
#include <cstdlib>

GLuint idProg_Ejes, idProg_P1_P2;
GLuint idProg_P3_Android, idProg_P3_Ojos;
GLuint idProg_P4_lata, idProg_P4_tapas, idProg_P4_peon_madera, idProg_P4_peon_blanco, idProg_P4_peon_negro;
GLuint idProg_Comun;

GLuint idProg_actual;

void printer(char *name, int value) {
    std::cout << name << " = " << value << std::endl;
}

bool UsarPrograma(GLuint idProg)
{
   if (idProg == idProg_Ejes || idProg == idProg_P1_P2
         || idProg == idProg_P3_Android || idProg == idProg_P3_Ojos
         || idProg == idProg_P4_lata || idProg == idProg_P4_tapas
         || idProg == idProg_P4_peon_madera || idProg == idProg_P4_peon_blanco || idProg == idProg_P4_peon_negro)

   {
      idProg_actual = idProg;
      glUseProgram(idProg_actual);
      return true;
   }
   return false;
}

GLint ObtenerLocalizacionUniform(GLint idProg, const char * cadena)
{
   GLint loc = glGetUniformLocation(idProg, cadena);
   if (loc != -1)
      return loc;
   else
   {
      std::cout << "Problema al obtener " << cadena << " como uniform, saliendo..." << std::endl;
      PRINTER(idProg);
      PRINTER(idProg_actual);
      PRINTER(idProg_P4_lata);
      PRINTER(idProg_P4_peon_madera);
      //glutLeaveMainLoop();
      exit(2);
   }
}
