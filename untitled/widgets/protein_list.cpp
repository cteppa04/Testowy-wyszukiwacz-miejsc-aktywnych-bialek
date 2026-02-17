#include "protein_list.h"
#include <QBoxLayout>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <classes/parse_file.h>

Protein_list::Protein_list(QWidget *parent) : QWidget(parent) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);

    auto *protein_container = new QWidget(this);
    m_protein_container_layout = new QVBoxLayout(protein_container);
    m_protein_container_layout->setAlignment(Qt::AlignTop);

    scroll_area->setWidget(protein_container);

    m_add_protein_button = new QPushButton("Add protein",this);
    m_add_protein_button->connect(m_add_protein_button,
                                  &QPushButton::clicked,
                                  this,
                                  &Protein_list::add_protein);

    mainLayout->addWidget(scroll_area,7);
    mainLayout->addWidget(m_add_protein_button,3);
}

Protein_list::~Protein_list()
{

}

void Protein_list::add_protein()
{
    QString file_name = QFileDialog::getOpenFileName(this,
                                                     "Please choose a file",
                                                     QDir::homePath(),
                                                     "PDB Files (*.pdb);;All Files (*)",
                                                     nullptr,
                                                     QFileDialog::DontUseNativeDialog);
    if(file_name.isEmpty()){
        return;
    }

    QFileInfo file_info(file_name);

    if (file_name.isEmpty() || file_info.suffix().toLower() != "pdb"){
        QMessageBox::information(this,"..","Wrong file type");
        return;
    }
    QVector<Atom> atomy = Parse_file::parse_PDB(file_name);

    auto *item = new Protein_list_item(Protein(file_info.fileName(),std::move(atomy)));
    item->connect(item,
                  &Protein_list_item::request_deletion,
                  this,
                  &Protein_list::delete_protein);

    item->connect(item,
                  &Protein_list_item::select_protein,
                  this,
                  &Protein_list::select_protein);
    m_protein_container_layout->addWidget(item);
}

void Protein_list::delete_protein(Protein_list_item *item)
{
    m_protein_container_layout->removeWidget(item);
    item->deleteLater();
}

void Protein_list::select_protein(Protein *selected_protein)
{
    emit render_protein(selected_protein);
}



