#version 330 core

// Entrada de datos
layout(location = 0) in vec3 posicion_vertice;
layout(location = 1) in vec3 color_vertice;

// Salida de datos, sera interpolada para cada fragment
out vec3 color_fragment;

// Valores constantes para la primitiva
uniform mat4 MVP;

void main(){    

    // Posicion final del vertice
    gl_Position =  MVP * vec4(posicion_vertice,1);

    // El color de cada vertice sera interpolado para conseguir
    // el color del fragment
    color_fragment = color_vertice;
}