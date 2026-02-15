#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
class Material
{
public:
    Material();
    glm::vec3 color;
    float transparency;
};

#endif // MATERIAL_H
