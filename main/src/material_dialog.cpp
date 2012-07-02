#include "../inc/material_dialog.h"
#include "../inc/resource.h"


MaterialDialog::MaterialDialog(Scene *scene, QWidget *parent) : QDialog(parent) {

    // initialise

    QWidget *widget_form = Resource::loadWidget(this, "material");

    QVBoxLayout *layout = new QVBoxLayout;

    list_names = findChild < QListWidget* > ("list_names");

    QLineEdit *line_edit_texture = findChild < QLineEdit* > ("line_edit_texture");

    QPushButton *push_button_texture = findChild < QPushButton* > ("push_button_texture");
    QPushButton *push_button_close = findChild < QPushButton* > ("push_button_close");

    QDoubleSpinBox *spin_box_ambient_red = findChild < QDoubleSpinBox* > ("spin_box_ambient_red");
    QDoubleSpinBox *spin_box_ambient_green = findChild < QDoubleSpinBox* > ("spin_box_ambient_green");
    QDoubleSpinBox *spin_box_ambient_blue = findChild < QDoubleSpinBox* > ("spin_box_ambient_blue");
    QDoubleSpinBox *spin_box_ambient_alpha = findChild < QDoubleSpinBox* > ("spin_box_ambient_alpha");
    QDoubleSpinBox *spin_box_diffuse_red = findChild < QDoubleSpinBox* > ("spin_box_diffuse_red");
    QDoubleSpinBox *spin_box_diffuse_green = findChild < QDoubleSpinBox* > ("spin_box_diffuse_green");
    QDoubleSpinBox *spin_box_diffuse_blue = findChild < QDoubleSpinBox* > ("spin_box_diffuse_blue");
    QDoubleSpinBox *spin_box_diffuse_alpha = findChild < QDoubleSpinBox* > ("spin_box_diffuse_alpha");
    QDoubleSpinBox *spin_box_emission_red = findChild < QDoubleSpinBox* > ("spin_box_emission_red");
    QDoubleSpinBox *spin_box_emission_green = findChild < QDoubleSpinBox* > ("spin_box_emission_green");
    QDoubleSpinBox *spin_box_emission_blue = findChild < QDoubleSpinBox* > ("spin_box_emission_blue");
    QDoubleSpinBox *spin_box_emission_alpha = findChild < QDoubleSpinBox* > ("spin_box_emission_alpha");
    QDoubleSpinBox *spin_box_specular_red = findChild < QDoubleSpinBox* > ("spin_box_specular_red");
    QDoubleSpinBox *spin_box_specular_green = findChild < QDoubleSpinBox* > ("spin_box_specular_green");
    QDoubleSpinBox *spin_box_specular_blue = findChild < QDoubleSpinBox* > ("spin_box_specular_blue");
    QDoubleSpinBox *spin_box_specular_alpha = findChild < QDoubleSpinBox* > ("spin_box_specular_alpha");
    QDoubleSpinBox *spin_box_shininess = findChild < QDoubleSpinBox* > ("spin_box_shininess");

    // signal slot handling

    connect(list_names, SIGNAL(currentRowChanged(int)), scene, SLOT(setMaterial(int)));
    connect(list_names, SIGNAL(currentRowChanged(int)), scene, SLOT(refreshMaterialColors()));
    connect(list_names, SIGNAL(currentRowChanged(int)), scene, SLOT(refreshTexture()));

    connect(push_button_texture, SIGNAL(clicked()), this, SLOT(loadTexture()));
    connect(push_button_close, SIGNAL(clicked()), this, SLOT(close()));

    connect(scene, SIGNAL(textureChanged(const QString&)), line_edit_texture, SLOT(setText(const QString&)));
    connect(this, SIGNAL(textureChanged(const QString&)), scene, SLOT(loadTexture(const QString&)));

    connect(spin_box_ambient_red, SIGNAL(valueChanged(double)), scene, SLOT(setAmbientRed(double)));
    connect(spin_box_ambient_green, SIGNAL(valueChanged(double)), scene, SLOT(setAmbientGreen(double)));
    connect(spin_box_ambient_blue, SIGNAL(valueChanged(double)), scene, SLOT(setAmbientBlue(double)));
    connect(spin_box_ambient_alpha, SIGNAL(valueChanged(double)), scene, SLOT(setAmbientAlpha(double)));
    connect(spin_box_diffuse_red, SIGNAL(valueChanged(double)), scene, SLOT(setDiffuseRed(double)));
    connect(spin_box_diffuse_green, SIGNAL(valueChanged(double)), scene, SLOT(setDiffuseGreen(double)));
    connect(spin_box_diffuse_blue, SIGNAL(valueChanged(double)), scene, SLOT(setDiffuseBlue(double)));
    connect(spin_box_diffuse_alpha, SIGNAL(valueChanged(double)), scene, SLOT(setDiffuseAlpha(double)));
    connect(spin_box_emission_red, SIGNAL(valueChanged(double)), scene, SLOT(setEmissionRed(double)));
    connect(spin_box_emission_green, SIGNAL(valueChanged(double)), scene, SLOT(setEmissionGreen(double)));
    connect(spin_box_emission_blue, SIGNAL(valueChanged(double)), scene, SLOT(setEmissionBlue(double)));
    connect(spin_box_emission_alpha, SIGNAL(valueChanged(double)), scene, SLOT(setEmissionAlpha(double)));
    connect(spin_box_specular_red, SIGNAL(valueChanged(double)), scene, SLOT(setSpecularRed(double)));
    connect(spin_box_specular_green, SIGNAL(valueChanged(double)), scene, SLOT(setSpecularGreen(double)));
    connect(spin_box_specular_blue, SIGNAL(valueChanged(double)), scene, SLOT(setSpecularBlue(double)));
    connect(spin_box_specular_alpha, SIGNAL(valueChanged(double)), scene, SLOT(setSpecularAlpha(double)));
    connect(spin_box_shininess, SIGNAL(valueChanged(double)), scene, SLOT(setShininess(double)));

    connect(scene, SIGNAL(ambientRedChanged(double)), spin_box_ambient_red, SLOT(setValue(double)));
    connect(scene, SIGNAL(ambientGreenChanged(double)), spin_box_ambient_green, SLOT(setValue(double)));
    connect(scene, SIGNAL(ambientBlueChanged(double)), spin_box_ambient_blue, SLOT(setValue(double)));
    connect(scene, SIGNAL(ambientAlphaChanged(double)), spin_box_ambient_alpha, SLOT(setValue(double)));
    connect(scene, SIGNAL(diffuseRedChanged(double)), spin_box_diffuse_red, SLOT(setValue(double)));
    connect(scene, SIGNAL(diffuseGreenChanged(double)), spin_box_diffuse_green, SLOT(setValue(double)));
    connect(scene, SIGNAL(diffuseBlueChanged(double)), spin_box_diffuse_blue, SLOT(setValue(double)));
    connect(scene, SIGNAL(diffuseAlphaChanged(double)), spin_box_diffuse_alpha, SLOT(setValue(double)));
    connect(scene, SIGNAL(emissionRedChanged(double)), spin_box_emission_red, SLOT(setValue(double)));
    connect(scene, SIGNAL(emissionGreenChanged(double)), spin_box_emission_green, SLOT(setValue(double)));
    connect(scene, SIGNAL(emissionBlueChanged(double)), spin_box_emission_blue, SLOT(setValue(double)));
    connect(scene, SIGNAL(emissionAlphaChanged(double)), spin_box_emission_alpha, SLOT(setValue(double)));
    connect(scene, SIGNAL(specularRedChanged(double)), spin_box_specular_red, SLOT(setValue(double)));
    connect(scene, SIGNAL(specularGreenChanged(double)), spin_box_specular_green, SLOT(setValue(double)));
    connect(scene, SIGNAL(specularBlueChanged(double)), spin_box_specular_blue, SLOT(setValue(double)));
    connect(scene, SIGNAL(specularAlphaChanged(double)), spin_box_specular_alpha, SLOT(setValue(double)));
    connect(scene, SIGNAL(shininessChanged(double)), spin_box_shininess, SLOT(setValue(double)));

    connect(scene, SIGNAL(materialChanged(const QList < QPair < QString, QStringList > >&)), this, SLOT(createSelection(const QList < QPair < QString, QStringList > >&)));

    // set layout

    layout->addWidget(widget_form);
    setLayout(layout);

    // set defaults

    setWindowTitle("Edit Materials");
}

void MaterialDialog::keyPressEvent(QKeyEvent *event) {

    switch (event->key()) {

        case Qt::Key_Return:
        case Qt::Key_Enter:
            break;

        case Qt::Key_Escape:
            this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}

void MaterialDialog::loadTexture() {

    QString file_path = QFileDialog::getOpenFileName(this, "Choose Image");

    if(!file_path.isEmpty())
        emit textureChanged(file_path);
}

void MaterialDialog::createSelection(const QList < QPair < QString, QStringList > > &list) {

    QListIterator < QPair < QString, QStringList > > iter(list);

    list_names->clear();

    while(iter.hasNext()) {

        QPair < QString, QStringList > row = iter.next();
        QListWidgetItem *item = new QListWidgetItem(list_names);

        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setText(row.first + ( row.second.size() != 0 ? ("(" + row.second.join(", ") + ")") : ""));

        list_names->addItem(item);
    }

    list_names->setCurrentRow(0);
}
