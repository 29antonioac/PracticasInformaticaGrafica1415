#ifndef VISUALIZACION_HPP
#define VISUALIZACION_HPP

#include <string>
using std::string;

enum visualizacion {ALAMBRE,SOLIDO,AJEDREZ,PUNTOS, SOLIDO_CARAS};
enum normales {NADA,CARAS,VERTICES,AMBAS};
enum figuras {NINGUNA,ESFERA,CUBO,CONO,CILINDRO,TORO,DODECAEDRO,OCTAEDRO,TETRAEDRO,ICOSAEDRO,TETERA};

inline const string enumToString(visualizacion v)
{
   switch(v)
   {
   case ALAMBRE:
      return "Alambre";
      break;
   case SOLIDO:
      return "Solido";
      break;
   case AJEDREZ:
      return "Ajedrez";
      break;
   case PUNTOS:
      return "Puntos";
      break;
   case SOLIDO_CARAS:
      return "Solido con atributos de caras";
      break;
   default:
      return "Enumerado no válido (modo de dibujo)";
      break;
   }
}

inline const string enumToString(normales n)
{
   switch(n)
   {
   case NADA:
      return "Ninguna";
      break;
   case CARAS:
      return "Caras";
      break;
   case VERTICES:
      return "Vertices";
      break;
   case AMBAS:
      return "Ambas";
      break;
   default:
      return "Enumerado no válido (normales)";
      break;
   }
}

#endif
