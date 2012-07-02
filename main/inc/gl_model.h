#ifndef __GL_MODEL_H__
#define __GL_MODEL_H__

#include <ec_core.h>
#include <QtOpenGL>
#include <GL/glu.h>

class glModel {

private:

//////////////////////////////////
// Data

    typedef struct _Triangle {

        uint xyz[3];
        uint uv[3];
        bool use_uv;

    } Triangle;

    typedef struct _Group {

        QString name;                           // name of group
        uint id_material;                       // index to material
        QVector < uint > i_edges;               // edge indices for lines
        QVector < uint > i_points;              // vertex and normal indices
        QVector < uint > i_texcoords;           // texture coordinate indices
        QVector < Triangle > triangles;         // collection of triangles;
        QVector < QVector3D > seq_vertices;     // sequence of vertices
        QVector < QVector3D > seq_normals;      // sequence of normals
        QVector < QVector2D > seq_texcoords;    // sequence of texture coordinates

    } Group;

    typedef struct _Material {

        QString name;                           // name of material
        QString name_texture;                   // name of texture
        uint id_texture;                        // texture id
        float ambient[4];                       // ambient color of material (Ka)
        float diffuse[4];                       // diffuse color of material (Kd)
        float emissive[4];                      // emissive color of material (Ke)
        float specular[4];                      // specular color of material (Ks)
        float shininess;                        // specular exponent (Ns)

    } Material;

    QVector < QVector3D > vertices;             // collection of vertices
    QVector < QVector3D > identity;             // collection of vertex originals
    QVector < QVector3D > normals;              // collection of normals
    QVector < QVector2D > texcoords;            // collection of texture coordinates

    QVector < Group > groups;
    QVector < Material > materials;

    QGLWidget *gl_widget;

//////////////////////////////////
// Procedures

    void loadMaterial(const QString &file_path);

    void parseOBJ(const QDir &dir, QTextStream &in, bool append);
    void parseMTL(const QDir &dir, QTextStream &in);

    void vertexSequence(uint id_group);
    void normalSequence(uint id_group);
    void texcoordSequence(uint id_group);

//////////////////////////////////

public:

//////////////////////////////////
// Data

    typedef struct _Facet {

        uint id_group;
        bool use_texture;
        QVector < QPair < uint, uint > > points;

    } Facet;

//////////////////////////////////
// Constructors and Destructors

    glModel(QGLWidget *widget);
    ~glModel();

//////////////////////////////////
// Procedures

    uint findGroup(const QString &name);
    uint findMaterial(const QString &name);

    uint addGroup(const QString &name, uint id_material);

    uint addMaterial(const QString &name);
    void setMaterial(uint id_group, uint id_material);
    uint getMaterial(uint id_group);

    uint numGroups();
    uint numMaterials();

    QString groupName(uint id_group);
    QString materialName(uint id_material);
    QString textureName(uint id_material);

    void bindTexture(const QPixmap &pixmap, const QString &name, uint id_material);
    void deleteTexture(uint id_material);

    float *ambient(uint id_material);
    float *diffuse(uint id_material);
    float *emission(uint id_material);
    float *specular(uint id_material);
    float &shininess(uint id_material);

    uint points();
    uint edges();
    uint facets();

    void setGeometry(const QVector < QVector3D > &v, const QVector < QVector2D > &t, const QVector < Facet > &f, bool append);
    void overwriteIdentity();
    void loadIdentity();

    QPair < QVector3D, QVector3D > bounds();
    QPair < QVector3D, QVector3D > bounds(uint id_group);
    QVector3D dimension();
    QVector3D dimension(uint id_group);
    QVector3D centralPoint();
    QVector3D centralPoint(uint id_group);
    double unitScaleFactor();
    double unitScaleFactor(uint id_group);
    double unitize();
    double unitize(uint id_group);
    void translate(const QVector3D &v);
    void translate(const QVector3D &v, uint id_group);
    void scale(const QVector3D &v);
    void scale(const QVector3D &v, uint id_group);

    void loadOBJ(const QString &file_path, bool append);
    void createTerrain(const QImage &image, uint id_group, bool append);

    void draw(QColor color, bool rend_lines, bool rend_smooth, bool use_material, bool use_texture, bool show_normals);

//////////////////////////////////

};

#endif
