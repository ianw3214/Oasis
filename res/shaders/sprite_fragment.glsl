#version 330 core

in vec2 vTexCoord;

out vec4 colour;

uniform float u_textureWidth;
uniform float u_textureHeight;

uniform sampler2D u_Texture;

void main() {
    float x = vTexCoord.x / u_textureWidth;
    float y = vTexCoord.y / u_textureHeight;
	vec4 texColour = texture(u_Texture, vec2(x, y));
	colour = texColour;
}
