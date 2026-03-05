#version 330 core
in vec2 uv;

uniform sampler2D text;

out vec4 FragColor;

void main(void)
{
    vec3 color = texture(text,uv).rgb;

    float alpha = texture(text,uv).a;

    FragColor = vec4(color,alpha);
}
