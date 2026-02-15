#include "mesh_factory.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <classes/position_transforms.h>
#include <glm/glm.hpp>
#include <QDebug>

Mesh* Mesh_factory::Sphere_mesh(uint slices, uint stacks)
{
    Mesh* mesh = new Mesh();
    //generate verticies and indices
    //add first sphere vertex
    mesh->verticies.append(QVector<float>{0.0,1.0,0.0});

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
            mesh->verticies.append(position.x);
            mesh->verticies.append(position.y);
            mesh->verticies.append(position.z);
        }
        pitch -= delta_pitch;
    }

    //append last vertex
    mesh->verticies.append(QVector<float>{0.0,-1.0,0.0});


    //calculate indices -> path of each triangle;
    uint top_vertex = 0;
    uint bottom_vertex = mesh->verticies.size()/3 - 1;
    //top cap
    for (uint current_slice = 0; current_slice < slices; ++current_slice) {
        mesh->indices.append(top_vertex);
        mesh->indices.append(current_slice + 1);
        mesh->indices.append((current_slice + 1) % slices + 1);
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

            mesh->indices.append(a);
            mesh->indices.append(c);
            mesh->indices.append(d);

            mesh->indices.append(a);
            mesh->indices.append(b);
            mesh->indices.append(d);

        }
    }

    //bottom cap
    uint last_stack_start = bottom_vertex - slices;
    for (uint current_slice = 0; current_slice < slices; ++current_slice) {
        mesh->indices.append(last_stack_start + current_slice);
        mesh->indices.append(last_stack_start + (current_slice + 1) % slices);
        mesh->indices.append(bottom_vertex);
    }

    mesh->renderer = new Renderer(&mesh->verticies,&mesh->indices);
    return mesh;
}

