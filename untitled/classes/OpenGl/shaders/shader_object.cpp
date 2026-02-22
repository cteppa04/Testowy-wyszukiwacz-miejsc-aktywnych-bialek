#include "shader_object.h"
#include <QFile>
#include <QString>
#include <QOpenGLFunctions_3_3_Core>

Shader_object::~Shader_object()
{
    if(shader_ID != 0){
        glDeleteProgram(shader_ID);
        shader_ID = 0;
    }
}

Shader_object::Shader_object(QString vertex_shader_source, QString fragment_shader_source)
{
    initializeOpenGLFunctions();

    QString shader_temp_source;
    //vertex shader initialization
    QFile file(vertex_shader_source);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug("No vertex shader idiot");
    }else{
        shader_temp_source = file.readAll();
        file.close();
    }

    QByteArray temp;
    temp = shader_temp_source.toUtf8();
    const char* shader_source;
    shader_source = temp.constData();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,1,&shader_source,NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&succes);
    if(!succes){
        glGetShaderInfoLog(vertex_shader,512,NULL,error_info);
        qDebug() << "error nig: " << error_info;
    }

    //same with fragment shader
    file.setFileName(fragment_shader_source);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug("No fragment shader idiot");
    }else{
        shader_temp_source = file.readAll();
        file.close();
    }

    temp = shader_temp_source.toUtf8();
    shader_source = temp.constData();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&shader_source,NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&succes);
    if(!succes){
        glGetShaderInfoLog(fragment_shader,512,NULL,error_info);
        qDebug() << "error nigger: " << error_info;
    }

    //create shader program
    shader_ID = glCreateProgram();

    glAttachShader(shader_ID, vertex_shader);
    glAttachShader(shader_ID, fragment_shader);
    glLinkProgram(shader_ID);

    glGetProgramiv(shader_ID, GL_LINK_STATUS, &succes);
    if (!succes) {
        glGetProgramInfoLog(shader_ID, 512, NULL, error_info);
        qDebug() << "Program link error: " << error_info;
    }

    //delete shader files
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader_object::use()
{
    glUseProgram(shader_ID);
}

GLint Shader_object::get_uniform_location(const std::string &name)
{
    if(uniform_cache.find(name) != uniform_cache.end()){
        return uniform_cache[name];
    }

    GLint location = glGetUniformLocation(shader_ID,name.c_str());
    uniform_cache[name] = location;
    return location;
}

void Shader_object::set_bool(const std::string &name, bool value)
{
    glUniform1i(get_uniform_location(name),value);
}

void Shader_object::set_int(const std::string &name, int value)
{
    glUniform1i(get_uniform_location(name),value);
}

void Shader_object::set_float(const std::string &name, float value)
{
    glUniform1f(get_uniform_location(name),value);
}

void Shader_object::set_vec3(const std::string &name, const glm::vec3 &value)
{
    glUniform3fv(get_uniform_location(name),1,glm::value_ptr(value));
}

void Shader_object::set_mat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(get_uniform_location(name),1,GL_FALSE,glm::value_ptr(mat));
}
