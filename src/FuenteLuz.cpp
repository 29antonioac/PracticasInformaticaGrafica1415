#include "FuenteLuz.hpp"

#include <GL/gl.h>

unsigned FuenteLuz::numero_fuentes = 0;

FuenteLuz::FuenteLuz(Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
{
   this->componente_ambiental = AniadeW(componente_ambiental,1.0);
   this->componente_difusa = AniadeW(componente_difusa,1.0);
   this->componente_especular = AniadeW(componente_especular,1.0);
   this->id_luz = GL_LIGHT0 + numero_fuentes;
   numero_fuentes++;
}

FuenteLuzPosicional::FuenteLuzPosicional(Tupla3f posicion, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
   : FuenteLuz::FuenteLuz(componente_ambiental, componente_difusa, componente_especular)
{
   this->posicion = AniadeW(posicion,1.0);
}

FuenteLuzDireccional::FuenteLuzDireccional(float alpha, float beta, Tupla3f componente_ambiental, Tupla3f componente_difusa, Tupla3f componente_especular)
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

      glLightfv(id_luz, GL_POSITION,this->posicion.data());
      glLightfv(id_luz, GL_AMBIENT, this->componente_ambiental.data());
      glLightfv(id_luz, GL_DIFFUSE, this->componente_difusa.data());
      glLightfv(id_luz, GL_SPECULAR,this->componente_especular.data());

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
