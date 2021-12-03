#version 410

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;

uniform mat4 MVP;

// texture coordinates
smooth out vec3 TexCoord;

void main()
{
	gl_Position = MVP*vec4(Vertex, 1);

	TexCoord = Vertex;
}