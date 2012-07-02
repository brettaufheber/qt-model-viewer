#include "../inc/gl_model.h"


void glModel::draw(QColor color, bool rend_lines, bool rend_smooth, bool use_material, bool use_texture, bool show_normals) {

    QVectorIterator < Group > iter(groups);
    float color_ambient_diffuse[4];
    float material_off[] = { 0.0, 0.0, 0.0, 1.0 };
    float shininess_off = 0.0;

    color_ambient_diffuse[0] = color.redF();
    color_ambient_diffuse[1] = color.greenF();
    color_ambient_diffuse[2] = color.blueF();
    color_ambient_diffuse[3] = color.alphaF();

    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);

    if(rend_smooth)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);

    while(iter.hasNext()) {

        const Group *group = &iter.next();
        const Material *material = &materials[group->id_material];

        glEnable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(use_material) {

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->emissive);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);

        } else {

            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color_ambient_diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_off);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_off);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_off);
        }

        if(use_material && use_texture && material->id_texture != UNDEFINED) {

            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, material->id_texture);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glVertexPointer(3, GL_FLOAT, 0, group->seq_vertices.data());
            glNormalPointer(GL_FLOAT, 0, group->seq_normals.data());
            glTexCoordPointer(2, GL_FLOAT, 0, group->seq_texcoords.data());

            glDrawArrays(GL_TRIANGLES, 0, group->seq_texcoords.size());

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);

            glDisable(GL_TEXTURE_2D);

        } else {

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3, GL_FLOAT, 0, (float*) vertices.data());
            glNormalPointer(GL_FLOAT, 0, (float*) normals.data());

            if(rend_lines)
                glDrawElements(GL_LINES, group->i_edges.size(), GL_UNSIGNED_INT, group->i_edges.data());
            else
                glDrawElements(GL_TRIANGLES, group->i_points.size(), GL_UNSIGNED_INT, group->i_points.data());

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
        }

        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);

        if(show_normals) {

            QVector< QVector3D > normals_t;
            double scale = 1.0d / unitScaleFactor();

            for(int i = 0; i < group->i_points.size(); i++)
                normals_t << vertices[group->i_points[i]] << (vertices[group->i_points[i]] + normals[group->i_points[i]] * scale * 0.02d);

            glEnableClientState(GL_VERTEX_ARRAY);

            glVertexPointer(3, GL_FLOAT, 0, (float*) normals_t.data());
            glDrawArrays(GL_LINES, 0, normals_t.size());

            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }

    glDisable(GL_MULTISAMPLE);
    glDisable(GL_DEPTH_TEST);
}
