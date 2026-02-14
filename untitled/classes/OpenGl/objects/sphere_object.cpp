#include "sphere_object.h"
#include <glm/glm.hpp>

Sphere_object::Sphere_object(Sphere_mesh* mesh,glm::vec3 color,GLfloat transparency)
{
    initializeOpenGLFunctions();
    //set transparncy
    m_transparency = transparency;
    //set color
    m_color = color;
    //set mesh
    m_mesh = mesh;

    //generate buffers
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    //bind buffers
    //Bind VAO
    glBindVertexArray(VAO);

    //Bind and fill VBO
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,m_mesh->verticies.size() * sizeof(float),m_mesh->verticies.data(),GL_STATIC_DRAW);

    //Bind and fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_mesh->indices.size() * sizeof(GLuint),m_mesh->indices.data(),GL_STATIC_DRAW);

    //set attributes
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    //unbind buffers

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // optional

}

Sphere_object::~Sphere_object()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
