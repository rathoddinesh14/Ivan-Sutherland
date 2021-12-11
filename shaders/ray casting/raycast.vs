#version 410

layout(location = 0) in vec3 vertexPos;
// texture coordinate
smooth out vec3 textureCoord;

// uniform variables
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPos.xyz, 1);

	// Cube vertices are defined in the range [-0.5, 0.5] in each dimension.
	// The texture coordinates are defined in the range [0, 1] in each dimension.
	// The texture coordinates are mapped to the cube vertices by using the
	// following equations:
	textureCoord = vertexPos + vec3(0.5);
}