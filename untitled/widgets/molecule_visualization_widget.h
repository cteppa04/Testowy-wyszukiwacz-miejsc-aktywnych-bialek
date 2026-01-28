#ifndef MOLECULE_VISUALIZATION_WIDGET_H
#define MOLECULE_VISUALIZATION_WIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

class Molecule_visualization_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Molecule_visualization_widget(QWidget *parent = nullptr);
    ~Molecule_visualization_widget();
protected:
    //functions that must be overriden from QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    GLuint VAO;
    GLuint VBO;
    GLuint shader_program;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
signals:
};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
