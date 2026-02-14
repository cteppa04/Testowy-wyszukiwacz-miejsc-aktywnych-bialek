#include "sphere_mesh.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <classes/position_transforms.h>
#include <glm/glm.hpp>
#include <QDebug>

Sphere_mesh::Sphere_mesh(uint slices, uint stacks)
{

    //generate verticies and indices
    //add first sphere vertex
    verticies.append(QVector<float>{0.0,1.0,0.0});

    //generate middle vertices
    float delta_yaw = glm::radians(360.0) / slices;
    float delta_pitch = glm::radians(180.0) / (stacks + 1);

    float pitch = glm::radians(90.0) - delta_pitch;

    for(uint stack_count = 0; stack_count < stacks;++stack_count){
        float yaw = 0.0;
        //insert first layer vertex
        for(uint slices_count = 0; slices_count < slices; slices_count++ ){
            glm::vec3 position = euler_to_cartesian(pitch,yaw,1.0);
            yaw += delta_yaw;
            verticies.append(position.x);
            verticies.append(position.y);
            verticies.append(position.z);
        }
        pitch -= delta_pitch;
    }

    //append last vertex
    verticies.append(QVector<float>{0.0,-1.0,0.0});



    //calculate indices -> path of each triangle;
    uint top_vertex = 0;
    uint bottom_vertex = verticies.size()/3 - 1;
    //top cap
    for (uint current_slice = 0; current_slice < slices; ++current_slice) {
        indices.append(top_vertex);
        indices.append(current_slice + 1);
        indices.append((current_slice + 1) % slices + 1);
    }
    //middle

    for(uint current_stack = 0;current_stack < stacks - 1;++current_stack){
        uint current_stack_start = current_stack * slices + 1;
        uint next_stack_start = current_stack_start + slices;

        for (uint current_slice = 0; current_slice < slices; ++current_slice) {
            uint a = current_stack_start + current_slice;
            uint b = current_stack_start + (current_slice + 1) % slices;
            uint c = next_stack_start + current_slice;
            uint d = next_stack_start + (current_slice + 1) % slices;

            indices.append(a);
            indices.append(c);
            indices.append(d);

            indices.append(a);
            indices.append(b);
            indices.append(d);

        }
    }

    //bottom cap
    uint last_stack_start = bottom_vertex - slices;
    for (uint current_slice = 0; current_slice < slices; ++current_slice) {
        indices.append(last_stack_start + current_slice);
        indices.append(last_stack_start + (current_slice + 1) % slices);
        indices.append(bottom_vertex);
    }
}

