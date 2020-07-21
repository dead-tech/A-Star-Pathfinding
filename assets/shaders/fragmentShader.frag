#version 450 core
out vec4 FragColor;
uniform bool startNode;
uniform bool endNode;
void main()
{
    if(startNode){
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    } else if (endNode) {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    } else {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
};