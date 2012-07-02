#include <ec_core.h>

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <dlfcn.h>
#endif


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

sharedLibrary::sharedLibrary(const QString &library) {

#ifdef Q_WS_WIN

    lib_handle = (void*) LoadLibrary((LPCWSTR)library.toStdString().c_str());

#else

    lib_handle = (void*) dlopen(library.toStdString().c_str(), RTLD_LAZY);

#endif

    if(lib_handle == NULL)
        is_open = false;
    else
        is_open = true;
}

// == == == == == == == == == == == == == == == == ==

sharedLibrary::~sharedLibrary() {

#ifdef Q_WS_WIN

    if(!FreeLibrary((HINSTANCE) lib_handle))
        throw Exception("sharedLibrary::~sharedLibrary; invalid domain");

#else

    if(dlclose(lib_handle) == -1)
        throw Exception("sharedLibrary::~sharedLibrary; invalid domain");

#endif

    is_open = false;
}

// == == == == == == == == == == == == == == == == ==

bool sharedLibrary::isOpen() {

    return is_open;
}

// == == == == == == == == == == == == == == == == ==

void *sharedLibrary::loadSymbol(const QString &identifier) {

    void *fref;

#ifdef Q_WS_WIN

    fref = (void*) GetProcAddress((HINSTANCE)lib_handle, identifier.toStdString().c_str());

#else

    dlerror(); // clear error code

    fref = (void*) dlsym(lib_handle, identifier.toStdString().c_str());

    const char *dlsym_error = dlerror();

    if(dlsym_error)
        return NULL;

#endif

    return fref;
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
