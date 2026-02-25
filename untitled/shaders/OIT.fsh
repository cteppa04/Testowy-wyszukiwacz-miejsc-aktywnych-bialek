#version 330 core

in vec4 vertexColor;

layout (location = 0) out vec4 accumColor;
layout (location = 1) out float accumReveal;

void main()
{

    float alpha = vertexColor.a;

    if(alpha <= 0.0) discard;

    accumColor = vec4(vertexColor.rgb * alpha,alpha);
    accumReveal = alpha;

}
