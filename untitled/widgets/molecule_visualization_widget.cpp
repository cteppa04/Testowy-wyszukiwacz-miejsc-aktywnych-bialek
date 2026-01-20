#include "widgets/molecule_visualization_widget.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>
#include <QMessageBox>

Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QMessageBox::information(this,"..","ok");
}

Molecule_visualization_widget::~Molecule_visualization_widget()
{}

void Molecule_visualization_widget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

}

void Molecule_visualization_widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Molecule_visualization_widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Example: draw a single triangle (legacy)
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f( 0.5f, -0.5f, 0.0f);
    glVertex3f( 0.0f,  0.5f, 0.0f);
    glEnd();
}
