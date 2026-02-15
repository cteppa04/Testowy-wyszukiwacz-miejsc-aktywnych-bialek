#ifndef MATERIAL_FACTORY_H
#define MATERIAL_FACTORY_H

#include <classes/OpenGl/material/material.h>
namespace Material_factory
{
    Material* smooth_material(glm::vec3 color,float transparency);
};

#endif // MATERIAL_FACTORY_H
