#include "../inc/gl_model.h"


void glModel::createTerrain(const QImage &image, uint id_group, bool append) {

    QVector < QVector3D > vertices;
    QVector < QVector2D > texcoords;
    QVector < Facet > facets;

    if(image.width() <= 1 || image.height() <= 1)
        throw ecCore::InputException("glModel::createTerrain; image to small");

    for(uint y = 0; y < (uint) image.height(); y++) {

        for(uint x = 0; x < (uint) image.width(); x++) {

            QColor pixel(image.pixel(x, y));

            vertices.append(QVector3D(x, (pixel.red() + pixel.blue() + pixel.green()) / 3, y));
            texcoords.append(QVector2D((double) x / (double) (image.width() - 1), (double) (image.height() - y -1) / (double) (image.height() - 1)));
        }
    }

    for(uint i = 0; i < (uint) image.width() * (image.height() - 1); i++) {

        if((i + 1) % image.width() == 0)
            continue;

        Facet facet;

        facet.id_group = id_group;
        facet.use_texture = true;

        facet.points.append(QPair < uint, uint > (i, i));
        facet.points.append(QPair < uint, uint > (i + image.width(), i + image.width()));
        facet.points.append(QPair < uint, uint > (i + image.width() + 1, i + image.width() + 1));
        facet.points.append(QPair < uint, uint > (i + 1, i + 1));

        facets.append(facet);
    }

    setGeometry(vertices, texcoords, facets, append);
}
