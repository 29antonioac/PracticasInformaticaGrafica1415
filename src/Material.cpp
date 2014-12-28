#include "Material.hpp"


Material::Material(Tupla3f componente_emision, Tupla3f componente_ambiental,
      Tupla3f componente_difusa, Tupla3f componente_especular, float exponente_especular, Textura * textura)
{
   this->textura = textura;
   this->componente_emision = AniadeW(componente_emision,1.0);
   this->componente_ambiental = AniadeW(componente_ambiental,1.0);
   this->componente_difusa = AniadeW(componente_difusa,1.0);
   this->componente_especular = AniadeW(componente_especular,1.0);
   this->exponente_especular = exponente_especular;
}

bool Material::Activar()
{
   bool respuesta = false;
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, componente_emision.data());
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, componente_ambiental.data());
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, componente_difusa.data());
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, componente_especular.data());
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, exponente_especular);

   if (textura == nullptr)
   {
      glDisable(GL_TEXTURE_2D);
      glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
      glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
      glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
      glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   }
   else
   {
      glEnable(GL_TEXTURE_2D);
      respuesta = textura->Activar();
   }

   return respuesta;
}

bool Material::NecesitoCoordenadasTextura()
{
   if (textura != nullptr && textura->getModo() == 0)
      return true;
   else
      return false;
}
