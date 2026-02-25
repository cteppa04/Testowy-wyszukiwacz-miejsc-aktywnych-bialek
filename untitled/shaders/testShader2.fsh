#version 330 core

in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
