#include "material_factory.h"


Material *Material_factory::material(Shader_object *shader)
{
    Material* material = new Material();
    material->m_shader = shader;


    return material;
}
