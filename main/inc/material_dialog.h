#ifndef __MATERIAL_DIALOG_H__
#define __MATERIAL_DIALOG_H__

#include <ec_core.h>
#include "scene.h"


class MaterialDialog : public QDialog {

private:

    Q_OBJECT

    QListWidget *list_names;

protected:

    void keyPressEvent(QKeyEvent *event);

private slots:

    void loadTexture();
    void createSelection(const QList < QPair < QString, QStringList > > &list);

signals:

    void textureChanged(const QString &file_path);

public:

    MaterialDialog(Scene *scene, QWidget *parent = NULL);

};

#endif
