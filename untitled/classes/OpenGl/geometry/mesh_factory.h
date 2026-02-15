#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include <glm/glm.hpp>
#include <QVector>
#include <classes/OpenGl/geometry/mesh.h>

namespace Mesh_factory
{
    Mesh* Sphere_mesh(uint slices = 3,uint stacks = 1);
};

#endif // MESH_FACTORY_H
