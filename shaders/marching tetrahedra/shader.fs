#version 410

uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specLight;
uniform float specPower;

in vec3 vNormal;
in vec3 vPosition;

uniform vec3 lightSrc;
uniform vec3 camPos;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

// ------------------- Ambient Lighting -------------------
	vec3 ambCol;
	ambCol			= lightColor * ambientIntensity;

// ------------------- Diffuse Lighting -------------------
	vec3 norm 		= normalize(vNormal);
	vec3 lightDir 	= normalize(lightSrc-vPosition);
	float diffFac 	= max(dot(norm, -lightDir), 0.0);
	vec3 diffCol 	= lightColor * diffFac;

// ------------------- Specular Lighting -------------------
	vec3 viewDir 	= normalize(camPos - vPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec 		= pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specCol 	= spec * lightColor;
// --------------------------------------------------------


    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor 	= vec4(vec3(1-vPosition.z/6) * (ambCol+diffCol+specCol) , 1.0);
}