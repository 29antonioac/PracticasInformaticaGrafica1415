#ifndef TEXTURA_HPP
#define TEXTURA_HPP

#include "jpg_imagen.hpp"
#include <GL/gl.h>
#include <string>

using std::string;

class Textura
{
private:
   GLuint id_textura;
   jpg::Imagen * img;
   unsigned modo_generacion_coordenadas_textura;
   float cs[4], ct[4];
public:
   Textura (string archivo, unsigned modo_generacion_coordenadas_textura, float cs[4], float ct[4]);
   void Activar();
};

#endif
