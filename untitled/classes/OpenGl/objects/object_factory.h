#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "classes/OpenGl/material/material.h"
#include <classes/OpenGl/objects/object.h>
#include <classes/OpenGl/geometry/mesh.h>

#include <glm/glm.hpp>

namespace Object_factory
{
    Object* sphere(Mesh* mesh,Material* material,float radius,glm::vec3 position);
};

#endif // OBJECT_FACTORY_H
