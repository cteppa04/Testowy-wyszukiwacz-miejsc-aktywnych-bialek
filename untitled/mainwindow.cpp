#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "atom.h"
#include "parse_pdb.h"

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
    QVector<Atom> atomy = parsePDB(file_name);
    QString allAtomsString;
    for(const Atom &x : atomy){
        allAtomsString += QString::number(x.serial)+": " + x.element;
    }
    QMessageBox::information(this,"..",allAtomsString);

}
