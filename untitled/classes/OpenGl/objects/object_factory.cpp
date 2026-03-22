#include "object_factory.h"
#include <glm/gtc/matrix_transform.hpp>

Object *Object_factory::Basic(Mesh *mesh, Material *material)
{
    Object* object = new Object();
    //fill object variables
    object->mesh = mesh;
    object->material = material;

    return object;
}
