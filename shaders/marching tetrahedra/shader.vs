#version 410
  
layout(location = 0) in vec3 vVertex; //object space vertex position

//uniform
uniform mat4 MVP;  //combined modelview projection matrix

out vec4 pos;

void main()
{  
	//get the clipspace vertex position
	gl_Position = MVP*vec4(vVertex, 1);
	pos = vec4(vVertex, 1);
}