#ifndef OBJECT_INSTANCE_H
#define OBJECT_INSTANCE_H

#include <glm/glm.hpp>

class Object_instance
{
public:
    Object_instance(float size,glm::vec3 position);
    glm::mat4 m_model;
};

#endif // OBJECT_INSTANCE_H
