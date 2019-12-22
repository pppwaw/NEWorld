#version 330

uniform sampler2D Texture;
in vec2 FragUV;
in float FragShade;
out vec4 Out_Color;

void main(){
   Out_Color = texture(Texture, FragUV.st) * FragShade;
}
