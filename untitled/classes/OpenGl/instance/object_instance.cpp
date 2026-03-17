#include "object_instance.h"

Object_instance::Object_instance(Object *object)
{
    initializeOpenGLFunctions();

    m_object = object;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO_offsets);
    glGenBuffers(1,&VBO_scales);
    glGenBuffers(1,&VBO_colors);

    glBindVertexArray(VAO);

    //bind mesh buffers and set attributes
    //VBO verticies
    glBindBuffer(GL_ARRAY_BUFFER,object->mesh->renderer->VBO);
    //attributes
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    //VBO_normals
    glBindBuffer(GL_ARRAY_BUFFER,object->mesh->renderer->VBO_normals);
    //attributes
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,object->mesh->renderer->EBO);

    std::size_t vec4size = sizeof(glm::vec4);
    std::size_t vec3size = sizeof(glm::vec3);
    //VBO_colors
    glBindBuffer(GL_ARRAY_BUFFER,VBO_colors);
    //attributes
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,4,GL_FLOAT,GL_FALSE,vec4size,(void*)0);
    glVertexAttribDivisor(2,1);

    //VBO_offsets
    glBindBuffer(GL_ARRAY_BUFFER,VBO_offsets);
    //attributes
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,vec3size,(void*)0);
    glVertexAttribDivisor(3,1);

    //VBO_scale
    glBindBuffer(GL_ARRAY_BUFFER,VBO_scales);
    //attributes
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,1,GL_FLOAT,GL_FALSE,sizeof(float),(void*)0);
    glVertexAttribDivisor(4,1);

    // 0 - verticies
    // 1 - normals
    // 2 - colors
    // 3 - offset
    // 4 - scale
    glBindVertexArray(0);
}

int Object_instance::instance_count()
{
    return m_colors.size();
}

void Object_instance::add_instance(glm::vec3 offset, float scale, glm::vec3 color, float opacity)
{
    m_offsets.append(offset);
    m_scales.append(scale);
    m_colors.append(glm::vec4(color,opacity));
}

void Object_instance::delete_instance(uint index)
{
    if(index >= m_colors.size()){
        return;
    }
    m_offsets.erase(m_offsets.begin() + index);
    m_scales.erase(m_scales.begin() + index);
    m_colors.erase(m_colors.begin() + index);
}

void Object_instance::clear()
{
    m_offsets.clear();
    m_scales.clear();
    m_colors.clear();
    updateGPU();
}

void Object_instance::updateGPU()
{
    //fill buffers
    //colors
    glBindBuffer(GL_ARRAY_BUFFER,VBO_colors);
    glBufferData(GL_ARRAY_BUFFER,m_colors.size() * sizeof(glm::vec4),m_colors.data(),GL_STATIC_DRAW);
    //offsets
    glBindBuffer(GL_ARRAY_BUFFER,VBO_offsets);
    glBufferData(GL_ARRAY_BUFFER,m_offsets.size() * sizeof(glm::vec3),m_offsets.data(),GL_STATIC_DRAW);
    //scales
    glBindBuffer(GL_ARRAY_BUFFER,VBO_scales);
    glBufferData(GL_ARRAY_BUFFER,m_scales.size() * sizeof(float),m_scales.data(),GL_STATIC_DRAW);

    //end
    glBindBuffer(GL_ARRAY_BUFFER,0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        qDebug() << "OpenGL error:" << err;
}

