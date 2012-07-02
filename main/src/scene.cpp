#include "../inc/scene.h"


Scene::Scene(QGLWidget *widget) : color_foreground(200, 200, 180), color_background(80, 80, 80), distance(1.4f), model(NULL), gl_widget(widget) {

    QRadialGradient gradient(30, 30, 30, 30, 30);
    gradient.setColorAt(0.2, Qt::yellow);
    gradient.setColorAt(0.8, Qt::transparent);

    light_item = new QGraphicsRectItem(0, 0, 80, 80);
    light_item->setPen(Qt::NoPen);
    light_item->setBrush(gradient);
    light_item->setFlag(QGraphicsItem::ItemIsMovable);
    light_item->setPos(100, 100);

    addItem(light_item);
}

Scene::~Scene() {

    ecCore::release(model);
}

int Scene::normaliseAngle(int angle) {

    while(angle < 0)
        angle += 360;

    while(angle > 360)
        angle -= 360;

    return angle;
}

void Scene::refreshScale() {

    if(model == NULL)
        throw ecCore::Exception("Scene::refreshScale; model not loaded");

    emit xScaleChanged(scale_x);
    emit yScaleChanged(scale_y);
    emit zScaleChanged(scale_z);
}

void Scene::refreshTexture() {

    if(model == NULL)
        throw ecCore::Exception("Scene::refreshTexture; model not loaded");

    emit textureChanged(model->textureName(id_material));
}

void Scene::refreshMaterial() {

    if(model == NULL)
        throw ecCore::Exception("Scene::refreshMaterial; model not loaded");

    QList < QPair < QString, QStringList > > list;

    for(uint i = 0; i < model->numMaterials(); i++) {

        QPair < QString, QStringList > row;

        row.first = model->materialName(i);

        for(uint j = 0; j < model->numGroups(); j++) {

            if(i == model->getMaterial(j))
                row.second.append(model->groupName(j));
        }

        list.append(row);
    }

    emit materialChanged(list);
}

void Scene::refreshMaterialColors() {

    if(model == NULL)
        throw ecCore::Exception("Scene::refreshMaterialColors; model not loaded");

    emit ambientRedChanged(model->ambient(id_material)[0]);
    emit ambientGreenChanged(model->ambient(id_material)[1]);
    emit ambientBlueChanged(model->ambient(id_material)[2]);
    emit ambientAlphaChanged(model->ambient(id_material)[3]);
    emit diffuseRedChanged(model->diffuse(id_material)[0]);
    emit diffuseGreenChanged(model->diffuse(id_material)[1]);
    emit diffuseBlueChanged(model->diffuse(id_material)[2]);
    emit diffuseAlphaChanged(model->diffuse(id_material)[3]);
    emit emissionRedChanged(model->emission(id_material)[0]);
    emit emissionGreenChanged(model->emission(id_material)[1]);
    emit emissionBlueChanged(model->emission(id_material)[2]);
    emit emissionAlphaChanged(model->emission(id_material)[3]);
    emit specularRedChanged(model->specular(id_material)[0]);
    emit specularGreenChanged(model->specular(id_material)[1]);
    emit specularBlueChanged(model->specular(id_material)[2]);
    emit specularAlphaChanged(model->specular(id_material)[3]);
    emit shininessChanged(model->shininess(id_material));
}

void Scene::drawBackground(QPainter *painter, const QRectF&) {

    if(painter->paintEngine()->type() != QPaintEngine::OpenGL && painter->paintEngine()->type() != QPaintEngine::OpenGL2) {

        qWarning() << "Scene: drawBackground needs a QGLWidget to be set as viewport on the graphics view";
        return;
    }

    painter->beginNativePainting();

    glClearColor(color_background.redF(), color_background.greenF(), color_background.blueF(), color_background.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(model != NULL) {

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPerspective(70, width() / height(), 0.01, 1000.0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        float position[] = { light_item->x() - width() / 2, height() / 2 - light_item->y(), 512, 0 };
        glLightfv(GL_LIGHT0, GL_POSITION, position);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, -distance);
        glRotatef((float) angle_x, 1, 0, 0);
        glRotatef((float) angle_y, 0, 1, 0);
        glRotatef((float) angle_z, 0, 0, 1);

        glEnable(GL_LIGHT0);

        model->draw(color_foreground, rend_lines, rend_smooth, use_material, use_texture, show_normals);

        glDisable(GL_LIGHT0);

        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
    }

    painter->endNativePainting();
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    QGraphicsScene::mouseMoveEvent(event);

    if(!event->isAccepted()) {

        if(event->buttons()) {

            QPoint delta = event->screenPos() - event->lastScreenPos();

            angle_x = normaliseAngle(angle_x + delta.y());
            angle_y = normaliseAngle(angle_y + delta.x());
            angle_z = normaliseAngle(angle_z + 0);

            emit xRotationChanged(angle_x);
            emit yRotationChanged(angle_y);
            emit zRotationChanged(angle_z);

            event->accept();
            update();
        }
    }
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent *event) {

    QGraphicsScene::wheelEvent(event);

    if(!event->isAccepted()) {

        distance *= qPow(1.2, -event->delta() / 120);

        event->accept();
        update();
    }
}

void Scene::loadModel(const QString &file_path) {

    ecCore::release(model);

    model = new glModel(gl_widget);

    model->loadOBJ(file_path, false);
    model->unitize();
    model->overwriteIdentity();

    id_material = model->findMaterial("default");
    scale_x = scale_y = scale_z = 1.0d;

    refreshScale();
    refreshMaterial();
    refreshMaterialColors();
    refreshTexture();

    update();
}

void Scene::loadTerrain(const QString &file_path) {

    ecCore::release(model);

    QImage image;

    if(!image.load(file_path))
        throw ecCore::InputException("Scene::loadTexture; cannot open image »" + file_path + "«");

    model = new glModel(gl_widget);

    id_material = model->addMaterial("default");
    uint id_group = model->addGroup("default", id_material);
    scale_x = scale_y = scale_z = 1.0d;

    model->createTerrain(image, id_group, false);
    model->unitize();
    model->overwriteIdentity();

    refreshScale();
    refreshMaterial();
    refreshMaterialColors();
    refreshTexture();

    update();
}

void Scene::loadTexture(const QString &file_path) {

    if(model == NULL)
        throw ecCore::Exception("Scene::loadTexture; model not loaded");

    QPixmap pixmap;

    if(!pixmap.load(file_path))
        throw ecCore::InputException("Scene::loadTexture; cannot open texture »" + file_path + "«");

    model->bindTexture(pixmap, file_path, id_material);

    refreshTexture();

    update();
}

void Scene::unload() {

    ecCore::release(model);
    update();
}

void Scene::setMaterial(int id) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setMaterial; model not loaded");

    if(model->numMaterials() > (uint) id)
        id_material = id;
}

void Scene::setModeLines() {

    rend_lines = true;
    rend_smooth = true;
    use_texture = false;
    update();
}

void Scene::setModeFlat() {

    rend_lines = false;
    rend_smooth = false;
    update();
}

void Scene::setModeSmooth() {

    rend_lines = false;
    rend_smooth = true;
    update();
}

void Scene::setModeColor() {

    use_material = false;
    update();
}

void Scene::setModeMaterial() {

    use_material = true;
    update();
}

void Scene::setModeTexture(bool enabled) {

    use_texture = enabled;
    update();
}

void Scene::setModeNormals(bool enabled) {

    show_normals = enabled;
    update();
}

void Scene::setForegroundColor() {

    QColor color = QColorDialog::getColor(color_foreground);

    if (color.isValid()) {

        color_foreground = color;
        update();
    }
}

void Scene::setBackgroundColor() {

    QColor color = QColorDialog::getColor(color_background);

    if (color.isValid()) {

        color_background = color;
        update();
    }
}

void Scene::setXScale(double scale) {

    scale_x = scale;
    model->loadIdentity();
    model->scale(QVector3D(scale_x, scale_y, scale_z));
    update();
}

void Scene::setYScale(double scale) {

    scale_y = scale;
    model->loadIdentity();
    model->scale(QVector3D(scale_x, scale_y, scale_z));
    update();
}

void Scene::setZScale(double scale) {

    scale_z = scale;
    model->loadIdentity();
    model->scale(QVector3D(scale_x, scale_y, scale_z));
    update();
}

void Scene::setXRotation(int angle) {

    angle_x = normaliseAngle(angle);
    emit xRotationChanged(angle_x);
    update();
}

void Scene::setYRotation(int angle) {

    angle_y = normaliseAngle(angle);
    emit yRotationChanged(angle_y);
    update();
}

void Scene::setZRotation(int angle) {

    angle_z = normaliseAngle(angle);
    emit zRotationChanged(angle_z);
    update();
}

void Scene::setAmbientRed(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setAmbientRed; model not loaded");

    model->ambient(id_material)[0] = (float) value;
    update();
}

void Scene::setAmbientGreen(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setAmbientGreen; model not loaded");

    model->ambient(id_material)[1] = (float) value;
    update();
}

void Scene::setAmbientBlue(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setAmbientBlue; model not loaded");

    model->ambient(id_material)[2] = (float) value;
    update();
}

void Scene::setAmbientAlpha(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setAmbientAlpha; model not loaded");

    model->ambient(id_material)[3] = (float) value;
    update();
}

void Scene::setDiffuseRed(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setDiffuseRed; model not loaded");

    model->diffuse(id_material)[0] = (float) value;
    update();
}

void Scene::setDiffuseGreen(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setDiffuseGreen; model not loaded");

    model->diffuse(id_material)[1] = (float) value;
    update();
}

void Scene::setDiffuseBlue(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setDiffuseBlue; model not loaded");

    model->diffuse(id_material)[2] = (float) value;
    update();
}

void Scene::setDiffuseAlpha(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setDiffuseAlpha; model not loaded");

    model->diffuse(id_material)[3] = (float) value;
    update();
}

void Scene::setEmissionRed(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setEmissionRed; model not loaded");

    model->emission(id_material)[0] = (float) value;
    update();
}

void Scene::setEmissionGreen(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setEmissionGreen; model not loaded");

    model->emission(id_material)[1] = (float) value;
    update();
}

void Scene::setEmissionBlue(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setEmissionBlue; model not loaded");

    model->emission(id_material)[2] = (float) value;
    update();
}

void Scene::setEmissionAlpha(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setEmissionAlpha; model not loaded");

    model->emission(id_material)[3] = (float) value;
    update();
}

void Scene::setSpecularRed(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setSpecularRed; model not loaded");

    model->specular(id_material)[0] = (float) value;
    update();
}

void Scene::setSpecularGreen(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setSpecularGreen; model not loaded");

    model->specular(id_material)[1] = (float) value;
    update();
}

void Scene::setSpecularBlue(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setSpecularBlue; model not loaded");

    model->specular(id_material)[2] = (float) value;
    update();
}

void Scene::setSpecularAlpha(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setSpecularAlpha; model not loaded");

    model->specular(id_material)[3] = (float) value;
    update();
}

void Scene::setShininess(double value) {

    if(model == NULL)
        throw ecCore::Exception("Scene::setShininess; model not loaded");

    model->shininess(id_material) = (float) value;
    update();
}

bool Scene::isLoaded() {

    return model != NULL;
}

uint Scene::numPoints() {

    if(model == NULL)
        throw ecCore::Exception("Scene::numPoints; model not loaded");

    return model->points();
}

uint Scene::numEdges() {

    if(model == NULL)
        throw ecCore::Exception("Scene::numEdges; model not loaded");

    return model->edges();
}

uint Scene::numFacets() {

    if(model == NULL)
        throw ecCore::Exception("Scene::numFaces; model not loaded");

    return model->facets();
}
