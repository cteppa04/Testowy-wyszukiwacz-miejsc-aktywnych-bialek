#ifndef PROTEIN_LIST_ITEM_H
#define PROTEIN_LIST_ITEM_H

#include "classes/protein.h"
#include "qlabel.h"
#include <QWidget>
#include <qpushbutton.h>

class Protein_list_item : public QWidget
{
    Q_OBJECT
public:
    Protein_list_item(Protein protein,QWidget *parent = nullptr);
    Protein m_protein;
signals:
    void request_deletion(Protein_list_item *item);
    void select_protein(Protein *selected_protein);
private:
    //override klicking on item
    void mousePressEvent(QMouseEvent *event) override;
    QLabel *m_nameLabel;
    QPushButton *m_delButton;
};

#endif // PROTEIN_LIST_ITEM_H
