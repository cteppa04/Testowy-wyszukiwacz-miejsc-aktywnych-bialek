#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "qboxlayout.h"
#include <widgets/protein_list.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout* layout = new QHBoxLayout(ui->centralWidget);

    list = new Protein_list(this);
    animation_widget = new Animation_widget(this);

    list->connect(list,
            &Protein_list::render_protein,
            animation_widget,
            &Animation_widget::render_protein);
    layout->addWidget(list,3);
    layout->addWidget(animation_widget,7);

    ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
