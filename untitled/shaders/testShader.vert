#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 color;

uniform mat4 transformation_matrices[3];

out vec4 vertexColor; // specify a color output to the fragment shader

void main(){


    gl_Position = transformation_matrices[2] * transformation_matrices[1] * transformation_matrices[0] * vec4(aPos, 1.0);
    vertexColor = vec4(color, 1.0);
}
