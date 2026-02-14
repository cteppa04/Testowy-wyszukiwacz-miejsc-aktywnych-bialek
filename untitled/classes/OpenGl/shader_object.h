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
    std::unordered_map<std::string,GLint> uniform_cache;
    GLint get_uniform_location(const std::string &name);
    void set_bool(const std::string &name, bool value);
    void set_int(const std::string &name, int value);
    void set_float(const std::string &name, float value);
    void set_vec3(const std::string &name, const glm::vec3 &value);
    void set_mat4(const std::string &name, const glm::mat4 &mat);
};

#endif // SHADER_OBJECT_H
