#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>

class Renderer : private QOpenGLFunctions_3_3_Core
{
public:
    Renderer(QVector<float>* verticies,QVector<uint>* indices,QVector<float>* normals);
    ~Renderer();
    GLuint VBO,EBO,VBO_normals;
};

#endif // RENDERER_H
