#include "renderer.h"


Renderer::Renderer(QVector<float> *verticies, QVector<uint> *indices)
{
    initializeOpenGLFunctions();

    //generate buffers
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    //Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,verticies->size() * sizeof(float),verticies->data(),GL_STATIC_DRAW);

    //Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices->size() * sizeof(GLuint),indices->data(),GL_STATIC_DRAW);

    //unbind buffers

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // optional

}

Renderer::~Renderer()
{
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}
