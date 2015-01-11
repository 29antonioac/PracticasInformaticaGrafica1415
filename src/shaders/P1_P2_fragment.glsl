#version 330 core

// Valores interpolados desde el vertex shader
//in vec3 color_fragment;
in vec4 normalPunto;
in vec4 posicionPunto;
in vec3 vectorObservador;

uniform mat4 Vista;

// Datos de salida
out vec3 color;

//void FGE (in float exponente_brillo, in vec3 vectorNormal, in vec3 vectorObservador, in vec3 vectorFuente, out float resultado)
//{
//    resultado = pow(max(dot(normalize(vectorObservador + vectorFuente), vectorNormal),0.0),exponente_brillo);
//}

float FGE (float exponente_brillo, vec3 vectorNormal, vec3 vectorObservador, vec3 vectorFuente)
{
    return pow(max(dot(normalize(vectorObservador + vectorFuente), vectorNormal),0.0),exponente_brillo);
}

vec3 EvaluarMIL( vec3 material[4], float exponente_brillo,
                vec3 vectorNormal, vec3 vectorObservador, vec3 vectorFuente, vec3 colorFuente)
{
    vec3 colorEmision = material[0];
    vec3 colorAmbiental = colorFuente * material[1];
    vec3 colorDifuso = colorFuente * material[2];
    vec3 colorEspecular = colorFuente * material[3];
    
    vec3 resultado = colorEmision + colorAmbiental;
    float nl = dot(vectorNormal,vectorFuente);
    if (nl > 0.0)
        resultado += colorDifuso*nl + colorEspecular*FGE(exponente_brillo, vectorNormal, vectorObservador, vectorFuente);
        
    return resultado;
}

void main()
{

        // Tengo que definirlos
    vec3 vectorFuente = vec3(0.0,10.0,0.0) - posicionPunto.xyz; // vector hacia la fuente de luz w = 0
    vec4 temp = Vista * vec4(vectorFuente,0.0);
    vectorFuente = temp.xyz;
    vec3 colorFuente = vec3(1.0,1.0,1.0); // color de la fuente de luz
    vec3 colorMaterial[4]; // colores material 0 = emi 1 = amb 2 = dif 3 = esp
    float exponenteEspecular = 5.0;
    
    colorMaterial[0] = vec3(0.0,0.8,0.0);
    colorMaterial[1] = vec3(0.1,0.1,0.1) * colorMaterial[0];
    colorMaterial[2] = vec3(0.3,0.3,0.3);
    colorMaterial[3] = vec3(0.7,0.7,0.7);
    
    
    // ----------
    vec3 normal = normalize(normalPunto.xyz);
    vec3 observador = normalize(vectorObservador);
    vec3 color_resultado = vec3(0.0,0.0,0.0);
    
    vec3 vector_luz = normalize(vectorFuente);

    color_resultado += EvaluarMIL(colorMaterial, exponenteEspecular,
                normal, observador, vector_luz, colorFuente);
    

    // color = color interpolado desde el vertex shader
    //gl_FragColor = vec4( colorResultante,1.0);
    color = color_resultado;

}