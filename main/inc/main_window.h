#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <ec_core.h>
#include "material_dialog.h"
#include "scene.h"


class MainWindow : public QMainWindow {

private:

    Q_OBJECT

    Scene *scene;
    MaterialDialog *material_dialog;

    QLabel *label_points;
    QLabel *label_edges;
    QLabel *label_facets;
    QCheckBox *check_box_texture;

    QString string_label_points;
    QString string_label_edges;
    QString string_label_facets;

    void setInfo(uint points, uint edges, uint facets);

private slots:

    void setMaterials();
    void setModeLines(bool enabled);
    void loadModel();
    void loadTerrain();
    void unload();
    void about();

public:

    MainWindow();

};

#endif
