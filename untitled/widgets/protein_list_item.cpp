#include "protein_list_item.h"
#include "qboxlayout.h"

#include <QLabel>

Protein_list_item::Protein_list_item(Protein protein,QWidget *parent):
    QWidget(parent),
    m_protein(std::move(protein))
{
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(5);
    this->setLayout(mainLayout);

    m_nameLabel = new QLabel(m_protein.m_name,this);
    m_delButton = new QPushButton("X",this);
    m_delButton->connect(m_delButton,
                        &QPushButton::clicked,
                        this,
                        [this]{
                             emit request_deletion(this);
                        });

    mainLayout->addWidget(m_nameLabel,1);
    mainLayout->addWidget(m_delButton);

    QFontMetrics fm(m_nameLabel->font());
    this->setMinimumHeight(fm.height() * 3);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);

    this->setContentsMargins(5,5,5,5);
}

void Protein_list_item::mousePressEvent(QMouseEvent *event)
{
    emit select_protein(&m_protein);
    //keep default behaviour
    QWidget::mousePressEvent(event);
}


