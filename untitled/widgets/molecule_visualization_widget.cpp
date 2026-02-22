#include <widgets/molecule_visualization_widget.h>
#include "classes/OpenGl/objects/object_factory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QTimer>
#include <QKeyEvent>

#include <classes/OpenGl/shaders/shader_object.h>

#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/material/material_master.h>
#include <classes/OpenGl/objects/object.h>
#include <classes/OpenGl/resource_manager.h>


glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

//keyboard input array
QSet<int> keys;

//origin point camera initialization

const glm::vec3 CAMERA_STARTING_POS(0.0f,0.0f,10.0f);
glm::vec3 orbit_camera_orbit_point (0.0f,0.0f,0.0f);
glm::vec3 camera_position(CAMERA_STARTING_POS);

Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // //handle keyboard clicking
    // setFocusPolicy(Qt::StrongFocus); // widget can accept keyboard focus
    // setFocus(); // actively give it focus

    //handle camera
    camera.setType(CAMERA_H::Camera::Type::Orbit);
    camera.start(&CAMERA_STARTING_POS);
    camera.set_camera_orbit_point(orbit_camera_orbit_point);

    //set fps
    QTimer *timer_refresh = new QTimer(this);
    connect(timer_refresh, &QTimer::timeout, this, QOverload<>::of(&Molecule_visualization_widget::update));
    timer_refresh->start(16); // ~60 FPS

}

void Molecule_visualization_widget::add_protein(Protein *protein)
{
    for(int i = 0;i < protein->m_atom_list.count();++i){
        auto atom = protein->m_atom_list[i];
        qDebug() << atom.m_element.toStdString();
        //normalize atom color to read map properly
        std::string key = atom.m_element.toStdString();
        key.erase(0, key.find_first_not_of(" \t\n\r"));
        key.erase(key.find_last_not_of(" \t\n\r") + 1);

        if(!key.empty()){
            key[0] = std::toupper(key[0]);
            if(key.size() > 1){
                key[1] = std::toupper(key[1]);
            }
        }

        auto it = elements_colors.find(key);
        auto color = it->second;
        qDebug() << "Color: " << color.x << color.y << color.z;
        auto model = glm::mat4(1.0f);
        model = glm::translate(model,atom.m_position);
        model = glm::scale(model,glm::vec3(0.5f));
        atoms->add_instance(model,color,1.0f);
        qDebug() << protein->m_atom_list[i].m_position.x << protein->m_atom_list[i].m_position.y << protein->m_atom_list[i].m_position.z;
    }
    atoms->updateGPU();
}

void Molecule_visualization_widget::delete_protein()
{
    atoms->clear();
}


void Molecule_visualization_widget::paintGL()
{
    view = camera.update(&keys);

    //clear colors in bg and enable depth testing
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both color and depth

    //use program
    glUseProgram(object_manager.get("atom")->material->m_shader->shader_ID);

    //send matrices to shader program
    object_manager.get("atom")->material->m_shader->set_mat4("view",view);
    object_manager.get("atom")->material->m_shader->set_mat4("projection",projection);
    //render
    glBindVertexArray(object_manager.get("atom")->mesh->renderer->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,object_manager.get("atom")->mesh->indices.count(),GL_UNSIGNED_INT,0,atoms->instance_count());

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

    //create transformation matrces
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

    //create atom object
    mesh_manager.add("atom_sphere",Mesh_factory::Sphere_mesh(16,16));

    shader_manager.add("basic",new Shader_object(":/resources/shaders/testShader.vert",
                                                  ":/resources/shaders/testShader2.fsh"));

    material_manager.add("smooth",Material_factory::material(shader_manager.get("basic")));

    object_manager.add("atom",Object_factory::sphere(mesh_manager.get("atom_sphere"),
                                              material_manager.get("smooth")));
    //setup atoms
    atoms = new Object_instance(object_manager.get("atom"));
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

void Molecule_visualization_widget::showEvent(QShowEvent *event)
{
    grabKeyboard();
    QOpenGLWidget::showEvent(event);
}
