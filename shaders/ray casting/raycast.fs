#version 410

smooth in vec3 TexCoord;
uniform sampler3D textureUnit;

out vec4 fragColor;

void main()
{
    float volData = texture(textureUnit, TexCoord).r;

    fragColor = vec4(vec3(volData), 1.0);
}