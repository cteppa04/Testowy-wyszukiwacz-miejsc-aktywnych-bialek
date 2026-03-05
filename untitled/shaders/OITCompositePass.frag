#version 330 core
in vec2 uv;

uniform sampler2D textColor;
uniform sampler2D textRevelage;

out vec4 FragColor;

void main(void)
{
    vec3 color = texture(textColor,uv).rgb;

    float alpha = 1 - texture(textRevelage,uv).a;

    FragColor = vec4(color,alpha);
}
