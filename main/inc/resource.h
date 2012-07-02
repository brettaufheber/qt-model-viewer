#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <ec_core.h>
#include <QUiLoader>


class Resource {

public:

    static QWidget *loadWidget(QWidget *parent, const QString &name);
    static QIcon loadIcon(const QString &name);

};

#endif
