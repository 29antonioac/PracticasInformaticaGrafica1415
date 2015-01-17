#ifndef TEXTURA_HPP
#define TEXTURA_HPP

#include "jpg_imagen.hpp"
#include "IDs_Shaders.hpp"
#include <GL/glew.h>
#include <string>

using std::string;

class Textura
{
private:
   static unsigned num_texturas;
   GLuint id_textura, location_textura;
   jpg::Imagen * img;
   unsigned modo_generacion_coordenadas_textura;
   float cs[4], ct[4];
public:
   Textura (string archivo, unsigned modo_generacion_coordenadas_textura, float cs[4], float ct[4]);
   ~Textura();
   bool Activar();
   unsigned getModo();
   GLuint getID();
};

#endif
