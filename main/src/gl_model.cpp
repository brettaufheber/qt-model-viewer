#include "../inc/gl_model.h"


glModel::glModel(QGLWidget *widget) : gl_widget(widget) {}

// == == == == == == == == == == == == == == == == ==

glModel::~glModel() {

    for(uint i = 0; i < (uint) materials.size(); i++)
        deleteTexture(i);
}

// == == == == == == == == == == == == == == == == ==

uint glModel::findGroup(const QString &name) {

    QVectorIterator < Group > iter(groups);

    for(uint i = 0; iter.hasNext(); i++)
        if(iter.next().name == name)
            return i;

    return FAILURE;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::findMaterial(const QString &name) {

    QVectorIterator < Material > iter(materials);

    for(uint i = 0; iter.hasNext(); i++)
        if(iter.next().name == name)
            return i;

    return FAILURE;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::addGroup(const QString &name, uint id_material) {

    if(findGroup(name) != FAILURE)
        throw ecCore::Exception("glModel::addGroup; group already exists »" + name + "«");

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::addGroup; material index out of range");

    Group group;
    group.name = name;
    group.id_material = id_material;

    groups.append(group);

    return groups.size() - 1;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::addMaterial(const QString &name) {

    if(findMaterial(name) != FAILURE)
        throw ecCore::Exception("glModel::addMaterial; material already exists »" + name + "«");

    Material material;

    material.name = name;
    material.ambient[0] = 0.2f;
    material.ambient[1] = 0.2f;
    material.ambient[2] = 0.2f;
    material.ambient[3] = 1.0f;
    material.diffuse[0] = 0.8f;
    material.diffuse[1] = 0.8f;
    material.diffuse[2] = 0.8f;
    material.diffuse[3] = 1.0f;
    material.emissive[0] = 0.0f;
    material.emissive[1] = 0.0f;
    material.emissive[2] = 0.0f;
    material.emissive[3] = 1.0f;
    material.specular[0] = 0.0f;
    material.specular[1] = 0.0f;
    material.specular[2] = 0.0f;
    material.specular[3] = 1.0f;
    material.shininess = 0.0f;
    material.id_texture = UNDEFINED;

    materials.append(material);

    return materials.size() - 1;
}

// == == == == == == == == == == == == == == == == ==

void glModel::setMaterial(uint id_group, uint id_material) {

    if((uint) groups.size() <= id_group)
        throw ecCore::Exception("glModel::setMaterial; group index out of range");

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::setMaterial; material index out of range");

    groups[id_group].id_material = id_material;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::getMaterial(uint id_group) {

    if((uint) groups.size() <= id_group)
        throw ecCore::Exception("glModel::getMaterial; group index out of range");

    return groups[id_group].id_material;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::numGroups() {

    return groups.size();
}

// == == == == == == == == == == == == == == == == ==

uint glModel::numMaterials() {

    return materials.size();
}

// == == == == == == == == == == == == == == == == ==

QString glModel::groupName(uint id_group) {

    if((uint) groups.size() <= id_group)
        throw ecCore::Exception("glModel::groupName; group index out of range");

    return groups[id_group].name;
}

// == == == == == == == == == == == == == == == == ==

QString glModel::materialName(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::materialName; material index out of range");

    return materials[id_material].name;
}

// == == == == == == == == == == == == == == == == ==

QString glModel::textureName(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::textureName; material index out of range");

    return materials[id_material].name_texture;
}

// == == == == == == == == == == == == == == == == ==

void glModel::bindTexture(const QPixmap &pixmap, const QString &name, uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::bindTexture; material index out of range");

    if(materials[id_material].id_texture != UNDEFINED)
        deleteTexture(id_material);

    materials[id_material].name_texture = name;
    materials[id_material].id_texture = gl_widget->bindTexture(pixmap, GL_TEXTURE_2D, GL_RGBA, QGLContext::DefaultBindOption);
}

// == == == == == == == == == == == == == == == == ==

void glModel::deleteTexture(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::deleteTexture; material index out of range");

    if(materials[id_material].id_texture != UNDEFINED) {

        gl_widget->deleteTexture(materials[id_material].id_texture);
        materials[id_material].name_texture.clear();
        materials[id_material].id_texture = UNDEFINED;
    }
}

// == == == == == == == == == == == == == == == == ==

float *glModel::ambient(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::ambient; material index out of range");

    return materials[id_material].ambient;
}

// == == == == == == == == == == == == == == == == ==

float *glModel::diffuse(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::diffuse; material index out of range");

    return materials[id_material].diffuse;
}

// == == == == == == == == == == == == == == == == ==

float *glModel::emission(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::emission; material index out of range");

    return materials[id_material].emissive;
}

// == == == == == == == == == == == == == == == == ==

float *glModel::specular(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::specular; material index out of range");

    return materials[id_material].specular;
}

// == == == == == == == == == == == == == == == == ==

float &glModel::shininess(uint id_material) {

    if((uint) materials.size() <= id_material)
        throw ecCore::Exception("glModel::shininess; material index out of range");

    return materials[id_material].shininess;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::points() {

    return vertices.size();
}

// == == == == == == == == == == == == == == == == ==

uint glModel::edges() {

    QVectorIterator < Group > iter(groups);
    uint num = 0;

    while(iter.hasNext())
        num += iter.next().i_edges.size();

    return num / 2;
}

// == == == == == == == == == == == == == == == == ==

uint glModel::facets() {

    QVectorIterator < Group > iter(groups);
    uint num = 0;

    while(iter.hasNext())
        num += iter.next().i_points.size();

    return num / 3;
}
