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

        //shaders shit
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




        //create VAO - vertex array object
        glGenVertexArrays(2, VAO);
        glBindVertexArray(VAO[0]);

        //create buffer object and bind it to GL_ARRAY_BUFFER and save data to it
        glGenBuffers(2,VBO);
        float wierzcholki[] ={
           -0.875f,  -0.8f,   0.0f,
           -0.75f,   0.0f,   0.0f,
           -0.625f,   -0.8f,   0.0f,

        };
        glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(wierzcholki),wierzcholki,GL_STATIC_DRAW);



        //set buffer attributres (how should a shader read the shit in buffer)
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);

        //unbind the VBO and VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);


        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER,VBO[1]);
        GLfloat test[] = {
            0.0,0.0,0.0,  1.0,0.0,0.0,
            1.0,0.0,0.0,  0.0,1.0,0.0,
            0.75,1.0,0.0,  0.0,0.0,1.0
        };
        glBufferData(GL_ARRAY_BUFFER,sizeof(test),test,GL_STATIC_DRAW);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)(3* sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
}


void Molecule_visualization_widget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(VAO[0]);

    glDrawArrays(GL_TRIANGLES,0,3);


    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glDrawArrays(GL_TRIANGLES,0,3);

}
