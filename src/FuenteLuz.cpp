#include "FuenteLuz.hpp"

#include <GL/gl.h>


#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

unsigned FuenteLuz::numero_fuentes = 0;

FuenteLuz::FuenteLuz(glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular)
{
   this->componente_ambiental = glm::vec4(componente_ambiental,1.0);
   this->componente_difusa = glm::vec4(componente_difusa,1.0);
   this->componente_especular = glm::vec4(componente_especular,1.0);
   this->id_luz = GL_LIGHT0 + numero_fuentes;
   numero_fuentes++;
}

FuenteLuzPosicional::FuenteLuzPosicional(glm::vec3 posicion, glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->posicion = glm::vec4(posicion,1.0);
}

FuenteLuzDireccional::FuenteLuzDireccional(float alpha, float beta, glm::vec3 componente_ambiental, glm::vec3 componente_difusa, glm::vec3 componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->alpha = alpha;
   this->beta = beta;
}

void FuenteLuzDireccional::Activar()
{
   glEnable(id_luz);

   const float ejeZ[4] = {0.0,0.0,1.0,0.0};

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   	   //glLoadIdentity();

   	   glRotatef(this->alpha, 0.0,1.0,0.0);
   	   glRotatef(this->beta, -1.0,0.0,0.0);
   	   glLightfv(id_luz, GL_POSITION, ejeZ);
   	   glLightfv(id_luz, GL_AMBIENT, glm::value_ptr(componente_ambiental));
         glLightfv(id_luz, GL_DIFFUSE, glm::value_ptr(componente_difusa));
         glLightfv(id_luz, GL_SPECULAR,glm::value_ptr(componente_especular));

   glPopMatrix();
}

void FuenteLuzDireccional::ModificaAlpha(int signo)
{
	assert(signo == 1 || signo == -1);
	//if (alpha > 2*M_PI) alpha -= 2*M_PI;
	//else if (alpha < -2*M_PI) alpha += 2*M_PI;
	alpha += signo*M_PI/6;
}

void FuenteLuzDireccional::ModificaBeta(int signo)
{
	assert(signo == 1 || signo == -1);
	//if (beta > 2*M_PI) beta -= 2*M_PI;
	//else if (beta < -2*M_PI) beta += 2*M_PI;
	beta += signo*M_PI/6;
}

float FuenteLuzDireccional::getAlpha()
{
   return alpha;
}
float FuenteLuzDireccional::getBeta()
{
   return beta;
}

void FuenteLuzPosicional::Activar()
{
   glEnable(id_luz);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

      //glLoadIdentity();

      glLightfv(id_luz, GL_POSITION,glm::value_ptr(posicion));
      glLightfv(id_luz, GL_AMBIENT, glm::value_ptr(componente_ambiental));
      glLightfv(id_luz, GL_DIFFUSE, glm::value_ptr(componente_difusa));
      glLightfv(id_luz, GL_SPECULAR,glm::value_ptr(componente_especular));

   glPopMatrix();
}


void ColeccionFuentesLuz::Activar()
{
   for (unsigned i = 0; i < fuentes.size(); i++)
   {
      fuentes[i]->Activar();
   }
}

void ColeccionFuentesLuz::Agregar(FuenteLuz * fuente)
{
   if (fuentes.size() < 8)
      fuentes.push_back(fuente);
   else
      std::cout << "Ya hay 8 fuentes, que son las máximas!" << std::endl;
}
