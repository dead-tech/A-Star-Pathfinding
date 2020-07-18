#version 450 core
layout(location = 0) in vec3 position;

uniform mat4 scaleTransform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * scaleTransform * vec4(position, 1.0f);
}