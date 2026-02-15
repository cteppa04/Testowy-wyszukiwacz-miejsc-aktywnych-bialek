#ifndef SPHERE_OBJECT_H
#define SPHERE_OBJECT_H

#include <classes/OpenGl/geometry/mesh_factory.h>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class Sphere_object : protected QOpenGLFunctions_3_3_Core
{
public:
    Sphere_object(Mesh *mesh, glm::vec3 position, float radius, glm::vec3 color, GLfloat transparency);
    ~Sphere_object();
    GLuint VAO,VBO,EBO;

    glm::vec3 m_color;
    GLfloat m_transparency;
    GLfloat m_radius;
    const Mesh *m_mesh;
    glm::vec3 m_position;
private:
};

#endif // SPHERE_OBJECT_H
