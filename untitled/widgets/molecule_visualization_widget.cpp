#include "widgets/molecule_visualization_widget.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QTimer>
#include <QKeyEvent>

#include <classes/OpenGl/shader_object.h>
#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/objects/object_master.h>


glm::vec3 Positions[] = {
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
glm::vec3 colors[10];

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

    QTimer *timer_refresh = new QTimer(this);
    connect(timer_refresh, &QTimer::timeout, this, QOverload<>::of(&Molecule_visualization_widget::update));
    timer_refresh->start(16); // ~60 FPS
}


void Molecule_visualization_widget::paintGL()
{
    view = camera.update(&keys);

    //clear colors in bg and enable depth testing
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glClearDepth(1.0f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both color and depth


    //use program
    shader_program->use();

    //sent matrices to shader program
    glBindVertexArray(test->VAO);

    for(unsigned int i = 0; i < 10; i++){
        model = glm::mat4(1.0f);
        model = glm::translate(model,Positions[i]);
        //model = glm::rotate(model,glm::radians(timer.elapsed()/10.f),glm::vec3(0.0,1.0,0.0));
        glm::mat4 transformation_matrices[3] = {model,view,projection};
        glUniformMatrix4fv(uniform_loc,3,GL_FALSE,glm::value_ptr(transformation_matrices[0]));
        glUniform3fv(color_loc,1,glm::value_ptr(colors[i]));
        glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(test->m_mesh->indices.size()),GL_UNSIGNED_INT,nullptr);
    }
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

    shader_program = new Shader_object(":/resources/shaders/testShader.vert",":/resources/shaders/testShader2.fsh");
    //create sphere object
    test = new Sphere_object(new Sphere_mesh(16,16),glm::vec3(0.0,1.0,0.5),1.0f);


    //create transformation matrces
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
    uniform_loc = glGetUniformLocation(shader_program->shader_ID,"transformation_matrices");
    color_loc = glGetUniformLocation(shader_program->shader_ID, "aColor");

    for(int i = 0; i < 10; i++) {
        colors[i] = glm::vec3(
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
            );
    }

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
