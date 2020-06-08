#version 330 core

in vec2 vTexCoord;

out vec4 colour;

uniform float u_textureWidth;
uniform float u_textureHeight;
uniform vec4 u_tint;

uniform sampler2D u_Texture;

void main() {
    float x = vTexCoord.x / u_textureWidth;
    float y = vTexCoord.y / u_textureHeight;
	vec4 texColour = texture(u_Texture, vec2(x, y));
    texColour.r += u_tint.r * u_tint.a;
    texColour.g += u_tint.g * u_tint.a;
    texColour.b += u_tint.b * u_tint.a;
	colour = texColour;
}
