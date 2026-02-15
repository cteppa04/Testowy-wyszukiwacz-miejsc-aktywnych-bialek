#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <QVector>

class Mesh
{
public:
    Mesh();

    QVector<float> verticies;
    QVector<uint> indices;
};

#endif // MESH_H
