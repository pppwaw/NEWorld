#version 330
layout(shared, row_major) uniform;
uniform mat4 MVP;
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCrood;
layout (location = 2) in float Shade;
out vec2 FragUV;
out float FragShade;

void main() {
    FragUV = TexCrood;
    FragShade = Shade;
    gl_Position = vec4(Position, 1.0) * MVP;
}
