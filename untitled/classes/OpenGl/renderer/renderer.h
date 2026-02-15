#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <classes/OpenGl/geometry/mesh.h>

class Renderer : private QOpenGLFunctions_3_3_Core
{
public:
    Renderer(Mesh* mesh);
    GLuint VAO,VBO,EBO;
};

#endif // RENDERER_H
