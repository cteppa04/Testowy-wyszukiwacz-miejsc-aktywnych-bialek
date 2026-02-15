#ifndef OBJECT_H
#define OBJECT_H

#include "classes/OpenGl/material/material.h"
#include "classes/OpenGl/renderer/renderer.h"
#include <glm/glm.hpp>
#include <QOpenGLFunctions_3_3_Core>

class Object
{
public:
    Object();
    ~Object();
    Mesh* mesh;
    Renderer* renderer;
    Material* material;
    glm::mat4 model;

private:
};

#endif // OBJECT_H
