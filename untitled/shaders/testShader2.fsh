#version 330 core
in vec3 vertexNormal;
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
/*    if (gl_FrontFacing)
        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // green = front
    else
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); */// red = back
    float ambient_fraction = 0.5;
    float diff = max(dot(normalize(vertexNormal), normalize(vec3(-1.0,1.0,1.0))), 0.0);
    vec3 color = vertexColor.xyz * diff + vertexColor.xyz * ambient_fraction;
    FragColor = vec4(color,vertexColor.a);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
