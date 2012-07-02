#include <ec_core.h>


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {}

// == == == == == == == == == == == == == == == == ==

Application::~Application() {}

// == == == == == == == == == == == == == == == == ==

bool Application::notify(QObject *receiver, QEvent *event) {

    try {

        return QApplication::notify(receiver, event);

    } catch(InputException &e) {

        qCritical() << "Warning: " << e.message();
        QMessageBox::warning(NULL, "Warning", e.message());

    } catch(Exception &e) {

        qCritical() << e.message();
        QMessageBox::critical(NULL, "Critical Error", e.message());

        exit(FAILURE);
    }

    return false;
}

// == == == == == == == == == == == == == == == == ==

CliApplication::CliApplication(int &argc, char **argv) : QCoreApplication(argc, argv) {}

// == == == == == == == == == == == == == == == == ==

CliApplication::~CliApplication() {

    QListIterator < Procedure > iter(store);

    while(iter.hasNext()) {

        Procedure proc = iter.next();

        if(proc.type == Void || proc.type == Boolean)
            release < pStatus* > (proc.property);
        else if(proc.type == String)
            release < pString* > (proc.property);
        else if(proc.type == List || proc.type == Unbound)
            release < pList* > (proc.property);
    }
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::call(const Procedure &proc) {

    if(proc.type == Void) {

        if(((pStatus*) proc.property)->status)
            ((fpt_void) proc.fn)();

    } else if(proc.type == Boolean)
        ((fpt_boolean) proc.fn)(((pStatus*) proc.property)->status);
    else if(proc.type == String)
        ((fpt_string) proc.fn)(((pString*) proc.property)->string);
    else if(proc.type == List)
        ((fpt_list) proc.fn)(((pList*) proc.property)->list);
    else if(proc.type == Unbound)
        ((fpt_unbound) proc.fn)(((pList*) proc.property)->list);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::insert(Procedure &proc, const QString value) {

    if(proc.accessed)
        throw InputException("You have already entered this option: " + proc.full_name);

    if(proc.type == Void)
        ((pStatus*) proc.property)->status = true;
    else if(proc.type == Boolean) {

        if(value == "on" || value == "yes" || value == "true" || value == "1")
            ((pStatus*) proc.property)->status = true;
        else if (value == "off" || value == "no" || value == "false" || value == "0")
            ((pStatus*) proc.property)->status = false;
        else
            throw InputException("You entered an invalid value for argument " + proc.full_name + ": " + value);

    } else if(proc.type == String)
        ((pString*) proc.property)->string = value;
    else if(proc.type == List)
        ((pList*) proc.property)->list << value.split(":");
    else if(proc.type == Unbound)
        ((pList*) proc.property)->list << value;

    if(proc.type != Unbound)
    proc.accessed = true;
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::parse() {

/*  You can use the following notations:
 *
 *    general:
 *      -p -q = -pq = --papa --quebec
 *      -p a -q b = -pq a b = --papa=a --quebec=b
 *
 *    explicit for boolean:
 *      --romeo=<on|yes|true|1>
 *      --sierra=<off|no|false|0>
 *
 *    lists:
 *      --foxtrot a:b:c
 */

    QRegExp regexp_short("^(\\-[A-z]+)$");
    QRegExp regexp_full("^(\\-\\-[A-z]+(-)?[A-z]+)$");
    QRegExp regexp_full_assignation("^(\\-\\-[A-z]+(-)?[A-z]+(=))");
    QStringList args = arguments();
    QStringListIterator iter_args(args);

    if(args.size() <= 1)
        throw InputException("You have not enter any argument!");

    iter_args.next(); // ignore file name

    while(iter_args.hasNext()) {

        QString arg = iter_args.next();

        if(regexp_short.indexIn(arg) != -1) {

            arg = arg.right(arg.length() - 1);

            for(uint i = 0; i < (uint) arg.length(); i++) {

                QMutableListIterator < Procedure > iter_procs(store);
                bool inserted = false;

                while(iter_procs.hasNext()) {

                    Procedure *proc = &iter_procs.next();

                    if(proc->short_name == (QString) arg[i]) {

                        if(proc->type == Void)
                            insert(*proc, NULL);
                        else {

                            if(!iter_args.hasNext())
                                throw InputException("You entered not enough arguments for the given condition: " + arg);

                            insert(*proc, iter_args.next());
                        }

                        inserted = true;
                        break;
                    }
                }

                if(!inserted)
                    throw InputException("You have enter an incorrect argument: " + arg);
            }

        } else if(regexp_full.indexIn(arg) != -1) {

            QMutableListIterator < Procedure > iter_procs(store);
            bool inserted = false;

            arg = arg.right(arg.length() - 2);

            while(iter_procs.hasNext()) {

                Procedure *proc = &iter_procs.next();

                if(proc->full_name == arg) {

                    if(proc->type == Void)
                        insert(*proc, NULL);
                    else {

                        if(!iter_args.hasNext())
                            throw InputException("You entered not enough arguments for the given condition: " + arg);

                        insert(*proc, iter_args.next());
                    }

                    inserted = true;
                    break;
                }
            }

            if(!inserted)
                throw InputException("You have enter an incorrect argument: " + arg);

        } else if(regexp_full_assignation.indexIn(arg) != -1) {

            QMutableListIterator < Procedure > iter_procs(store);
            bool inserted = false;

            QString value = arg.right(arg.length() - arg.indexOf("=") - 1);

            arg = arg.right(arg.length() - 2);
            arg = arg.left(arg.indexOf("="));

            while(iter_procs.hasNext()) {

                Procedure *proc = &iter_procs.next();

                if(proc->full_name == arg) {

                    insert(*proc, value);
                    inserted = true;
                    break;
                }
            }

            if(!inserted)
                throw InputException("You have enter an incorrect argument: " + arg);

        } else {

            QMutableListIterator < Procedure > iter_procs(store);
            bool inserted = false;

            while(iter_procs.hasNext()) {

                Procedure *proc = &iter_procs.next();

                if(proc->type == Unbound) {

                    insert(*proc, arg);
                    inserted = true;
                    break;
                }
            }

            if(!inserted)
                throw InputException("You have enter an invalid syntax: " + arg);
        }
    }
}

// == == == == == == == == == == == == == == == == ==

bool CliApplication::notify(QObject *receiver, QEvent *event) {

    try {

        return QCoreApplication::notify(receiver, event);

    } catch(InputException &e) {

        qCritical() << "Warning: " << e.message();

    } catch(Exception &e) {

        qCritical() << e.message();

        exit(FAILURE);
    }

    return false;
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::argVoid(const QString &short_name, const QString &full_name, void (*fn)(void)) {

    Procedure p;

    p.fn = (void*) fn;
    p.type = Void;
    p.accessed = false;
    p.property = (void*) new pStatus;

    ((pStatus*) p.property)->status = false;

    p.short_name = short_name;
    p.full_name = full_name;

    store.push_back(p);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::argBoolean(const QString &short_name, const QString &full_name, void (*fn)(bool), bool default_status) {

    Procedure p;

    p.fn = (void*) fn;
    p.type = Boolean;
    p.accessed = false;
    p.property = (void*) new pStatus;

    ((pStatus*) p.property)->status = default_status;

    p.short_name = short_name;
    p.full_name = full_name;

    store.push_back(p);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::argString(const QString &short_name, const QString &full_name, void (*fn)(const QString &args)) {

    Procedure p;

    p.fn = (void*) fn;
    p.type = String;
    p.accessed = false;
    p.property = (void*) new pString;

    p.short_name = short_name;
    p.full_name = full_name;

    store.push_back(p);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::argList(const QString &short_name, const QString &full_name, void (*fn)(const QStringList &args)) {

    Procedure p;

    p.fn = (void*) fn;
    p.type = List;
    p.accessed = false;
    p.property = (void*) new pList;

    p.short_name = short_name;
    p.full_name = full_name;

    store.push_back(p);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::argUnbound(void (*fn)(const QStringList &args)) {

    Procedure p;

    p.fn = (void*) fn;
    p.type = Unbound;
    p.accessed = false;
    p.property = (void*) new pList;

    store.push_back(p);
}

// == == == == == == == == == == == == == == == == ==

void CliApplication::cli() {

    QListIterator < Procedure > iter(store);

    parse();

    while(iter.hasNext())
        call(iter.next());
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
