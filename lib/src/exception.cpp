#include <ec_core.h>


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

Exception::Exception(const QString &what) {

    msg = what;
}

// == == == == == == == == == == == == == == == == ==

const QString Exception::message() const {

    return
    "Runtime error occurred during the procedure:\n" + msg + "\n"
    "Please contact the application's support team for more information.\n";
}

// == == == == == == == == == == == == == == == == ==

InputException::InputException(const QString &what) : Exception(what) {}

// == == == == == == == == == == == == == == == == ==

const QString InputException::message() const {

    return msg;
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
