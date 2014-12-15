#ifndef TEXTURA_HPP
#define TEXTURA_HPP

#include "jpg_imagen.hpp"

class Textura
{
private:
   GLuint id_textura;
   jpg::Imagen * img;
   unsigned modo_generacion_coordenadas_textura;
   float cs[4], ct[4];
public:
   void Activar();
};

#endif
