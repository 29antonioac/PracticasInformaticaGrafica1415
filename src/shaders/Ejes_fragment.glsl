#version 410

// Valores interpolados desde el vertex shader
in vec3 color_fragment;

// Datos de salida
out vec3 color;

void main(){

    // color = color interpolado desde el vertex shader
    color = color_fragment;

}