#ifndef PROTEIN_LIST_H
#define PROTEIN_LIST_H

#include "qboxlayout.h"
#include "qpushbutton.h"
#include "widgets/protein_list_item.h"
#include <QVector>
#include <QWidget>

class Protein_list : public QWidget
{
    Q_OBJECT

public:
    Protein_list(QWidget *parent = nullptr);
    ~Protein_list();
    void add_protein();
signals:
    void render_protein(Protein *protein);
private slots:
    void delete_protein(Protein_list_item *item);
    void select_protein(Protein *selected_protein);
private:

    QPushButton *m_add_protein_button;
    QVBoxLayout *m_protein_container_layout;
};

#endif // PROTEIN_LIST_H
