#include "Material.hpp"

#include <glm/gtc/type_ptr.hpp>


Material::Material(glm::vec3 componente_emision, glm::vec3 componente_ambiental,
      glm::vec3 componente_difusa, glm::vec3 componente_especular, float exponente_especular, Textura * textura)
{
   this->textura = textura;
   this->componente_emision = glm::vec4(componente_emision,1.0);
   this->componente_ambiental = glm::vec4(componente_ambiental,1.0);
   this->componente_difusa = glm::vec4(componente_difusa,1.0);
   this->componente_especular = glm::vec4(componente_especular,1.0);
   this->exponente_especular = exponente_especular;
}

bool Material::Activar()
{
   bool respuesta = false;
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(componente_emision));
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(componente_ambiental));
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(componente_difusa));
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(componente_especular));
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
