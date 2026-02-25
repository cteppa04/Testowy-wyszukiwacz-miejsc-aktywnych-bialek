#include "protein_list.h"
#include <QBoxLayout>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <classes/parse_file.h>
#include <QJsonArray>
#include <QStandardPaths>
#include <QTextStream>

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

    load_from_JSON();
}

Protein_list::~Protein_list()
{
    save_to_JSON();
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

void Protein_list::save_to_JSON()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);

    path += M_JSON_FILE_NAME;

    QFile file(path);

    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    qDebug() << path;

    QJsonArray proteins_array;
    QList<Protein_list_item*> items = this->findChildren<Protein_list_item*>();
    for(Protein_list_item* item : items){
        proteins_array.append(to_JSON(&item->m_protein));
    }

    QJsonObject root;
    root["proteins"] = proteins_array;

    QJsonDocument doc(root);

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void Protein_list::load_from_JSON()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    path += M_JSON_FILE_NAME;

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open protein json file";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonObject root = doc.object();

    QJsonArray proteins = root["proteins"].toArray();

    for(auto value : proteins){
        QJsonObject protein_object = value.toObject();

        QString name = protein_object["name"].toString();

        QJsonArray atoms_array = protein_object["atoms"].toArray();

        QVector<Atom> atoms;
        for(auto element : atoms_array  ){
            auto atom_object = element.toObject();
            Atom atom;
            atom.m_serial = atom_object["serial"].toInt();
            atom.m_element = atom_object["element"].toString();

            atom.m_position.x = float(atom_object["x"].toDouble());
            atom.m_position.y = float(atom_object["y"].toDouble());
            atom.m_position.z = float(atom_object["z"].toDouble());

            atoms.append(atom);
        }

        Protein protein(name,atoms);
        auto item = new Protein_list_item(protein,this);
        m_protein_container_layout->addWidget(item);

        item->connect(item, &Protein_list_item::request_deletion,
                      this, &Protein_list::delete_protein);
        item->connect(item, &Protein_list_item::select_protein,
                      this, &Protein_list::select_protein);
    }

}

QJsonObject Protein_list::to_JSON(Protein *protein)
{
    QJsonObject object_json;
    object_json["name"] = protein->m_name;
    QJsonArray atoms_array;
    for(auto &atom : protein->m_atom_list){
        QJsonObject atom_json;

        atom_json["serial"] = atom.m_serial;
        atom_json["element"] = atom.m_element;
        atom_json["x"] = atom.m_position.x;
        atom_json["y"] = atom.m_position.y;
        atom_json["z"] = atom.m_position.z;

        atoms_array.append(atom_json);
    }
    object_json["atoms"] = atoms_array;

    return object_json;
}





