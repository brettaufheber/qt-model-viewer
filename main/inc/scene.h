#ifndef __SCENE_H__
#define __SCENE_H__

#include <ec_core.h>
#include "gl_model.h"


class Scene : public QGraphicsScene {

private:

    Q_OBJECT

    bool rend_lines, rend_smooth, use_material, use_texture, show_normals;

    QColor color_foreground, color_background;

    float distance;
    double scale_x, scale_y, scale_z;
    int angle_x, angle_y, angle_z;

    glModel *model;
    uint id_material;

    QGLWidget *gl_widget;

    QGraphicsRectItem *light_item;

    int normaliseAngle(int angle);

protected:

    void drawBackground(QPainter *painter, const QRectF&);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);

public slots:

    void loadModel(const QString &file_path);
    void loadTerrain(const QString &file_path);
    void loadTexture(const QString &file_path);
    void unload();
    void setMaterial(int id);

    void setModeLines();
    void setModeFlat();
    void setModeSmooth();
    void setModeColor();
    void setModeMaterial();
    void setModeTexture(bool enabled);
    void setModeNormals(bool enabled);

    void setForegroundColor();
    void setBackgroundColor();

    void setXScale(double scale);
    void setYScale(double scale);
    void setZScale(double scale);

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

    void setAmbientRed(double value);
    void setAmbientGreen(double value);
    void setAmbientBlue(double value);
    void setAmbientAlpha(double value);
    void setDiffuseRed(double value);
    void setDiffuseGreen(double value);
    void setDiffuseBlue(double value);
    void setDiffuseAlpha(double value);
    void setEmissionRed(double value);
    void setEmissionGreen(double value);
    void setEmissionBlue(double value);
    void setEmissionAlpha(double value);
    void setSpecularRed(double value);
    void setSpecularGreen(double value);
    void setSpecularBlue(double value);
    void setSpecularAlpha(double value);
    void setShininess(double value);

    void refreshScale();
    void refreshMaterial();
    void refreshMaterialColors();
    void refreshTexture();

signals:

    void xScaleChanged(double scale);
    void yScaleChanged(double scale);
    void zScaleChanged(double scale);

    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

    void ambientRedChanged(double value);
    void ambientGreenChanged(double value);
    void ambientBlueChanged(double value);
    void ambientAlphaChanged(double value);
    void diffuseRedChanged(double value);
    void diffuseGreenChanged(double value);
    void diffuseBlueChanged(double value);
    void diffuseAlphaChanged(double value);
    void emissionRedChanged(double value);
    void emissionGreenChanged(double value);
    void emissionBlueChanged(double value);
    void emissionAlphaChanged(double value);
    void specularRedChanged(double value);
    void specularGreenChanged(double value);
    void specularBlueChanged(double value);
    void specularAlphaChanged(double value);
    void shininessChanged(double value);

    void textureChanged(const QString &file_path);

    void materialChanged(const QList < QPair < QString, QStringList > > &list);

public:

    Scene(QGLWidget *widget);
    ~Scene();

    bool isLoaded();
    uint numPoints();
    uint numEdges();
    uint numFacets();

};

#endif
