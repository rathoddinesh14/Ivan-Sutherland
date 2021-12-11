#version 410

out vec4 fragCol;
smooth in vec3 textureCoord;

//uniform variables
uniform sampler3D volumeTexture;
uniform vec3 cameraPos;
uniform vec3 rayStepSize;
uniform float isoValue;

//constants
const int MAX_SAMPLES = 300;		//total samples for each ray march step
const float DELTA = 0.001;			//the step size for gradient calculation

// this function returns location of potential iso-point within the volumeTexture
// by using iterative midpoint method
vec3 getPotentialIsoPoint(vec3 a, vec3 b) {
	vec3 midpoint = a + (b - a) / 2.0;
	for(int i = 0; i < 5; i++) {
		float isoSample = texture(volumeTexture, midpoint).r;
		if(isoSample < isoValue)
			a = midpoint;
		else
			b = midpoint;
		midpoint = a + (b - a) / 2.0;
	}
	return midpoint;
}

// this function returns the gradient of the volumeTexture at the input location
// using center finite difference approximation method
vec3 getGradientVector(vec3 location) {
	vec3 a, b;

	// get the gradient in x direction
	a.x = texture(volumeTexture, location + vec3(DELTA, 0.0, 0.0)).x;
	b.x = texture(volumeTexture, location - vec3(DELTA, 0.0, 0.0)).x;

	// get the gradient in y direction
	a.y = texture(volumeTexture, location + vec3(0.0, DELTA, 0.0)).x;
	b.y = texture(volumeTexture, location - vec3(0.0, DELTA, 0.0)).x;

	// get the gradient in z direction
	a.z = texture(volumeTexture, location + vec3(0.0, 0.0, DELTA)).x;
	b.z = texture(volumeTexture, location - vec3(0.0, 0.0, DELTA)).x;

	return normalize((b - a) / (2.0 * DELTA));
}

// this function returns the color of the volumeTexture at the input location
// applying the phong lighting model, light vector(L), view vector(V), and
// normal vector(N)
vec4 Phong(vec3 L, vec3 N, vec3 V, float specPow, vec3 diffCol) {
	vec3 R = reflect(-L, N);
	float diffuse = max(dot(N, L), 0.0);
	float specular = pow(max(dot(R, V), 0.0), specPow);
	return vec4(diffCol * diffuse + specular, 1.0);
}

void main() {
	// default color with alpha = 0
	fragCol = vec4(1.0, 0.0, 0.0, 0.0);

	// local variable for texture coordinate
	vec3 dataPos = textureCoord;

	// ray marching direction and step vector along the ray
	vec3 rayDirection = normalize((textureCoord - vec3(0.5)) - cameraPos);
	vec3 rayStep = rayDirection * rayStepSize;

	//for all samples along the ray
	for(int i = 0; i < MAX_SAMPLES; i++) {
		// advance ray by rayStep
		dataPos = dataPos + rayStep;

		// validate the data position (within the volume texture)
		if(	dataPos.x < 0.0 || dataPos.x > 1.0 ||
			dataPos.y < 0.0 || dataPos.y > 1.0 ||
			dataPos.z < 0.0 || dataPos.z > 1.0)
			break;

		float sample1 = texture(volumeTexture, dataPos).r;
		float sample2 = texture(volumeTexture, dataPos + rayStep).r;

		// zero crossing condition
		if((sample1 - isoValue) <= 0 && (sample2 - isoValue) > 0) {
			
			vec3 isoPoint = getPotentialIsoPoint(dataPos, dataPos + rayStep);

			// Normal vector
			vec3 N = getGradientVector(isoPoint);					

			// View vector
			vec3 V = -rayDirection;

			// Light vector
			vec3 L = V;

			// fragCol =  PhongLighting(L,N,V, 250, vec3(0.5));
			fragCol = Phong(L, N, V, 250, isoPoint);
			break;
		}
	}
}