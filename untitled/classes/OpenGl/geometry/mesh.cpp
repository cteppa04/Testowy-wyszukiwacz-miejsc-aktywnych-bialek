#include "mesh.h"

Mesh::Mesh() {}

Mesh::~Mesh()
{
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
}
