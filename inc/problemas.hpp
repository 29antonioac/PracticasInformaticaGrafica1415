#ifndef __IG_PROBLEMAS_HPP__
#define __IG_PROBLEMAS_HPP__

#include <vector>

#include "MallaTVT.hpp"
#include "practica.hpp"

class Problemas : virtual public Practica
{
public:

   void DibujarObjetos();
   void Inicializar(int argc, char * argv[]);
   void EnciendeLuces();
   void CambioModoDibujo(visualizacion modo_dibujo);
   void CambioModoNormales();
   void CambioColorFijo();
   bool GestionarEvento(unsigned char tecla);
   void ClickRaton(int x, int y);
   void Debug();
   void Ayuda(vector<string> & strings_control);

   void FijarCamara();
   void FijarProyeccion(float ventana_tam_x, float ventana_tam_y);
   void ModificaEjeXCamara(float nuevo);
   void ModificaEjeYCamara(float nuevo);
   void ModificarEscala(int signo);

   virtual ~Problemas();

   Problemas();
   Problemas(Problemas const&);              // No se implementa para evitar copias
   Problemas& operator=(Problemas const&);   // No se implementa para evitar asignaciones

private:

   MallaTVT * malla;

   vector<float> vertices_ply;
   vector<int> caras_ply;

   string file;

   Camara * camara;
};

#endif
