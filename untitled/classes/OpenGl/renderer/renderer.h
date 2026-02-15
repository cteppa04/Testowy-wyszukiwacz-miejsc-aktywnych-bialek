#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>

class Renderer : private QOpenGLFunctions_3_3_Core
{
public:
    Renderer(QVector<float>* verticies,QVector<uint>* indices);
    ~Renderer();
    GLuint VAO,VBO,EBO;
};

#endif // RENDERER_H
