#include "../inc/gl_model.h"


void glModel::vertexSequence(uint id_group) {

    Group *group = &groups[id_group];
    QVectorIterator < Triangle > iter(group->triangles);
    QVector < QVector3D > seq_vertices;

    group->seq_vertices.clear();

    while(iter.hasNext()) {

        Triangle triangle = iter.next();

        if(triangle.use_uv)
            group->seq_vertices << vertices[triangle.xyz[0]] << vertices[triangle.xyz[1]] << vertices[triangle.xyz[2]];
        else
            seq_vertices << vertices[triangle.xyz[0]] << vertices[triangle.xyz[1]] << vertices[triangle.xyz[2]];
    }

    group->seq_vertices << seq_vertices;
}

// == == == == == == == == == == == == == == == == ==

void glModel::normalSequence(uint id_group) {

    Group *group = &groups[id_group];
    QVectorIterator < Triangle > iter(group->triangles);
    QVector < QVector3D > seq_normals;

    group->seq_normals.clear();

    while(iter.hasNext()) {

        Triangle triangle = iter.next();

        if(triangle.use_uv)
            group->seq_normals << normals[triangle.xyz[0]] << normals[triangle.xyz[1]] << normals[triangle.xyz[2]];
        else
            seq_normals << normals[triangle.xyz[0]] << normals[triangle.xyz[1]] << normals[triangle.xyz[2]];
    }

    group->seq_normals << seq_normals;
}

// == == == == == == == == == == == == == == == == ==

void glModel::texcoordSequence(uint id_group) {

    Group *group = &groups[id_group];
    QVectorIterator < Triangle > iter(group->triangles);

    group->seq_texcoords.clear();

    while(iter.hasNext()) {

        Triangle triangle = iter.next();

        if(triangle.use_uv)
            group->seq_texcoords << texcoords[triangle.uv[0]] << texcoords[triangle.uv[1]] << texcoords[triangle.uv[2]];
    }
}

// == == == == == == == == == == == == == == == == ==

void glModel::setGeometry(const QVector < QVector3D > &v, const QVector < QVector2D > &t, const QVector < Facet > &f, bool append) {

/*  => right-handed coordinate system
 *         _________________
 *      6 /|             7 /|
 *       / |              / |
 *      /  |             /  |
 *     /   |            /   |
 *    /____|___________/    |
 *  2 |    |         3 |    |
 *    |    |           |    |
 *    |  4 |           |  5 |
 *    |    |___________|____|
 *    |   /            |   /
 *    |  /             |  /
 *  0 | /            1 | /
 *    |/_______________|/
 *
 *   creation of edges:
 *
 *    front facet:  0 - 1, 1 - 3
 *    back facet:   4 - 6, 6 - 7
 *    upper facet:  2 - 3, 3 - 7
 *    lower facet:  0 - 4, 4 - 5
 *    left facet:   0 - 2, 2 - 6
 *    right facet:  1 - 5, 5 - 7
 */

    uint size_old_v;
    uint size_old_t;

    // append or overwrite vertices and texture coordinate

    if(append) {

        size_old_v = vertices.size();
        size_old_t = texcoords.size();

        vertices << v;
        identity << v;
        texcoords << t;

    } else {

        size_old_v = 0;
        size_old_t = 0;

        vertices = v;
        identity = v;
        texcoords = t;

        for(uint i = 0; i < (uint) groups.size(); i++) {

            groups[i].i_edges.clear();
            groups[i].i_points.clear();
            groups[i].i_texcoords.clear();
            groups[i].triangles.clear();
        }
    }

    normals = QVector < QVector3D > (vertices.size());

    // traverse facets

    QVectorIterator < Facet > iter_facets(f);

    while(iter_facets.hasNext()) {

        Facet facet = iter_facets.next();
        Group *group = &groups[facet.id_group];

        // create edges

        for(int i = 0; i < facet.points.size(); i++) {

            uint edge_a = facet.points[i].first + size_old_v;
            uint edge_b = facet.points[(i + 1) % facet.points.size()].first + size_old_v;

            if(edge_a < edge_b)
                group->i_edges << edge_a << edge_b;
        }

        /* triangulation for convex facets
         *
         * triangle[i].points[0] := triangle[i - 1].points[0]
         * triangle[i].points[1] := triangle[i - 1].points[2]
         * triangle[i].points[2] := next
         */

        if(facet.points.size() < 3)
            throw ecCore::Exception("glModel::setGeometry; inconsistent data");

        QVectorIterator < QPair < uint, uint > > iter_points(facet.points);
        Triangle triangle;

        triangle.use_uv = facet.use_texture;

        for(uint i = 0; i < 3; i++) {

            QPair < uint, uint > point = iter_points.next();

            triangle.xyz[i] = point.first + size_old_v;
            group->i_points << triangle.xyz[i];

            if(facet.use_texture) {

                triangle.uv[i] = point.second + size_old_t;
                group->i_texcoords << triangle.uv[i];
            }
        }

        group->triangles << triangle;

        uint iv = group->i_points.size() - 3;
        uint it = group->i_texcoords.size() - 3;

        while(iter_points.hasNext()) {

            QPair < uint, uint > point = iter_points.next();
            Triangle triangle;

            triangle.use_uv = facet.use_texture;

            triangle.xyz[0] = group->i_points[iv];
            triangle.xyz[1] = group->i_points[iv + 2];
            triangle.xyz[2] = point.first + size_old_v;

            group->i_points << triangle.xyz[0] << triangle.xyz[1] << triangle.xyz[2];

            if(facet.use_texture) {

                triangle.uv[0] = group->i_texcoords[it];
                triangle.uv[1] = group->i_texcoords[it + 2];
                triangle.uv[2] = point.second + size_old_t;

                group->i_texcoords << triangle.uv[0] << triangle.uv[1] << triangle.uv[2];
            }

            group->triangles << triangle;
            iv += 3;
            it += 3;
        }
    }

    // calculate normals

    QVectorIterator < Group > iter_groups(groups);

    while(iter_groups.hasNext()) {

        Group group = iter_groups.next();

        for(int i = 0; i < group.i_points.size(); i += 3) {

            QVector3D a = vertices.at(group.i_points.at(i + 0));
            QVector3D b = vertices.at(group.i_points.at(i + 1));
            QVector3D c = vertices.at(group.i_points.at(i + 2));

            QVector3D normal = QVector3D::crossProduct(b - a, c - a).normalized();

            normals[group.i_points.at(i + 0)] += normal;
            normals[group.i_points.at(i + 1)] += normal;
            normals[group.i_points.at(i + 2)] += normal;
        }
    }

    for(int i = 0; i < normals.size(); i++)
        normals[i].normalize();

    // create triangle sequence

    for(uint i = 0; i < (uint) groups.size(); i++) {

        vertexSequence(i);
        normalSequence(i);
        texcoordSequence(i);
    }
}

// == == == == == == == == == == == == == == == == ==

void glModel::overwriteIdentity() {

    identity = vertices;
}

// == == == == == == == == == == == == == == == == ==

void glModel::loadIdentity() {

    vertices = identity;

    // create vertex sequence
    for(uint i = 0; i < (uint) groups.size(); i++)
        vertexSequence(i);
}

// == == == == == == == == == == == == == == == == ==

QPair < QVector3D, QVector3D > glModel::bounds() {

    QVectorIterator < QVector3D > iter_vertices(vertices);
    QVector3D bounds_max;
    QVector3D bounds_min;

    if(iter_vertices.hasNext()) {

        bounds_max = bounds_min = iter_vertices.next();

        while(iter_vertices.hasNext()) {

            QVector3D p = iter_vertices.next();

            bounds_min.setX(qMin(bounds_min.x(), p.x()));
            bounds_max.setX(qMax(bounds_max.x(), p.x()));
            bounds_min.setY(qMin(bounds_min.y(), p.y()));
            bounds_max.setY(qMax(bounds_max.y(), p.y()));
            bounds_min.setZ(qMin(bounds_min.z(), p.z()));
            bounds_max.setZ(qMax(bounds_max.z(), p.z()));
        }
    }

    return QPair < QVector3D, QVector3D > (bounds_max, bounds_min);
}

// == == == == == == == == == == == == == == == == ==

QPair < QVector3D, QVector3D > glModel::bounds(uint id_group) {

    QVectorIterator < uint > iter_points(groups[id_group].i_points);
    QVector3D bounds_max;
    QVector3D bounds_min;

    if(iter_points.hasNext()) {

        bounds_max = bounds_min = vertices[iter_points.next()];

        while(iter_points.hasNext()) {

            QVector3D p = vertices[iter_points.next()];

            bounds_min.setX(qMin(bounds_min.x(), p.x()));
            bounds_max.setX(qMax(bounds_max.x(), p.x()));
            bounds_min.setY(qMin(bounds_min.y(), p.y()));
            bounds_max.setY(qMax(bounds_max.y(), p.y()));
            bounds_min.setZ(qMin(bounds_min.z(), p.z()));
            bounds_max.setZ(qMax(bounds_max.z(), p.z()));
        }
    }

    return QPair < QVector3D, QVector3D > (bounds_max, bounds_min);
}

// == == == == == == == == == == == == == == == == ==

QVector3D glModel::dimension() {

    QPair < QVector3D, QVector3D > b = bounds();

    // calculate width, height, and depth
    return b.first - b.second;
}

// == == == == == == == == == == == == == == == == ==

QVector3D glModel::dimension(uint id_group) {

    QPair < QVector3D, QVector3D > b = bounds(id_group);

    // calculate width, height, and depth
    return b.first - b.second;
}

// == == == == == == == == == == == == == == == == ==

QVector3D glModel::centralPoint() {

    QPair < QVector3D, QVector3D > b = bounds();

    // calculate the center
    return (b.first + b.second) * 0.5d;
}

// == == == == == == == == == == == == == == == == ==

QVector3D glModel::centralPoint(uint id_group) {

    QPair < QVector3D, QVector3D > b = bounds(id_group);

    // calculate the center
    return (b.first + b.second) * 0.5d;
}

// == == == == == == == == == == == == == == == == ==

double glModel::unitScaleFactor() {

    QVector3D dim = dimension();

    double max = qMax(dim.x(), qMax(dim.y(), dim.z()));

    if(max == 0)
        throw ecCore::Exception("glModel::unitScaleFactor; division by zero");

    // calculate unitizing scale factor
    return 1.0d / max;
}

// == == == == == == == == == == == == == == == == ==

double glModel::unitScaleFactor(uint id_group) {

    QVector3D dim = dimension(id_group);

    double max = qMax(dim.x(), qMax(dim.y(), dim.z()));

    if(max == 0)
        throw ecCore::Exception("glModel::unitScaleFactor; division by zero");

    // calculate unitizing scale factor
    return 1.0d / max;
}

// == == == == == == == == == == == == == == == == ==

double glModel::unitize() {

    QPair < QVector3D, QVector3D > b = bounds();

    // calculate width, height, and depth
    QVector3D dimension = b.first - b.second;

    // calculate the center
    QVector3D center = (b.first + b.second) * 0.5d;

    double max = qMax(dimension.x(), qMax(dimension.y(), dimension.z()));

    if(max == 0)
        throw ecCore::Exception("glModel::unitize; division by zero");

    // calculate unitizing scale factor
     double scale = 1.0d / max;

    // unitize
    for(uint i = 0; i < (uint) vertices.size(); i++)
        vertices[i] = (vertices[i] - center) * scale;

    // create vertex sequence
    for(uint i = 0; i < (uint) groups.size(); i++)
        vertexSequence(i);

    return scale;
}

// == == == == == == == == == == == == == == == == ==

double glModel::unitize(uint id_group) {

    QPair < QVector3D, QVector3D > b = bounds(id_group);

    // calculate width, height, and depth
    QVector3D dimension = b.first - b.second;

    // calculate the center
    QVector3D center = (b.first + b.second) * 0.5d;

    double max = qMax(dimension.x(), qMax(dimension.y(), dimension.z()));

    if(max == 0)
        throw ecCore::Exception("glModel::unitize; division by zero");

    // calculate unitizing scale factor
     double scale = 1.0d / max;

    // unitize

     QVectorIterator < uint > iter_points(groups[id_group].i_points);

     while(iter_points.hasNext()) {

         uint i = iter_points.next();
         vertices[i] = (vertices[i] - center) * scale;
     }

    // create vertex sequence
    vertexSequence(id_group);

    return scale;
}

// == == == == == == == == == == == == == == == == ==

void glModel::translate(const QVector3D &v) {

    for(uint i = 0; i < (uint) vertices.size(); i++)
        vertices[i] += v;

    // create vertex sequence
    for(uint i = 0; i < (uint) groups.size(); i++)
        vertexSequence(i);
}

// == == == == == == == == == == == == == == == == ==

void glModel::translate(const QVector3D &v, uint id_group) {

    QVectorIterator < uint > iter_points(groups[id_group].i_points);

    while(iter_points.hasNext())
        vertices[iter_points.next()] += v;

    // create vertex sequence
    vertexSequence(id_group);
}

// == == == == == == == == == == == == == == == == ==

void glModel::scale(const QVector3D &v) {

    for(uint i = 0; i < (uint) vertices.size(); i++)
        vertices[i] *= v;

    // create vertex sequence
    for(uint i = 0; i < (uint) groups.size(); i++)
        vertexSequence(i);
}

// == == == == == == == == == == == == == == == == ==

void glModel::scale(const QVector3D &v, uint id_group) {

    QVectorIterator < uint > iter_points(groups[id_group].i_points);

    while(iter_points.hasNext())
        vertices[iter_points.next()] *= v;

    // create vertex sequence
    vertexSequence(id_group);
}
