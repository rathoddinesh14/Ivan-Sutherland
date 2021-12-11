#version 410

// uniform float ambientIntensity;
// uniform float diffuseIntensity;
// uniform float specLight;
// uniform float specPower;

// in vec3 vNormal;
// in vec3 vPosition;

// uniform vec3 lightSrc;
// uniform vec3 camPos;

in float z;

vec4 colormap(float z){
	
	if(z < -0.8)
	return vec4(0.168872, 0.250754, 0.015434, 1.0);

	else if(z < -0.6)
	return vec4(0.219751, 0.160845, 0.050890, 1.0);

	else if(z < -0.4)
	return vec4(0.309161,0.188452,0.115882,1.0);

	else if(z < -0.2)
	return vec4(0.388874,0.205646,0.168464,1.0);

	else if(z < 0.0)
	return vec4(0.701541,0.321894,0.286795,1.0);

	else if(z < 0.2)
	return vec4(0.790147,0.352941,0.362754,1.0);

	else if(z < 0.4)
	return vec4(0.878751,0.411952,0.458781,1.0);

	else if(z < 0.6)
	return vec4(0.901965,0.515864,0.584845,1.0);

	else if(z < 0.8)
	return vec4(0.921487,0.557268,0.696742,1.0);

	else if(z <= 1)
	return vec4(0.969001,0.719921,0.829062,1.0);
}

void main()
{
//     vec3 lightColor = vec3(1.0, 1.0, 1.0);

// // ------------------- Ambient Lighting -------------------
// 	vec3 ambCol;
// 	ambCol			= lightColor * ambientIntensity;

// // ------------------- Diffuse Lighting -------------------
// 	vec3 norm 		= normalize(vNormal);
// 	vec3 lightDir 	= normalize(lightSrc-vPosition);
// 	float diffFac 	= max(dot(norm, -lightDir), 0.0);
// 	vec3 diffCol 	= lightColor * diffFac;

// // ------------------- Specular Lighting -------------------
// 	vec3 viewDir 	= normalize(camPos - vPosition);
// 	vec3 reflectDir = reflect(-lightDir, norm);
// 	float spec 		= pow(max(dot(viewDir, reflectDir), 0.0), 32);
// 	vec3 specCol 	= spec * lightColor;
// // --------------------------------------------------------


    //gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    gl_FragColor = colormap(z);
    // gl_FragColor 	= vec4(vec3(1-vPosition.z/6) * (ambCol+diffCol+specCol) , 1.0);
}
