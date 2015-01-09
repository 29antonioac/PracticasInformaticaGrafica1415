#include "IDs_Shaders.hpp"

GLuint idProg_Ejes, idProg_P1_P2;
GLuint idProg_P3_Android, idProg_P3_Ojos;
GLuint idProg_P4;

GLuint idProg_actual;

bool UsarPrograma(GLuint idProg)
{
   if (idProg == idProg_Ejes || idProg == idProg_P1_P2
         || idProg == idProg_P3_Android || idProg == idProg_P3_Ojos
         || idProg == idProg_P4 )

   {
      idProg_actual = idProg;
      glUseProgram(idProg_actual);
      return true;
   }
   return false;
}
