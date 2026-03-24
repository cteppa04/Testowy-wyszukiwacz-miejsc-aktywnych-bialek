#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal; // the position variable has attribute position 1
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec3 aScale;
layout (location = 5) in mat4 aRotation; //rotation matrix

uniform mat4 view;
uniform mat4 projection;
uniform float scale;
uniform bool force_opaque;
uniform bool probe;
uniform float probe_size;
uniform vec3 probe_color;

out vec4 vertexColor; // specify a color output to the fragment shader
out vec3 vertexNormal;

void main(){
    //scale hadnling
    vec3 final_scale = aScale * scale;
    if(probe) final_scale += vec3(probe_size);

    //transformation
    vec3 scaled = aPos * final_scale;
    vec3 rotated = mat3(aRotation) * scaled;
    vec3 world_pos = rotated + aOffset;
    gl_Position = projection * view * vec4(world_pos,1.0);

    //color
    vec4 color = aColor;
    if(force_opaque) color.a = 1.0;
    if(probe) color = vec4(probe_color,1);
    vertexColor = color;

    //normals
    vec3 normals = mat3(aRotation) * aNormal;
    vertexNormal = normals;
}
