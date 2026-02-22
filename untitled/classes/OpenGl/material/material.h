#ifndef MATERIAL_H
#define MATERIAL_H

#include "classes/OpenGl/shaders/shader_object.h"
#include <glm/glm.hpp>

class Material
{
public:
    Material();
    Shader_object* m_shader;
};

#endif // MATERIAL_H
