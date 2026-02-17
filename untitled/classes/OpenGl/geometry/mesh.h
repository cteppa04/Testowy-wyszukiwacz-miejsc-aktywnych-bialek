#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <QVector>
#include <classes/OpenGl/renderer/renderer.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();
    QVector<float> verticies;
    QVector<uint> indices;
    Renderer* renderer;
};

#endif // MESH_H
