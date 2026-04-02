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
            mesh->indices.append(d);
            mesh->indices.append(b);

            mesh->indices.append(a);
            mesh->indices.append(c);
            mesh->indices.append(d);

        }
    }

    //bottom cap
    uint last_stack_start = bottom_vertex - slices;
    for (uint current_slice = 0; current_slice < slices; ++current_slice) {
        mesh->indices.append(last_stack_start + current_slice);
        mesh->indices.append(bottom_vertex);
        mesh->indices.append(last_stack_start + (current_slice + 1) % slices);
    }

    mesh->normals = mesh->verticies;

    mesh->renderer = new Renderer(&mesh->verticies,&mesh->indices,&mesh->normals);
    return mesh;
}


Mesh *Mesh_factory::Line_mesh(uint angle_count)
{
    //line directed in z positive axis

    Mesh *mesh = new Mesh();
    //veritcies
    auto delta_pitch = glm::radians(360.0/angle_count);
    float current_pitch = 0;

    //center point for base filling
    mesh->verticies.append(0);
    mesh->verticies.append(0);
    mesh->verticies.append(0);
    //base points
    for(uint i = 0; i < angle_count; i++){

        auto position = euler_to_cartesian(current_pitch,0,1);
        mesh->verticies.append(position.x);
        mesh->verticies.append(position.y);
        mesh->verticies.append(position.z);

        current_pitch += delta_pitch;
    }

    //ending points
    int vertex_count = mesh->verticies.size() / 3;
    for(int i = 0; i < vertex_count; i ++){
        mesh->verticies.append(mesh->verticies[i*3] + 1.0); //x
        mesh->verticies.append(mesh->verticies[i*3 + 1]); //y
        mesh->verticies.append(mesh->verticies[i*3 + 2]); //z
    }

    //indices
    //first cap
    for(uint i = 0; i < angle_count; i++){
        mesh->indices.append(0);
        mesh->indices.append(i + 1);
        mesh->indices.append((i + 1) % angle_count + 1);
    }

    //walls
    for(uint i = 1;i <= angle_count;i++){ //for each vertex starting at 1 to n skipping central vertex
        uint a = i;
        uint d = (i % angle_count) + 1;
        uint b = a + angle_count + 1;
        uint c = d + angle_count + 1;

        // first triangle
        mesh->indices.append(a);
        mesh->indices.append(b);
        mesh->indices.append(c);

        // second triangle
        mesh->indices.append(a);
        mesh->indices.append(c);
        mesh->indices.append(d);
    }

    //ending cap
    uint start_of_second_cap = angle_count + 1;
    for(uint i = 1; i <= angle_count; i++){;
        uint b = start_of_second_cap + i;
        uint c = ((i) % angle_count) + 1 + start_of_second_cap;
        mesh->indices.append(start_of_second_cap);
        mesh->indices.append(c);
        mesh->indices.append(b);
    }
    //normals


    mesh->normals = mesh->verticies;
    mesh->renderer = new Renderer(&mesh->verticies,&mesh->indices,&mesh->normals);
    return mesh;
}
