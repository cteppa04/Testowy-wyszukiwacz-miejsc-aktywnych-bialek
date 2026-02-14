#ifndef SPHERE_OBJECT_H
#define SPHERE_OBJECT_H

#include <classes/OpenGl/geometry/sphere_mesh.h>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class Sphere_object : protected QOpenGLFunctions_3_3_Core
{
public:
    Sphere_object(Sphere_mesh* mesh,glm::vec3 color,GLfloat transparency);
    ~Sphere_object();
    GLuint VAO,VBO,EBO;

    glm::vec3 m_color;
    GLfloat m_transparency;
    GLfloat m_radius;
    Sphere_mesh *m_mesh;
private:
};

#endif // SPHERE_OBJECT_H
