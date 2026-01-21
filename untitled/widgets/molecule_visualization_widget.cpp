#include "widgets/molecule_visualization_widget.h"


Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{}

Molecule_visualization_widget::~Molecule_visualization_widget()
{}

void Molecule_visualization_widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Molecule_visualization_widget::initializeGL()
{
        //set buffers and shit
        initializeOpenGLFunctions();

        //create VAO - vertex array object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //create buffer object and bind it to GL_ARRAY_BUFFER and save data to it
        glGenBuffers(1,&VBO);
        float wierzcholki[] ={
           -0.5f,  -0.5f,   0.0f,
            0.5f,   -0.5f,   0.0f,
            0.0f,   0.5f,   0.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(wierzcholki),wierzcholki,GL_STATIC_DRAW);

        //debuggin variables
        int succes;
        char error_info[512];

        //get vertex shader source, change it to *qstring and compile it
        QString shader_temp_source;
        QFile file(":/resources/shaders/testShader.vert");
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
        file.setFileName(":/resources/shaders/testShader2.fsh");
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
            qDebug() << "error nig: " << error_info;
        }

        //create shader program
        shader_program = glCreateProgram();

        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        //delete shader files
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);


        //set buffer attributres (how should a shader read the shit in buffer)
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

        //unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
}


void Molecule_visualization_widget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES,0,3);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

}
