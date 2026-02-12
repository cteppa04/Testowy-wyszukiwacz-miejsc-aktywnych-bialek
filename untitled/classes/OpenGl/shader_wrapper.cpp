#include "shader_wrapper.h"
#include <QFile>
#include <QString>
#include <QOpenGLFunctions_3_3_Core>

Shader_wrapper::Shader_wrapper() {
}

unsigned int Shader_wrapper::create_shader(QString vertex_shader_source, QString fragment_shader_source)
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

    //delete shader files
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_ID;
}

GLuint Shader_wrapper::use()
{
    return shader_ID;
}
