#include "widgets/molecule_visualization_widget.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QKeyEvent>

float wierzcholki[] ={
    // FRONT (+Z)
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,

    // BACK (-Z)
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,

    // LEFT (-X)
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

    // RIGHT (+X)
    0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

    0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,

    // TOP (+Y)
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,

    // BOTTOM (-Y)
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::mat4 model(1.0f);
glm::mat4 model_base(1.0f);
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);
GLuint uniform_loc = 0;

//keyboard input array
bool keys[256] = {false};

//origin point camera initialization
const float OP_CAMERA_MOVE_SPEED = 0.5f;
const float OP_CAMERA_ROTATION_SPEED = 0.05f;

const glm::vec3 CAMERA_STARTING_POS(0.0f,0.0f,0.0f);

glm::vec3 camera_position(CAMERA_STARTING_POS);
glm::vec3 new_camera_position(camera_position);
glm::vec3 camera_position_delta;

glm::vec3 z_axis = glm::vec3(0.0,0.0,1.0);
glm::vec3 y_axis = glm::vec3(0.0,1.0,0.0);
glm::vec3 x_axis = glm::vec3(1.0,0.0,0.0);

float pitch = 0.0f;
float yaw = 0.0f;
float roll = 0.0f;
float radius = 0.0f;

//function declarations
std::array<float,3> cartesian_to_euler(glm::vec3 position);
glm::vec3 euler_to_cartesian(float pitch, float yaw, float radius);

QElapsedTimer timer;
Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // widget can accept keyboard focus
    setFocus(); // actively give it focus
    timer.start();
}

Molecule_visualization_widget::~Molecule_visualization_widget()
{}

void Molecule_visualization_widget::paintGL()
{

    qDebug() << "Camera position: " << camera_position.x << " " <<  camera_position.y << " " << camera_position.z;
    //origin point camera control
    //radius modification
    if (keys[Qt::Key_W]){
        radius -= OP_CAMERA_MOVE_SPEED;
    }
    if (keys[Qt::Key_S]){
        radius += OP_CAMERA_MOVE_SPEED;
    }
    //make sure the radius is never smaller than 0;
    radius = std::max(0.0f,radius);
    //pitch modification
    if (keys[Qt::Key_D]){
        yaw+= OP_CAMERA_ROTATION_SPEED;
    }
    if (keys[Qt::Key_A]){
        yaw -= OP_CAMERA_ROTATION_SPEED;
    }

    //yaw modification
    if (keys[Qt::Key_E]){
        pitch += OP_CAMERA_ROTATION_SPEED;
    }
    if (keys[Qt::Key_Q]){
        pitch -= OP_CAMERA_ROTATION_SPEED;
    }
    //make sure the pitch angle [up/down] does not exceed 90 degrees
    pitch = glm::clamp(pitch,-89.0f,89.0f);

    qDebug() << "radius: " << radius;
    //calculate new camera position

    new_camera_position = euler_to_cartesian(pitch,yaw,radius);

    camera_position_delta = new_camera_position - camera_position;
    camera_position = new_camera_position;
    view = glm::translate(view,-camera_position_delta);




    //clear colors in bg and enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    //use program
    glUseProgram(shader_program);

    //sent matrices to shader program
        glBindVertexArray(VAO);
        for(unsigned int i = 0; i < 10; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model,cubePositions[i]);
            model = glm::rotate(model,glm::radians(timer.elapsed()/10.f),glm::vec3(0.0,1.0,0.0));
            glm::mat4 transformation_matrices[3] = {model,view,projection};
            glUniformMatrix4fv(uniform_loc,3,GL_FALSE,glm::value_ptr(transformation_matrices[0]));

            glDrawArrays(GL_TRIANGLES,0,36);
        }

    update();
}

glm::vec3 euler_to_cartesian(float pitch, float yaw, float radius){
    glm::vec3 direction_rad;
    direction_rad.x = std::sin(yaw) * cos(pitch);
    direction_rad.z = std::cos(yaw) * cos(pitch);
    direction_rad.y = std::sin(pitch);
    qDebug() << direction_rad.x << " " << direction_rad.y << " " << direction_rad.z;
    glm::vec3 position = direction_rad * radius;
    return position;
}

std::array<float,3> cartesian_to_euler(glm::vec3 position){
    float radius = glm::length(position);
    float yaw = glm::degrees(std::atan2(position.x,position.z));
    float pitch = glm::degrees(std::atan2(position.y,radius));
    return {yaw,pitch,radius};
}

void Molecule_visualization_widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Molecule_visualization_widget::initializeGL()
{
    //camera inicjalization
    auto euler = cartesian_to_euler(CAMERA_STARTING_POS);
    yaw = euler[0];
    pitch = euler[1];
    radius = euler[2];

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
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //create buffer object and bind it to GL_ARRAY_BUFFER and save data to it
    glGenBuffers(1,&VBO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(wierzcholki),wierzcholki,GL_STATIC_DRAW);

    //set buffer attributres (how should a shader read the shit in buffer)
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //unbind the VBO and VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


    //create transformation matrces
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
    uniform_loc = glGetUniformLocation(shader_program,"transformation_matrices");

}

//keyboard input handling
void Molecule_visualization_widget::keyPressEvent(QKeyEvent *event)
{
    if(!event->isAutoRepeat()){
        keys[event->key()] = true;
    }
}


void Molecule_visualization_widget::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
        keys[event->key()] = false;
}
