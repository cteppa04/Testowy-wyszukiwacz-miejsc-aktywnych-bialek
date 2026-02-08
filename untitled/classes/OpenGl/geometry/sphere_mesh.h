#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

#include <glm/glm.hpp>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector>

class Sphere_mesh : protected QOpenGLFunctions_3_3_Core
{
public:
    Sphere_mesh(uint slices = 3,
                uint stacks = 1);

    unsigned int VAO, VBO, EBO;



};

#endif // SPHERE_MESH_H
