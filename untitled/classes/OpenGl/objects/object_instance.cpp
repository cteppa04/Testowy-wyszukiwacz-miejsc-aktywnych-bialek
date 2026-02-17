#include "object_instance.h"
#include <glm/gtc/matrix_transform.hpp>

Object_instance::Object_instance(float size, glm::vec3 position)
{
    //create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,position);
    model = glm::scale(model,glm::vec3(size));
    m_model = model;
}
