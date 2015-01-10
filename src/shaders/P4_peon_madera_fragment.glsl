#version 330 core

// Valores interpolados desde el vertex shader
//in vec3 color_fragment;

// Datos de salida
out vec3 color;

void main(){

    // color = color interpolado desde el vertex shader
    color = vec3(0.7,0.3,0.7);

}