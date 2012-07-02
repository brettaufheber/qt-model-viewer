// == == == == == == == == == == == == == == == == ==
// INCLUDES
// == == == == == == == == == == == == == == == == ==

#include <QtWidgets>
#include <ec_vector.h>
#include <typeinfo>

// == == == == == == == == == == == == == == == == ==
// MACROS
// == == == == == == == == == == == == == == == == ==

#define IS_BIT(val, ofs)            (val & (1 << ofs))
#define SET_BIT(val, set, ofs)      val = set ? val | (1 << ofs) : val & ~(1 << ofs);
#define TOGGLE_BIT(val, ofs)        val = (val & (1 << ofs)) ? val & ~(1 << ofs) : val | (1 << ofs);
#define SWAP(p, q)                  p ^= q ^= p ^= q;

#define SUCCESS    (uint)  0
#define FAILURE    (uint) -1
#define UNDEFINED  (uint) -1

// == == == == == == == == == == == == == == == == ==

#ifndef __EC_CORE_H__
#define __EC_CORE_H__


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

typedef struct {

    uint secs;
    uint msecs;
    uint usecs;

}TimeInfo;

// == == == == == == == == == == == == == == == == ==

template < typename type_1 >
inline void release(type_1 &p) {

    if(p) {

        delete p;
        p = NULL;
    }
}

// == == == == == == == == == == == == == == == == ==

template < typename type_1, typename type_2 >
inline void release(type_2 &p) {

    if(p) {

        delete (static_cast < type_1 > (p));
        p = NULL;
    }
}

// == == == == == == == == == == == == == == == == ==

template < typename type_1 >
inline void releaseArray(type_1 &p) {

    if(p) {

        delete[] p;
        p = NULL;
    }
}

// == == == == == == == == == == == == == == == == ==

template < typename type_1, typename type_2 >
inline void releaseArray(type_2 &p) {

    if(p) {

        delete[] (static_cast < type_1 > (p));
        p = NULL;
    }
}

// == == == == == == == == == == == == == == == == ==

class Exception {

protected:

//////////////////////////////////
// Data

    QString msg;

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    Exception(const QString &what);

//////////////////////////////////
// Procedures

    const QString message() const;

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

class InputException : public Exception {

public:

//////////////////////////////////
// Constructors and Destructors

    InputException(const QString &what);

//////////////////////////////////
// Procedures

    const QString message() const;

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

class sharedLibrary {

private:

//////////////////////////////////
// Data

    void *lib_handle;
    bool is_open;

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    sharedLibrary(const QString &library);
    ~sharedLibrary();

//////////////////////////////////
// Procedures

    bool isOpen();
    void *loadSymbol(const QString &identifier);

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

class Clock {

private:

//////////////////////////////////
// Data

    qulonglong frequency;
    TimeInfo ti;

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    Clock();
    ~Clock();

//////////////////////////////////
// Procedures

    void setCounter();
    TimeInfo getCounter();

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

class Application : public QApplication {

public:

//////////////////////////////////
// Constructors and Destructors

    Application(int &argc, char **argv);
    virtual ~Application();

//////////////////////////////////
// Procedures

    virtual bool notify(QObject *receiver, QEvent *event);

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

class CliApplication : public QCoreApplication {

private:

//////////////////////////////////
// Data

    typedef void (*fpt_void)(void);
    typedef void (*fpt_boolean)(bool);
    typedef void (*fpt_string)(const QString&);
    typedef void (*fpt_list)(const QStringList&);
    typedef void (*fpt_unbound)(const QStringList&);

    typedef enum {

        Void,
        Boolean,
        String,
        List,
        Unbound

    } Parameter;

    typedef struct {

        bool status;

    } pStatus;

    typedef struct {

        QString string;

    } pString;

    typedef struct {

        QStringList list;

    } pList;

    typedef struct {

        void *fn, *property;
        Parameter type;
        bool accessed;
        QString short_name, full_name;

    } Procedure;

    QList < Procedure > store;

//////////////////////////////////
// Procedures

    void call(const Procedure &proc);
    void insert(Procedure &proc, const QString value);
    void parse();

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    CliApplication(int &argc, char **argv);
    virtual ~CliApplication();

//////////////////////////////////
// Procedures

    virtual bool notify(QObject *receiver, QEvent *event);

    void argVoid(const QString &short_name, const QString &full_name, void (*fn)(void));
    void argBoolean(const QString &short_name, const QString &full_name, void (*fn)(bool), bool default_status);
    void argString(const QString &short_name, const QString &full_name, void (*fn)(const QString &args));
    void argList(const QString &short_name, const QString &full_name, void (*fn)(const QStringList &args));
    void argUnbound(void (*fn)(const QStringList &args));

    void cli();

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

} // ecCore

#endif
