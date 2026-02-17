#ifndef ANIMATION_WIDGET_H
#define ANIMATION_WIDGET_H

#include "qlabel.h"
#include "qpushbutton.h"
#include "widgets/molecule_visualization_widget.h"
#include <QObject>
#include <QWidget>

class Animation_widget : public QWidget
{
    Q_OBJECT
public:
    Animation_widget(QWidget *parent = nullptr);
private:
    QLabel *m_animation_step_label;
    Molecule_visualization_widget *m_visualizator;
    QPushButton *m_next_step_button;
    QPushButton *m_previous_step_button;

};

#endif // ANIMATION_WIDGET_H
