#version 330 core

// Valores interpolados desde el vertex shader
in vec3 posicion_coordenadas_mundo;
in vec3 normal_espacio_camara;
in vec3 direccion_ojo_espacio_camara;
in vec3 direccion_luz_espacio_camara;

// Datos de salida
out vec3 color;

// Parametros uniform
uniform vec3 posicion_luz_coordenadas_mundo;

void main(){

    // Emision de luz
    vec3 colorLuz = vec3(1.0,1.0,1.0);
    float potenciaLuz = 50.0f;
    
    // Propiedades del material
    vec3 colorDifuso = vec3(1.0,1.0,1.0);
    vec3 colorAmbiental = vec3(0.1,0.1,0.1) * colorDifuso;
    vec3 colorEspecular = vec3(0.8,0.8,0.8);
    
    // Distancia a la luz
    float distancia = length(posicion_luz_coordenadas_mundo - posicion_coordenadas_mundo);

    // Normal del fragment en coordenadas de camara
    vec3 n = normalize(normal_espacio_camara);
    // Direccion de la luz desde el fragment
    vec3 l = normalize(direccion_luz_espacio_camara);
    // Coseno del angulo entre la normal y la direccion de la luz
    float cosTheta = clamp(dot(n,l),0,1);
    
    // Vector ojo
    vec3 E = normalize(direccion_ojo_espacio_camara);
    // Direccion en la que el triangulo refleja la luz
    vec3 R = reflect(-l,n);
    // Coseno del angulo entre el vector ojo y el vector reflejo
    float cosAlpha = clamp(dot(E,R),0,1);
    
    color = colorAmbiental + 
            colorDifuso * colorLuz * potenciaLuz * cosTheta / (distancia*distancia) +
            colorEspecular * colorLuz * potenciaLuz * pow(cosAlpha,5) / (distancia*distancia);
    
    
    // color = color interpolado desde el vertex shader
    //color = vec3(0.0,0.8,0.0);

}