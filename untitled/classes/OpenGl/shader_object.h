#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <QOpenGLFunctions_3_3_Core>

class Shader_object :protected QOpenGLFunctions_3_3_Core
{
public:
    Shader_object(QString vertex_shader_source,QString fragment_shader_source);
    ~Shader_object();
    //debugin variables
    int succes;
    char error_info[512];

    unsigned int shader_ID = 0;

    void use();

    // Helper functions to set uniforms
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setMat4(const std::string &name, const glm::mat4 &mat);
};

#endif // SHADER_OBJECT_H
