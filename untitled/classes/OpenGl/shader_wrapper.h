#ifndef SHADER_WRAPPER_H
#define SHADER_WRAPPER_H

#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <QOpenGLFunctions_3_3_Core>

class Shader_wrapper :protected QOpenGLFunctions_3_3_Core
{
public:
    Shader_wrapper();
    //debugin variables
    int succes;
    char error_info[512];

    unsigned int shader_ID;
    unsigned int create_shader(QString vertex_shader_source,QString fragment_shader_source);

    GLuint use();

    // Helper functions to set uniforms
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setMat4(const std::string &name, const glm::mat4 &mat);
};

#endif // SHADER_WRAPPER_H
