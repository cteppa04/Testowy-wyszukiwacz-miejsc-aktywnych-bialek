#include "widgets/molecule_visualization_widget.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QKeyEvent>

#include <classes/OpenGl/shader_wrapper.h>
#include <classes/OpenGl/geometry/master.h>

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
QSet<int> keys;

//origin point camera initialization

const glm::vec3 CAMERA_STARTING_POS(0.0f,0.0f,10.0f);

glm::vec3 orbit_camera_orbit_point (0.0f,0.0f,0.0f);

glm::vec3 camera_position(CAMERA_STARTING_POS);

//time declarations
QElapsedTimer timer;


Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // widget can accept keyboard focus
    setFocus(); // actively give it focus

    timer.start();
    //start the camera

    camera.setType(CAMERA_H::Camera::Type::Orbit);
    camera.start(&CAMERA_STARTING_POS);
    camera.set_camera_orbit_point(orbit_camera_orbit_point);

}

Molecule_visualization_widget::~Molecule_visualization_widget()
{
}

void Molecule_visualization_widget::paintGL()
{
    view = camera.update(&keys);

    //clear colors in bg and enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    //use program
    glUseProgram(shader_program.use());

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

void Molecule_visualization_widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Molecule_visualization_widget::initializeGL()
{
    //set buffers and shit
    qDebug() << "OpenGL version:"
             << context()->format().majorVersion()
             << "." << context()->format().minorVersion();
    makeCurrent();
    initializeOpenGLFunctions();

    Sphere_mesh test(4,2);
    shader_program.create_shader(":/resources/shaders/testShader.vert",":/resources/shaders/testShader2.fsh");

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
    uniform_loc = glGetUniformLocation(shader_program.use(),"transformation_matrices");

}

//keyboard key handling
void Molecule_visualization_widget::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        keys.insert(event->key());
    }
}

void Molecule_visualization_widget::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        keys.remove(event->key());
    }
}
