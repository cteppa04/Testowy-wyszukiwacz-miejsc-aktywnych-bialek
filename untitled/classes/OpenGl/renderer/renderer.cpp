#include "renderer.h"

Renderer::Renderer(QVector<float> *verticies, QVector<uint> *indices, QVector<float>* normals)
{
    initializeOpenGLFunctions();

    //generate buffers
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glGenBuffers(1,&VBO_normals);

    //Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,verticies->size() * sizeof(float),verticies->data(),GL_STATIC_DRAW);

    //Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices->size() * sizeof(GLuint),indices->data(),GL_STATIC_DRAW);

    //Bind and fill VBO_normlas
    glBindBuffer(GL_ARRAY_BUFFER,VBO_normals);
    glBufferData(GL_ARRAY_BUFFER,normals->size() * sizeof(float),normals->data(),GL_STATIC_DRAW);
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
    if (VBO_normals){
        glDeleteBuffers(1,&VBO_normals);
        VBO_normals = 0;
    }
}
