#version 330 core

// Entrada de datos
layout(location = 0) in vec3 posicion_vertice;
layout(location = 1) in vec3 color_vertice;
layout(location = 2) in vec3 normal_vertice;
layout(location = 3) in vec2 coordenadas_textura;

// Salida de datos, sera interpolada para cada fragment
out vec2 UV;
out vec4 normalPunto;
out vec4 posicionPunto;
out vec3 vectorObservador;

// Valores constantes para la primitiva
uniform mat4 Proyeccion;
uniform mat4 Vista;
uniform mat4 Modelado;

void main(){    

    normalPunto = Vista * Modelado * vec4(normal_vertice,0.0);
    posicionPunto = Vista * Modelado * vec4(posicion_vertice,1.0);
    vectorObservador = -posicionPunto.xyz;
    
    // Posicion final del vertice
    gl_Position =  Proyeccion * Vista * Modelado * vec4(posicion_vertice,1);

    // Salida de coordenadas de textura hacia el fragment shader
    UV = coordenadas_textura;

}