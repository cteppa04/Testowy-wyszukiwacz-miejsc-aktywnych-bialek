#ifndef OBJECT_INSTANCE_H
#define OBJECT_INSTANCE_H

#include "classes/OpenGl/objects/object.h"
#include <glm/glm.hpp>
#include <QVector>
#include <QOpenGLFunctions_3_3_Core>

class Object_instance : private QOpenGLFunctions_3_3_Core
{
public:
    Object_instance(Object *object);
    Object *m_object = nullptr;

    int instance_count();
    void add_instance(glm::mat4 model, glm::vec3 color, float opacity);
    void delete_instance(uint index);
    void clear();

    void updateGPU();

    GLuint VAO;
private:
    QVector<glm::mat4> m_models;
    QVector<glm::vec4> m_colors;

    GLuint modelsVBO;
    GLuint colorsVBO;
};

#endif // OBJECT_INSTANCE_H
