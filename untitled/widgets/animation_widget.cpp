#include "animation_widget.h"
#include "qboxlayout.h"

Animation_widget::Animation_widget(QWidget *parent):
    QWidget(parent)
{
    auto *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(5,5,5,5);

    m_animation_step_label = new QLabel("no protein selected",this);

    m_visualizator = new Molecule_visualization_widget(this);

    auto *buttons_layout = new QHBoxLayout();
    m_previous_step_button = new QPushButton("<-");
    m_next_step_button = new QPushButton("->");
    buttons_layout->addStretch(1);
    buttons_layout->addWidget(m_previous_step_button,5);
    buttons_layout->addStretch(1);
    buttons_layout->addWidget(m_next_step_button,5);
    buttons_layout->addStretch(1);


    main_layout->addWidget(m_animation_step_label,1,Qt::AlignCenter);
    main_layout->addWidget(m_visualizator,20);
    main_layout->addLayout(buttons_layout,3);

}

void Animation_widget::render_protein(Protein *protein)
{
    m_visualizator->delete_protein();
    m_visualizator->add_protein(protein);
    m_animation_step_label->setText(protein->m_name);
}
