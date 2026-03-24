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
    void add_instance(glm::vec3 offset, glm::vec3 scale,glm::mat4 rotation, glm::vec3 color, float opacity);
    void delete_instance(uint index);
    void clear();

    void updateGPU();

    GLuint VAO;
private:
    QVector<glm::vec3> m_offsets;
    QVector<glm::vec3> m_scales;
    QVector<glm::vec4> m_colors;
    QVector<glm::mat4> m_rotations;

    GLuint VBO_offsets;
    GLuint VBO_scales;
    GLuint VBO_colors;
    GLuint VBO_rotations;
};

#endif // OBJECT_INSTANCE_H
