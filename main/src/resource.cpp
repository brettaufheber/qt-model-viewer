#include "../inc/resource.h"


QWidget *Resource::loadWidget(QWidget *parent, const QString &name) {

    QUiLoader loader;
    QFile file(QString(":/resources/forms/%1.ui").arg(name));
    QWidget *widget;

    file.open(QFile::ReadOnly);
    widget = loader.load(&file, parent);
    file.close();

    return widget;
}

QIcon Resource::loadIcon(const QString &name) {

    QIcon icon;

    icon.addFile(QString(":/resources/icons/%1.16x16.png").arg(name), QSize(16, 16));
    icon.addFile(QString(":/resources/icons/%1.22x22.png").arg(name), QSize(22, 22));
    icon.addFile(QString(":/resources/icons/%1.32x32.png").arg(name), QSize(32, 32));
    icon.addFile(QString(":/resources/icons/%1.48x48.png").arg(name), QSize(48, 48));
    icon.addFile(QString(":/resources/icons/%1.64x64.png").arg(name), QSize(64, 64));

    return icon;
}
