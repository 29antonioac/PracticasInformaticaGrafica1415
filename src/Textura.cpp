#include "textura.hpp"
#include <cassert>
#include <GL/glut.h>

#include <iostream>

Textura::Textura(string archivo, unsigned modo_generacion_coordenadas_textura, float cs[4], float ct[4])
{
   assert (modo_generacion_coordenadas_textura == 0 || modo_generacion_coordenadas_textura == 1 || modo_generacion_coordenadas_textura == 2);
   this->img = new jpg::Imagen(archivo.c_str());
   this->modo_generacion_coordenadas_textura = modo_generacion_coordenadas_textura;

   for (unsigned i = 0; i < 4; i++)
   {
      this->cs[i] = cs[i];
      this->ct[i] = ct[i];
   }

   glGenTextures(1, &id_textura);
   glBindTexture(GL_TEXTURE_2D, id_textura);
   // Crear mipmaps de textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->tamX(), img->tamY(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->leerPixels());
   // Generar texturas
   if (modo_generacion_coordenadas_textura == 0)
   {
      // Desactivada generación automática, usar tabla de coordenadas de textura
   }
   else if (modo_generacion_coordenadas_textura == 1) // Generación de coordenadas en coordenadas de objeto
   {
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

      glTexGenfv(GL_S, GL_OBJECT_PLANE, cs);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, ct);

   }
   else     // Generación de coordenadas en coordenadas de ojo
   {
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

      glTexGenfv(GL_S, GL_EYE_PLANE, cs);
      glTexGenfv(GL_T, GL_EYE_PLANE, ct);
   }
   //--------------------
   glBindTexture(GL_TEXTURE_2D, 0);
}

Textura::~Textura()
{
   glDeleteTextures(1, &id_textura);
}

bool Textura::Activar()
{
   glEnable(GL_TEXTURE_2D);

   glBindTexture(GL_TEXTURE_2D, id_textura);

   if (modo_generacion_coordenadas_textura == 0)
   {
      glDisable(GL_TEXTURE_GEN_S);
      glDisable(GL_TEXTURE_GEN_T);
      return true;
   }
   else
   {
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
      return false;
   }
}

unsigned Textura::getModo()
{
   return modo_generacion_coordenadas_textura;
}

GLuint Textura::getID()
{
   return this->id_textura;
}
