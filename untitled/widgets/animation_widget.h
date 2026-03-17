#ifndef ANIMATION_WIDGET_H
#define ANIMATION_WIDGET_H

#include "qlabel.h"
#include "qpushbutton.h"
#include "widgets/molecule_visualization_widget.h"
#include <QObject>
#include <QWidget>
#include <classes/protein.h>

class Animation_widget : public QWidget
{
    Q_OBJECT
public:
    Animation_widget(QWidget *parent = nullptr);
public slots:
    void render_protein(Protein *protein);
    void previous_animation_step();
    void next_animation_step();
private:
    QLabel *m_animation_step_label;
    Molecule_visualization_widget *m_visualizator;
    QPushButton *m_next_step_button;
    QPushButton *m_previous_step_button;
    int m_current_animation_step = 1;
    int CASTP_ANIMATION_STEP_COUNT = 6;

};

#endif // ANIMATION_WIDGET_H
