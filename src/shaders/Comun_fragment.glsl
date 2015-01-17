#version 330 core
#extension GL_ARB_separate_shader_objects : enable

int numero_fuentes = 1;

// Valores interpolados desde el vertex shader
//in vec3 color_fragment;
in vec4 normalPunto;
in vec4 posicionPunto;
in vec3 vectorObservador;

// Valores de entrada constantes para la primitiva
//uniform vec4 vectorPosicionFuente[numero_fuentes];  // vector hacia la fuente de luz w = 0 direccional, w = 1 posicional
uniform vec3 colorFuente;   // color de la fuente de luz
uniform vec3 material[4];  // parametros material: emi, amb, dif, esp, expEsp
uniform float exponenteEspecular;
uniform mat4 Vista;

// Datos de salida
out vec3 color;


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
    vec4 vectorPosicionFuente;
    
    vectorPosicionFuente = vec4(0.0,15.0,0.0,1.0);
    //vectorPosicionFuente[1] = vec4(0.0,-15.0,0.0,1.0);
    
    
    vec3 normal = normalize(normalPunto.xyz);
    vec3 observador = normalize(vectorObservador);
    vec3 color_resultado = vec3(0.0,0.0,0.0);
    
    for (int i = 0; i < numero_fuentes; i++)
    {
	    vec3 vector_hacia_fuente;
	    if ((vectorPosicionFuente).w == 0.0) // Luz direccional
	    {
	        vector_hacia_fuente = (vectorPosicionFuente).xyz;
	    }
	    else
	    {
	        vector_hacia_fuente = vectorPosicionFuente.xyz - posicionPunto.xyz;
	        vec4 temp = Vista * vec4(vector_hacia_fuente,0.0);
	        vector_hacia_fuente = temp.xyz;
	    }   
	    vec3 vector_luz = normalize(vector_hacia_fuente);
	    color_resultado += EvaluarMIL(material, exponenteEspecular,
	                normal, observador, vector_luz, colorFuente);
    }
    

    // color = color interpolado desde el vertex shader
    //color = color_resultado;
    color = vec3(0.5,0.5,0.7);

}