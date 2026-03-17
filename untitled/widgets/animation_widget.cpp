    #include "animation_widget.h"
#include "qboxlayout.h"

Animation_widget::Animation_widget(QWidget *parent):
    QWidget(parent)
{
    auto *main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(5,5,5,5);

    m_animation_step_label = new QLabel("no protein selected",this);

    m_visualizator = new Molecule_visualization_widget(this);
    m_visualizator->set_animation_step(m_current_animation_step);

    auto *buttons_layout = new QHBoxLayout();
    m_previous_step_button = new QPushButton("<-");
    connect(m_previous_step_button,&QPushButton::clicked,this,&Animation_widget::previous_animation_step);
    m_next_step_button = new QPushButton("->");
    connect(m_next_step_button,&QPushButton::clicked,this,&Animation_widget::next_animation_step);
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

void Animation_widget::next_animation_step(){
    if(m_current_animation_step >= CASTP_ANIMATION_STEP_COUNT) return;
    m_current_animation_step += 1;
    m_visualizator->set_animation_step(m_current_animation_step);
    m_animation_step_label->setText(QString::number(m_current_animation_step));
}

void Animation_widget::previous_animation_step(){
    if(m_current_animation_step <= 1) return;
    m_current_animation_step -= 1;
    m_visualizator->set_animation_step(m_current_animation_step);
    m_animation_step_label->setText(QString::number(m_current_animation_step));
}
