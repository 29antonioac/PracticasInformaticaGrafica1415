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
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->tamX(), img->tamY(), GL_RGB, GL_UNSIGNED_BYTE, img->leerPixels());
}

void Textura::Activar()
{
   //glEnable(GL_TEXTURE_2D);
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->tamX(), img->tamY(), GL_RGB, GL_UNSIGNED_BYTE, img->leerPixels());

   if (modo_generacion_coordenadas_textura == 0)
   {
      // Desactivada generación automática, usar tabla de coordenadas de textura
   }
   else if (modo_generacion_coordenadas_textura == 1) // Generación de coordenadas en coordenadas de objeto
   {
      std::cout << "Activo textura" << std::endl;
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

      glTexGenfv(GL_S, GL_OBJECT_PLANE, cs);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, ct);
   }
   else     // Generación de coordenadas en coordenadas de ojo
   {
      glEnable(GL_TEXTURE_GEN_S);
      glEnable(GL_TEXTURE_GEN_T);
      glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

      glTexGenfv(GL_S, GL_EYE_PLANE, cs);
      glTexGenfv(GL_T, GL_EYE_PLANE, ct);
   }
   glBindTexture(GL_TEXTURE_2D, id_textura);
}
