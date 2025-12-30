#ifndef VISUALIZATIONWINDOW_H
#define VISUALIZATIONWINDOW_H

#include <QWidget>

namespace Ui {
class VisualizationWindow;
}

class VisualizationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationWindow(QWidget *parent = nullptr);
    ~VisualizationWindow();

private:
    Ui::VisualizationWindow *ui;
};

#endif // VISUALIZATIONWINDOW_H
