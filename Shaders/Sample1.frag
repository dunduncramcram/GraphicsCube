#version 330 core

out vec4 FragColor;

uniform sampler2D tex0;

in vec2 texCoord;


void main () {
               //  r   g    b   a Ranges from 0->1
FragColor = vec4(.5f ,0.0f, 0.0f, 1.0f); 

 FragColor = texture(tex0, texCoord);

}