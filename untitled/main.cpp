#include "mainwindow.h"
#include <QSurfaceFormat>
#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setOption(QSurfaceFormat::DebugContext);   // optional, useful for debugging
    format.setRenderableType(QSurfaceFormat::OpenGL); // enforce desktop OpenGL

    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
