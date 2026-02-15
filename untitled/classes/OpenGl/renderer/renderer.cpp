#include "renderer.h"


Renderer::Renderer(Mesh *mesh)
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
    glBufferData(GL_ARRAY_BUFFER,mesh->verticies.size() * sizeof(float),mesh->verticies.data(),GL_STATIC_DRAW);

    //Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(GLuint),mesh->indices.data(),GL_STATIC_DRAW);

    //set attributes
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //unbind buffers

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // optional

}
