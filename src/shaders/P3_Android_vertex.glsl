#version 410

// Entrada de datos
layout(location = 0) in vec3 posicion_vertice;
layout(location = 1) in vec3 color_vertice;
layout(location = 2) in vec3 normal_vertice;

//out vec3 color_fragment;
out vec4 normalPunto;
out vec4 posicionPunto;
out vec3 vectorObservador;

uniform mat4 Proyeccion;
uniform mat4 Vista;
uniform mat4 Modelado;


void main()
{

    normalPunto = Vista * Modelado * vec4(normal_vertice,0.0);
    posicionPunto = Vista * Modelado * vec4(posicion_vertice,1.0);
    vectorObservador = -posicionPunto.xyz;

    gl_Position = Proyeccion * Vista * Modelado * vec4(posicion_vertice,1.0);
    
    //color_fragment = normal_vertice;
}