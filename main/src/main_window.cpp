#include "../inc/main_window.h"
#include "../inc/graphics_view.h"
#include "../inc/resource.h"


MainWindow::MainWindow() {

    // initialise

    setCentralWidget(new QWidget());

    QGraphicsView *view = new GraphicsView();
    QGLWidget *gl_widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    scene = new Scene(gl_widget);

    view->setViewport(gl_widget);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setScene(scene);

    QWidget *widget_form = Resource::loadWidget(this, "model");
    material_dialog = new MaterialDialog(scene, this);

    QHBoxLayout *layout = new QHBoxLayout;

    QAction *action_open_model = new QAction("Load Model From OBJ", this);
    QAction *action_open_terrain = new QAction("Load Terrain From Image", this);
    QAction *action_clean_up = new QAction("Unload", this);
    QAction *action_exit = new QAction("Exit", this);
    QAction *action_foreground_color = new QAction("Choose Foreground Color", this);
    QAction *action_background_color = new QAction("Choose Background Color", this);
    QAction *action_materials = new QAction("Edit Materials", this);
    QAction *action_about = new QAction("About", this);
    QAction *action_about_qt = new QAction("About Qt", this);

    label_points = findChild < QLabel* > ("label_points");
    label_edges = findChild < QLabel* > ("label_edges");
    label_facets = findChild < QLabel* > ("label_facets");

    QRadioButton *radio_button_lines = findChild < QRadioButton* > ("radio_button_lines");
    QRadioButton *radio_button_flat = findChild < QRadioButton* > ("radio_button_flat");
    QRadioButton *radio_button_smooth = findChild < QRadioButton* > ("radio_button_smooth");

    QRadioButton *radio_button_color = findChild < QRadioButton* > ("radio_button_color");
    QRadioButton *radio_button_material = findChild < QRadioButton* > ("radio_button_material");

    check_box_texture = findChild < QCheckBox* > ("check_box_texture");
    QCheckBox *check_box_normals = findChild < QCheckBox* > ("check_box_normals");

    QDoubleSpinBox *spin_box_scale_x = findChild < QDoubleSpinBox* > ("spin_box_scale_x");
    QDoubleSpinBox *spin_box_scale_y = findChild < QDoubleSpinBox* > ("spin_box_scale_y");
    QDoubleSpinBox *spin_box_scale_z = findChild < QDoubleSpinBox* > ("spin_box_scale_z");

    QDial *dial_x = findChild < QDial* > ("dial_x");
    QDial *dial_y = findChild < QDial* > ("dial_y");
    QDial *dial_z = findChild < QDial* > ("dial_z");

    // create menu

    QMenu *menu_file = menuBar()->addMenu("&File");
    menu_file->addAction(action_open_model);
    menu_file->addAction(action_open_terrain);
    menu_file->addAction(action_clean_up);
    menu_file->addSeparator();
    menu_file->addAction(action_exit);

    QMenu *menu_scene = menuBar()->addMenu("&Scene");
    menu_scene->addAction(action_foreground_color);
    menu_scene->addAction(action_background_color);
    menu_scene->addAction(action_materials);

    QMenu *menu_help = menuBar()->addMenu("&Help");
    menu_help->addAction(action_about);
    menu_help->addAction(action_about_qt);

    // signal slot handling

    // mode
    connect(radio_button_lines, SIGNAL(toggled(bool)), this, SLOT(setModeLines(bool)));
    connect(radio_button_lines, SIGNAL(clicked()), scene, SLOT(setModeLines()));
    connect(radio_button_flat, SIGNAL(clicked()), scene, SLOT(setModeFlat()));
    connect(radio_button_smooth, SIGNAL(clicked()), scene, SLOT(setModeSmooth()));
    connect(radio_button_color, SIGNAL(clicked()), scene, SLOT(setModeColor()));
    connect(radio_button_material, SIGNAL(clicked()), scene, SLOT(setModeMaterial()));

    // additional things
    connect(check_box_texture, SIGNAL(toggled(bool)), scene, SLOT(setModeTexture(bool)));
    connect(check_box_normals, SIGNAL(toggled(bool)), scene, SLOT(setModeNormals(bool)));

    // scale
    connect(spin_box_scale_x, SIGNAL(valueChanged(double)), scene, SLOT(setXScale(double)));
    connect(spin_box_scale_y, SIGNAL(valueChanged(double)), scene, SLOT(setYScale(double)));
    connect(spin_box_scale_z, SIGNAL(valueChanged(double)), scene, SLOT(setZScale(double)));
    connect(scene, SIGNAL(xScaleChanged(double)), spin_box_scale_x, SLOT(setValue(double)));
    connect(scene, SIGNAL(yScaleChanged(double)), spin_box_scale_y, SLOT(setValue(double)));
    connect(scene, SIGNAL(zScaleChanged(double)), spin_box_scale_z, SLOT(setValue(double)));

    // rotation
    connect(dial_x, SIGNAL(valueChanged(int)), scene, SLOT(setXRotation(int)));
    connect(dial_y, SIGNAL(valueChanged(int)), scene, SLOT(setYRotation(int)));
    connect(dial_z, SIGNAL(valueChanged(int)), scene, SLOT(setZRotation(int)));
    connect(scene, SIGNAL(xRotationChanged(int)), dial_x, SLOT(setValue(int)));
    connect(scene, SIGNAL(yRotationChanged(int)), dial_y, SLOT(setValue(int)));
    connect(scene, SIGNAL(zRotationChanged(int)), dial_z, SLOT(setValue(int)));

    // menu
    connect(action_open_model, SIGNAL(triggered()), this, SLOT(loadModel()));
    connect(action_open_terrain, SIGNAL(triggered()), this, SLOT(loadTerrain()));
    connect(action_clean_up, SIGNAL(triggered()), this, SLOT(unload()));
    connect(action_clean_up, SIGNAL(triggered()), scene, SLOT(unload()));
    connect(action_foreground_color, SIGNAL(triggered()), scene, SLOT(setForegroundColor()));
    connect(action_background_color, SIGNAL(triggered()), scene, SLOT(setBackgroundColor()));
    connect(action_materials, SIGNAL(triggered()), this, SLOT(setMaterials()));
    connect(action_exit, SIGNAL(triggered()), this, SLOT(close()));
    connect(action_about, SIGNAL(triggered()), this, SLOT(about()));
    connect(action_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // set layout

    layout->addWidget(view);
    layout->addWidget(widget_form);
    centralWidget()->setLayout(layout);

    // set defaults

    scene->setModeSmooth();
    scene->setModeColor();
    scene->setModeTexture(false);
    scene->setModeNormals(false);

    string_label_points = label_points->text();
    string_label_edges = label_edges->text();
    string_label_facets = label_facets->text();

    setInfo(0, 0, 0);

    dial_x->setValue(20);
    dial_y->setValue(340);
    dial_z->setValue(0);

    setWindowIcon(Resource::loadIcon("app"));
    setWindowTitle(PROJECT);
}

void MainWindow::setInfo(uint points, uint edges, uint facets) {

    label_points->setText(string_label_points.arg(points));
    label_edges->setText(string_label_edges.arg(edges));
    label_facets->setText(string_label_facets.arg(facets));
}

void MainWindow::setMaterials() {

    if(scene->isLoaded()) {

        material_dialog->show();
        material_dialog->raise();
        material_dialog->activateWindow();
   }
}

void MainWindow::setModeLines(bool enabled) {

    check_box_texture->setEnabled(!enabled);

    if(enabled)
        check_box_texture->setChecked(false);
}

void MainWindow::loadModel() {

    QString file_path = QFileDialog::getOpenFileName(this, "Choose Model");

    if(!file_path.isEmpty()) {

        scene->loadModel(file_path);
        setInfo(scene->numPoints(), scene->numEdges(), scene->numFacets());
    }
}

void MainWindow::loadTerrain() {

    QString file_path = QFileDialog::getOpenFileName(this, "Choose Image");

    if(!file_path.isEmpty()) {

        scene->loadTerrain(file_path);
        setInfo(scene->numPoints(), scene->numEdges(), scene->numFacets());
    }
}

void MainWindow::unload() {

    material_dialog->done(0);
    setInfo(0, 0, 0);
}

void MainWindow::about() {

    QMessageBox::about(this, "About " PROJECT,
        PROJECT " version " VERSION_MAJOR "." VERSION_MINOR "." VERSION_PATCH "\n"
        COPYRIGHT " " AUTHOR "\n\n"
        "License - GPLv3: GNU General Public License version 3\n"
        "* visit http://gnu.org/licenses/gpl.html for details\n\n"
        "> download new versions from http://" WEB "\n"
        "> to reporting bugs send an e-mail: " EMAIL "\n");
}
