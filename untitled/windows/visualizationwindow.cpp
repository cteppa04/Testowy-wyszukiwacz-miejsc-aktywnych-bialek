#include "visualizationwindow.h"
#include "ui_visualizationwindow.h"

VisualizationWindow::VisualizationWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VisualizationWindow)
{
    ui->setupUi(this);
    setWindowTitle("Visualization");
    resize(800, 600);
}

VisualizationWindow::~VisualizationWindow()
{
    delete ui;
}
