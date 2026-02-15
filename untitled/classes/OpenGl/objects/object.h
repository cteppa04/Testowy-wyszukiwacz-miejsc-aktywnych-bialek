#ifndef OBJECT_H
#define OBJECT_H

#include "classes/OpenGl/geometry/mesh.h"
#include "classes/OpenGl/material/material.h"
#include <glm/glm.hpp>
#include <QOpenGLFunctions_3_3_Core>

class Object
{
public:
    Object();
    ~Object();
    Mesh* mesh;
    Material* material;
    glm::mat4 model;

private:
};

#endif // OBJECT_H
