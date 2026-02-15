#include "object_factory.h"
#include <glm/gtc/matrix_transform.hpp>

Object *Object_factory::sphere(Mesh *mesh, Material *material, float radius, glm::vec3 position)
{
    Object* object = new Object();
    //fill object variables
    object->mesh = mesh;
    object->material = material;
    object->renderer = new Renderer(mesh);

    //create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,position);
    model = glm::scale(model,glm::vec3(radius));
    object->model = model;

    return object;
}
