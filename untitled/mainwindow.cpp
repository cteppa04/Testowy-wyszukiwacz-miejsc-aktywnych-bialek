#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "classes/atom.h"
#include "classes/parse_file.h"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include <widgets/protein_list.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHBoxLayout* layout = new QHBoxLayout(ui->centralWidget);

    list = new Protein_list(this);
    animation_widget = new Animation_widget(this);

    layout->addWidget(list,3);
    layout->addWidget(animation_widget,7);

    ui->centralWidget->setLayout(layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
