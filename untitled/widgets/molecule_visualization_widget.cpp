#include <widgets/molecule_visualization_widget.h>
#include "classes/OpenGl/objects/object_factory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QTimer>
#include <QKeyEvent>

#include <classes/OpenGl/shader_object.h>
#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/material/material_master.h>
#include <classes/OpenGl/objects/object.h>
#include <classes/OpenGl/resource_manager.h>

//molecule colors
std::map<std::string, glm::vec3> element_colors = {
    {"H",  glm::vec3(1.0f, 1.0f, 1.0f)},   // White
    {"C",  glm::vec3(0.2f, 0.2f, 0.2f)},   // Dark gray
    {"N",  glm::vec3(0.0f, 0.0f, 1.0f)},   // Blue
    {"O",  glm::vec3(1.0f, 0.0f, 0.0f)},   // Red
    {"S",  glm::vec3(1.0f, 1.0f, 0.0f)},   // Yellow
    {"Se", glm::vec3(1.0f, 0.6f, 0.0f)},   // Orange
    {"P",  glm::vec3(1.0f, 0.5f, 0.0f)},   // Orange
    {"Cl", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
    {"Na", glm::vec3(0.5f, 0.5f, 1.0f)},   // Light blue
    {"K",  glm::vec3(0.5f, 0.0f, 1.0f)},   // Purple
    {"Ca", glm::vec3(0.5f, 0.5f, 0.0f)},   // Dark yellow
    {"Mg", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
    {"Fe", glm::vec3(0.8f, 0.4f, 0.0f)},   // Brown/Orange
    {"Cu", glm::vec3(1.0f, 0.5f, 0.5f)},   // Pink
    {"Zn", glm::vec3(0.5f, 0.5f, 0.5f)},   // Gray
    {"Co", glm::vec3(0.0f, 0.0f, 0.5f)},   // Dark blue
    {"I",  glm::vec3(0.58f, 0.0f, 0.58f)}  // Purple
};






glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

//keyboard input array
QSet<int> keys;

//origin point camera initialization

const glm::vec3 CAMERA_STARTING_POS(0.0f,0.0f,10.0f);
glm::vec3 orbit_camera_orbit_point (0.0f,0.0f,0.0f);
glm::vec3 camera_position(CAMERA_STARTING_POS);

//mesh management


Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus); // widget can accept keyboard focus
    setFocus(); // actively give it focus

    camera.setType(CAMERA_H::Camera::Type::Orbit);
    camera.start(&CAMERA_STARTING_POS);
    camera.set_camera_orbit_point(orbit_camera_orbit_point);

    QTimer *timer_refresh = new QTimer(this);
    connect(timer_refresh, &QTimer::timeout, this, QOverload<>::of(&Molecule_visualization_widget::update));
    timer_refresh->start(16); // ~60 FPS
}

void Molecule_visualization_widget::register_standard_atom_materials(Resource_manager<Material> &material_manager)
{
    // Map of element symbol to color
    std::map<std::string, glm::vec3> element_colors = {
        {"H",  glm::vec3(1.0f, 1.0f, 1.0f)},   // White
        {"C",  glm::vec3(0.2f, 0.2f, 0.2f)},   // Dark gray
        {"N",  glm::vec3(0.0f, 0.0f, 1.0f)},   // Blue
        {"O",  glm::vec3(1.0f, 0.0f, 0.0f)},   // Red
        {"S",  glm::vec3(1.0f, 1.0f, 0.0f)},   // Yellow
        {"Se", glm::vec3(1.0f, 0.6f, 0.0f)},   // Orange
        {"P",  glm::vec3(1.0f, 0.5f, 0.0f)},   // Orange
        {"Cl", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
        {"Na", glm::vec3(0.5f, 0.5f, 1.0f)},   // Light blue
        {"K",  glm::vec3(0.5f, 0.0f, 1.0f)},   // Purple
        {"Ca", glm::vec3(0.5f, 0.5f, 0.0f)},   // Dark yellow
        {"Mg", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
        {"Fe", glm::vec3(0.8f, 0.4f, 0.0f)},   // Brown/Orange
        {"Cu", glm::vec3(1.0f, 0.5f, 0.5f)},   // Pink
        {"Zn", glm::vec3(0.5f, 0.5f, 0.5f)},   // Gray
        {"Co", glm::vec3(0.0f, 0.0f, 0.5f)},   // Dark blue
        {"I",  glm::vec3(0.58f, 0.0f, 0.58f)}  // Purple
    };

    // Loop through and add each element to the material manager
    for (auto const& pair : element_colors) {
        const std::string& element = pair.first;
        const glm::vec3& color = pair.second;

        material_manager.add(element,
                             Material_factory::smooth_material(color, 1.0f)  // full opacity
                             );
    }
}

void Molecule_visualization_widget::paintGL()
{

    view = camera.update(&keys);

    //clear colors in bg and enable depth testing
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_BLEND);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both color and depth


    //use program
    shader_program->use();

    //sent matrices to shader program
    //glBindVertexArray(object_manager.get("atom")->mesh->renderer->VAO);

    // for(int i = 0; i < object_manager.get("atom")->instances.size(); i++){
    //     glm::mat4 transformation_matrices[3] = {object_manager.get("atom")->instances[i].m_model,view,projection};\
    //         shader_program->set_mat4("model",transformation_matrices[0]);
    //     shader_program->set_mat4("view",transformation_matrices[1]);
    //     shader_program->set_mat4("projection",transformation_matrices[2]);
    //     shader_program->set_vec3("aColor",object_manager.get("atom")->material->color);
    //     shader_program->set_float("aTransparency",object_manager.get("atom")->material->transparency);
    //     glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(object_manager.get("atom")->mesh->indices.size()),GL_UNSIGNED_INT,nullptr);
    // }
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
    register_standard_atom_materials(material_manager);

    //create transformation matrces
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);


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
