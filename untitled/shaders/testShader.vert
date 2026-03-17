    #version 330 core
    layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
    layout (location = 1) in vec3 aNormal; // the position variable has attribute position 1
    layout (location = 2) in vec4 aColor;
    layout (location = 3) in vec3 aOffset;
    layout (location = 4) in float aScale;

    uniform mat4 view;
    uniform mat4 projection;
    uniform float scale;
    uniform bool force_opaque = false;
    uniform bool probe = false;
    uniform float probe_size = 1.4;
    uniform vec3 probe_color;

    out vec4 vertexColor; // specify a color output to the fragment shader
    out vec3 vertexNormal;

    void main(){
        float final_scale = aScale * scale;

        if(probe){
            final_scale = aScale + probe_size;
        }

        vec3 world_pos = aPos * final_scale + aOffset;
        gl_Position = projection * view * vec4(world_pos,1.0);

        vec4 color = aColor;
        if(force_opaque){
            color = vec4(aColor.xyz,1.0);
        }

        if(probe){
            color = vec4(0.0,0.0,1.0,1.0);
        }
        //color
        vertexColor = color;
        vertexNormal = normalize(aNormal);
    }
