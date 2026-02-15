#include "renderer.h"


Renderer::Renderer(QVector<float> *verticies,QVector<uint> *indices)
{
    initializeOpenGLFunctions();

    //generate buffers
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    //bind buffers
    //Bind VAO
    glBindVertexArray(VAO);

    //Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,verticies->size() * sizeof(float),verticies->data(),GL_STATIC_DRAW);

    //Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices->size() * sizeof(GLuint),indices->data(),GL_STATIC_DRAW);

    //set attributes
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //unbind buffers

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // optional

}

Renderer::~Renderer()
{
    // Delete OpenGL buffers
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
}
