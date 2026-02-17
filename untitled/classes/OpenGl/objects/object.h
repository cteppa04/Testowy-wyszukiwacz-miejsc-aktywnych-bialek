#ifndef OBJECT_H
#define OBJECT_H

#include "classes/OpenGl/geometry/mesh.h"
#include "classes/OpenGl/material/material.h"
#include <glm/glm.hpp>
#include <QOpenGLFunctions_3_3_Core>
#include "classes/OpenGl/objects/object_instance.h"
#include <QVector>

class Object
{
public:
    Object();
    Mesh* mesh;
    Material* material;
    QVector<Object_instance> instances;
    void add_instance(Object_instance instance);
private:
};

#endif // OBJECT_H
