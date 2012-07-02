#include "../inc/gl_model.h"


void glModel::loadOBJ(const QString &file_path, bool append) {

    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw ecCore::InputException("glModel::glModel; cannot open OBJ file »" + file_path + "«");

    QDir dir(QFileInfo(file_path).path());
    QTextStream in(&file);

    parseOBJ(dir, in, append);

    file.close();
}

// == == == == == == == == == == == == == == == == ==

void glModel::loadMaterial(const QString &file_path) {

    QFile file(file_path);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QDir dir(QFileInfo(file_path).path());
        QTextStream in(&file);

        parseMTL(dir, in);

        file.close();

    } else
        qWarning() << "Warning: cannot open MTL file »" + file_path + "«\n";
}

// == == == == == == == == == == == == == == == == ==

void glModel::parseOBJ(const QDir &dir, QTextStream &in, bool append) {

    QRegExp general_pattern("^((v(\\s+\\S+){3})|(vt(\\s+\\S+){2}(\\s+0(\\.0+)?)?)|(f(\\s+\\S+){3,})|((g|mtllib)\\s+\\S.*)|(usemtl(\\s+\\S+)?))$");

    QVector < QVector3D > vertices;
    QVector < QVector2D > texcoords;
    QVector < Facet > facets;

    uint id_material = findMaterial("default");

    if(id_material == FAILURE)
        id_material = addMaterial("default");

    uint id_group = findGroup("default");

    if(id_group == FAILURE)
        id_group = addGroup("default", id_material);
    else
        setMaterial(id_group, id_material);

    uint counter_line = 0;

    while (!in.atEnd()) {

        QString line = in.readLine().trimmed();
        counter_line++;

        if(line.isEmpty() || line[0] == '#')
            continue;
        else if(general_pattern.indexIn(line) != -1) {

            QTextStream ss(&line);
            QString arg;

            ss >> arg;

            if(arg == "v") {

                qreal x, y, z;
                ss >> x;
                ss >> y;
                ss >> z;

                vertices.append(QVector3D(x, y, z));

            } else if(arg == "vt") {

                qreal u, v;
                ss >> u;
                ss >> v;

                texcoords.append(QVector2D(u, v));

            } else if(arg == "mtllib") {

                QString name = ss.readLine().trimmed();

                loadMaterial(dir.filePath(name));

            } else if(arg == "usemtl") {

                QString name;

                if(ss.atEnd())
                    name = "default";
                else
                    ss >> name;

                if(name == "default" || name == "null" || name == "(null)") {

                    id_material = findMaterial("default");
                    setMaterial(id_group, id_material);

                } else {

                    uint id = findMaterial(name);

                    if(id != FAILURE) {

                        id_material = id;
                        setMaterial(id_group, id_material);

                    } else
                        qWarning() << "Warning: cannot find material »" + name + "« (line " << counter_line << ")\n";
                }

            } else if(arg == "g") {

                QString name = ss.readLine().trimmed();

                id_group = findGroup(name);

                if(id_group == FAILURE)
                    id_group = addGroup(name, id_material);

            } else if(arg == "f") {

                QRegExp facet_pattern_v("^(f(\\s+\\-?\\d+){3,})$");
                QRegExp facet_pattern_vt("^(f((\\s+\\-?\\d+/\\-?\\d+){3,}|(\\s+\\-?\\d+/\\-?\\d+/\\-?\\d+){3,}))$");

                if(facet_pattern_v.indexIn(line) != -1) {

                    Facet facet;
                    QString facet_arg;

                    facet.id_group = id_group;
                    facet.use_texture = false;

                    while(!ss.atEnd()) {

                        ss >> facet_arg;

                        long index_v = facet_arg.toLong();

                        if(index_v != 0)
                            facet.points.append(QPair < uint, uint > (index_v > 0 ? index_v - 1 : vertices.size() + index_v, 0));
                        else
                            qWarning() << "Warning: incorrect indices are ignored (line " << counter_line << ")\n";
                    }

                    facets.append(facet);

                } else if(facet_pattern_vt.indexIn(line) != -1) {

                    Facet facet;
                    QString facet_arg;

                    facet.id_group = id_group;
                    facet.use_texture = true;

                    while(!ss.atEnd()) {

                        ss >> facet_arg;

                        QStringList facet_args = facet_arg.split('/');
                        long index_v = facet_args.at(0).toLong();
                        long index_t = facet_args.at(1).toLong();

                        if(index_v != 0 && index_t != 0)
                            facet.points.append(QPair < uint, uint > (index_v > 0 ? index_v - 1 : vertices.size() + index_v, index_t > 0 ? index_t - 1 : texcoords.size() + index_t));
                        else
                            qWarning() << "Warning: incorrect indices are ignored (line " << counter_line << ")\n";
                    }

                    facets.append(facet);
                }

            } else
                throw ecCore::Exception("glModel::parseOBJ; parser error occurred in line " + counter_line);
        }
    }

    if(vertices.size() == 0 || facets.size() == 0)
        throw ecCore::InputException("glModel::parseOBJ; cannot find information in the file");

    setGeometry(vertices, texcoords, facets, append);
}

// == == == == == == == == == == == == == == == == ==

void glModel::parseMTL(const QDir &dir, QTextStream &in) {

    QRegExp general_pattern("^(((Ka|Kd|Ke|Ks)(\\s+\\S+){3})|((Ns|Tr|d)\\s+\\S+)|(map_Kd\\s+\\S.*)|(newmtl(\\s+\\S+)?))$");

    uint id_material = findMaterial("default");
    uint counter_line = 0;

    while (!in.atEnd()) {

        QString line = in.readLine().trimmed();
        counter_line++;

        if(line.isEmpty() || line[0] == '#')
            continue;
        else if(general_pattern.indexIn(line) != -1) {

            QTextStream ss(&line);
            QString arg;

            ss >> arg;

            if(arg == "Ka") {

                ss >> ambient(id_material)[0];
                ss >> ambient(id_material)[1];
                ss >> ambient(id_material)[2];

            } else if(arg == "Kd") {

                ss >> diffuse(id_material)[0];
                ss >> diffuse(id_material)[1];
                ss >> diffuse(id_material)[2];

            } else if(arg == "Ke") {

                ss >> emission(id_material)[0];
                ss >> emission(id_material)[1];
                ss >> emission(id_material)[2];

            } else if(arg == "Ks") {

                ss >> specular(id_material)[0];
                ss >> specular(id_material)[1];
                ss >> specular(id_material)[2];

            } else if(arg == "Ns") {

                ss >> shininess(id_material);

            } else if(arg == "Tr" || arg == "d") {

                ss >> diffuse(id_material)[3];

            } else if(arg == "newmtl") {

                QString name;

                if(!ss.atEnd()) {

                    ss >> name;

                    if(findMaterial(name) == FAILURE)
                        id_material = addMaterial(name);
                    else
                        qWarning() << "Warning: material already exists »" + name + "« (line " << counter_line << ")\n";

                } else
                    id_material = findMaterial("default");

            } else if(arg == "map_Kd") {

                QString name = ss.readLine().trimmed();
                QPixmap pixmap;

                if(pixmap.load(dir.filePath(name)))
                    bindTexture(pixmap, QDir::cleanPath(dir.filePath(name)), id_material);
                else
                    qWarning() << "Warning: cannot open texture »" + name + "« (line " << counter_line << ")\n";

            } else
                throw ecCore::Exception("glModel::parseMTL; parser error occurred in line " + counter_line);

        } else
            qWarning() << "Warning: cannot match pattern in line " << counter_line << " » " << line << "«\n";
    }
}
