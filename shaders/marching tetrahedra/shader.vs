#version 410
  
layout(location = 0) in vec3 Vertex; //object space vertex position
layout(location = 1) in vec3 Normal; //object space vertex normal

//uniform
uniform mat4 MVP;  //combined modelview projection matrix
uniform mat4 gWorldTrans;

// out vec3 vPosition;
// out vec3 vNormal;

void main()
{  
	//get the clipspace vertex position
	gl_Position = MVP*vec4(Vertex, 1);
	// vNormal     = mat3(transpose(inverse(gWorldTrans))) * Normal;
	// vPosition = vec3(gWorldTrans * vec4(Vertex, 1.0));
}