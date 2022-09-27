#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 1) out vec3 outColor;


void main() {
  outColor = vec3(1,2,3);
}