#include "textura.hpp"
#include <cassert>


#include <iostream>

unsigned Textura::num_texturas = 0;

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
   location_textura = ObtenerLocalizacionUniform(idProg_actual,"textureSampler");

   glBindTexture(GL_TEXTURE_2D, id_textura);
   /*
   // Crear mipmaps de textura
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->tamX(), img->tamY(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->leerPixels());
*/
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->tamX(), img->tamY(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->leerPixels());
   glGenerateMipmap(GL_TEXTURE_2D);  //Generate num_mipmaps number of mipmaps here.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   //--------------------
   glBindTexture(GL_TEXTURE_2D, 0);
}

Textura::~Textura()
{
   glDeleteTextures(1, &id_textura);
}

bool Textura::Activar()
{
   //glEnable(GL_TEXTURE_2D);

   glActiveTexture(GL_TEXTURE0 + num_texturas);

   glBindTexture(GL_TEXTURE_2D, id_textura);

   glUniform1i(location_textura, num_texturas);

   if (modo_generacion_coordenadas_textura == 0)
   {
      //glDisable(GL_TEXTURE_GEN_S);
      //glDisable(GL_TEXTURE_GEN_T);
      return true;
   }
   else
   {
      //glEnable(GL_TEXTURE_GEN_S);
      //glEnable(GL_TEXTURE_GEN_T);
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
