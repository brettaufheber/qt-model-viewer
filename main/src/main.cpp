#include "../inc/main_window.h"


int main(int argc, char **argv) {

    Q_INIT_RESOURCE(resource);

    ecCore::Application app(argc, argv);

    MainWindow window;

    window.show();

    return app.exec();
}
