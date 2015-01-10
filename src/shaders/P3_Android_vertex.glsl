#version 330 core

// Entrada de datos
layout(location = 0) in vec3 posicion_vertice_modelado;
layout(location = 1) in vec3 color_vertice;
layout(location = 2) in vec3 normal_vertice_modelado;

// Salida de datos, sera interpolada para cada fragment
out vec3 posicion_coordenadas_mundo;
out vec3 normal_espacio_camara;
out vec3 direccion_ojo_espacio_camara;
out vec3 direccion_luz_espacio_camara;

// Valores constantes para la primitiva
uniform mat4 Proyeccion;
uniform mat4 Vista;
uniform mat4 Modelado;
uniform vec3 posicion_luz_coordenadas_mundo;


void main(){    

    // Posicion final del vertice en MVP
    gl_Position =  Proyeccion * Vista * Modelado * vec4(posicion_vertice_modelado,1);
    
    // Posicion del vertice en coordenadas de mundo M * posicion
    posicion_coordenadas_mundo = (Modelado * vec4(posicion_vertice_modelado,1)).xyz;
    
    // Vector desde el vertice hasta la camara en espacio de camara
    // En espacio de camara, la camara esta en el origen
    vec3 posicion_vertice_espacio_camara = ( Vista * Modelado * vec4(posicion_vertice_modelado,1)).xyz;
    direccion_ojo_espacio_camara = vec3(0,0,0) - posicion_vertice_espacio_camara;
    
    // Vector desde el vertice hasta la luz en coordenadas de camara
    vec3 posicion_luz_espacio_camara = ( Vista * Modelado * vec4(posicion_luz_coordenadas_mundo,1)).xyz;
    direccion_ojo_espacio_camara = posicion_luz_espacio_camara + direccion_ojo_espacio_camara;

    // Normal del vertice en coordenadas de camara
    normal_espacio_camara = ( Vista * Modelado * vec4(normal_vertice_modelado,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    
    // El color de cada vertice sera interpolado para conseguir
    // el color del fragment
    //color_fragment = color_vertice;
}