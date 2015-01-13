#ifndef IDS_SHADER_HPP
#define IDS_SHADER_HPP

#include <GL/glew.h>


// ID del par fragment-vertex shader
extern GLuint idProg_Ejes, idProg_P1_P2;
extern GLuint idProg_P3_Android, idProg_P3_Ojos;
extern GLuint idProg_P4_lata, idProg_P4_tapas, idProg_P4_peon_madera, idProg_P4_peon_blanco, idProg_P4_peon_negro;
extern GLuint idProg_Comun;

extern GLuint idProg_actual;

bool UsarPrograma(GLuint idProg);
GLint ObtenerLocalizacionUniform(GLint idProg, const char * cadena);

#endif
