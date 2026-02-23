#include "object_instance.h"

Object_instance::Object_instance(Object *object)
{
    initializeOpenGLFunctions();

    m_object = object;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&modelsVBO);
    glGenBuffers(1,&colorsVBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,modelsVBO);

    std::size_t vec4size = sizeof(glm::vec4);

    for(int i = 0;i < 4;i++){
        glEnableVertexAttribArray(1+i);
        glVertexAttribPointer(1 + i,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*)(i * vec4size));
        glVertexAttribDivisor(1 + i,1);
    }

    glBindBuffer(GL_ARRAY_BUFFER,colorsVBO);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5,4,GL_FLOAT,GL_FALSE,vec4size,(void*)0);
    glVertexAttribDivisor(5,1);

    glBindVertexArray(0);
}

int Object_instance::instance_count()
{
    return m_models.size();
}

void Object_instance::add_instance(glm::mat4 model, glm::vec3 color, float opacity)
{
    m_models.append(model);
    m_colors.append(glm::vec4(color,opacity));
}

void Object_instance::delete_instance(uint index)
{
    if(index >= m_models.size()){
        return;
    }

    m_models.erase(m_models.begin() + index);
    m_colors.erase(m_colors.begin() + index);
}

void Object_instance::clear()
{
    m_models.clear();
    m_colors.clear();
    updateGPU();
}

void Object_instance::updateGPU()
{
    //fill buffers
    glBindBuffer(GL_ARRAY_BUFFER,modelsVBO);
    glBufferData(GL_ARRAY_BUFFER,m_models.size() * sizeof(glm::mat4),m_models.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,colorsVBO);
    glBufferData(GL_ARRAY_BUFFER,m_colors.size() * sizeof(glm::vec4),m_colors.data(),GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        qDebug() << "OpenGL error:" << err;
}

