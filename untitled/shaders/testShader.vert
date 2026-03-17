    #version 330 core
    layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
    layout (location = 1) in vec3 aNormal; // the position variable has attribute position 1
    //location 2,3,4,5
    layout (location = 2) in mat4 aModel;
    layout (location = 6) in vec4 aColor;

    uniform mat4 view;
    uniform mat4 projection;
    uniform float scale;
    uniform bool force_opaque = false;
    uniform bool probe = false;

    out vec4 vertexColor; // specify a color output to the fragment shader
    out vec3 vertexNormal;

    void main(){
        gl_Position = projection * view * aModel * vec4(aPos * scale, 1.0);
        vec4 color = aColor;
        if(force_opaque){
            color = vec4(aColor.xyz,1.0);
        }
        vertexColor = color;
        vertexNormal = aNormal;
    }
