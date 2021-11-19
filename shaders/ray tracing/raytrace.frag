#version 410

precision highp float;

uniform sampler2D textureUnit;
in vec2 texCoord;

out vec4 fragColor;

void main() {
    fragColor = texture(textureUnit, texCoord);
}