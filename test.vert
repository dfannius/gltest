#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 gWorld;
/* flat */ out vec4 Color;                 // passed to fragment shader

void main()
{
    gl_Position = gWorld * vec4( position, 1.0 );
    // Color = vec4( clamp( position, 0.0, 1.0 ), 1.0 );
    Color = vec4( color, 1.0 );
}
