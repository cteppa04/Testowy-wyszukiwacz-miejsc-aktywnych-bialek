    #version 330 core
    layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
    //location 1,2,3,4
    layout (location = 1) in mat4 aModel;
    layout (location = 5) in vec4 aColor;
    uniform mat4 view;
    uniform mat4 projection;

    out vec4 vertexColor; // specify a color output to the fragment shader

    void main(){
        gl_Position = projection * view * aModel * vec4(aPos, 1.0);
        vertexColor = aColor;
    }
