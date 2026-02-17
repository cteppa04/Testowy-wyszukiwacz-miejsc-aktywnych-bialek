#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/animation_widget.h"
#include "widgets/protein_list.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Protein_list *list;
    Animation_widget *animation_widget;
};
#endif // MAINWINDOW_H
