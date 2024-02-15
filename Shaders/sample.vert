#version 330 core

// Gets the data at Attrib Index 0
// Converts it and stores it into a Vec3
layout(location = 0) in vec3 aPos;

uniform mat4 projection;

uniform mat4 view;

uniform mat4 transform;

layout(location = 2) in vec2 aTex;

out vec2 texCoord;

void main() {
	gl_Position = projection * view * transform * vec4(aPos,1.0);
	texCoord= aTex;

}