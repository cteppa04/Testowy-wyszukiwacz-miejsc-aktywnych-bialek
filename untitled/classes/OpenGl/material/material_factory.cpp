#include "material_factory.h"


Material *Material_factory::smooth_material(glm::vec3 color, float transparency)
{
    Material* material = new Material();
    material->color = color;
    material->transparency = transparency;

    return material;
}
