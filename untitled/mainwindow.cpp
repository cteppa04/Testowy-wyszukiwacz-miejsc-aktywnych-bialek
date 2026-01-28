#include <QDebug>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "classes/atom.h"
#include "classes/parse_file.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Wybierz plik grubasie","C://");
    QVector<Atom> atomy = Parse_file::parse_PDB(file_name);
    QString allAtomsString;
    //create atom string for test purposes
    for(const Atom &x : atomy){
        allAtomsString += QString::number(x.serial)+": " + x.element;
    }
    QMessageBox::information(this,"..",allAtomsString);

}
