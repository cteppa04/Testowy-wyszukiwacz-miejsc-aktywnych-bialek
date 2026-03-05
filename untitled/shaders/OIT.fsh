#version 330 core
layout (location = 0) out vec3 accumColor;
layout (location = 1) out float accumReveal;

in vec4 vertexColor;
uniform float weight;

void main()
{

    float alpha = vertexColor.a;

    if(alpha <= 0.01){
        //wyczysc fragment
        accumColor = vec3(0.0,0.0,0.0);
        accumReveal = 0.0;
    }else{
        //pisz normalnie
        accumColor = vertexColor.rgb * alpha * weight;
        accumReveal = alpha * weight;
    }


}
