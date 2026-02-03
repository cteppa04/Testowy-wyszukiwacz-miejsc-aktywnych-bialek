#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
class Sphere
{
public:
    Sphere();
    glm::vec3 position;
    glm::vec3 color;
    float transparency = 1.0f;
    unsigned int VAO, VBO;

};

#endif // SPHERE_H
