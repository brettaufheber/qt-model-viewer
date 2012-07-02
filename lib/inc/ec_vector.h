#ifndef __EC_VECTOR_H__
#define __EC_VECTOR_H__

#include <ec_core.h>


namespace ecCore {

namespace Vector {

// == == == == == == == == == == == == == == == == ==
// Prototypes

template < typename type >
class Iterator;

template < typename type >
class Container;

// == == == == == == == == == == == == == == == == ==

typedef struct node *ref;
typedef struct node {

    void *property;
    ref prev, next;

} Node;

// == == == == == == == == == == == == == == == == ==

template < typename type >
class Iterator {

private:

//////////////////////////////////
// Data

    template < typename type_of_friend >
    friend class Container;

    ref *first;
    ref *last;
    ref current_node;
    uint index;
    uint *size;

//////////////////////////////////
// Procedures

    void getNode(uint i);

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    Iterator(Container < type > &vec);
    ~Iterator();

//////////////////////////////////
// Procedures

    void begin();
    void end();
    uint getIndex();
    bool established();
    type &next();
    type &prev();

    type &operator[](uint i);

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

template < typename type >
class Container {

protected:

//////////////////////////////////
// Data

    template < typename type_of_friend >
    friend class Iterator;

    ref first;
    ref last;
    uint size;

    void create(ref p, ref n, uint i, type *value);
    void remove(ref t, uint i);

    Iterator < type > *basic_iter; // iterator for inserting und erasing
    Iterator < type > *ra_iter; // iterator for random access

//////////////////////////////////

public:

//////////////////////////////////
// Constructors and Destructors

    Container();
    ~Container();

//////////////////////////////////
// Procedures

    void begin();
    void end();
    uint getIndex();

    void pushBack(type *value = NULL);
    void pushFront(type *value = NULL);
    void popBack(type *value = NULL);
    void popFront(type *value = NULL);
    void insert(uint i, type *value = NULL);
    type &front();
    type &back();
    bool empty();
    uint length();
    void length(uint i);
    void erase();
    void erase(uint i);

    type &operator[](uint i);

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Iterator < type > ::getNode(uint i) {

    if(!current_node)
        throw Exception("Iterator::getNode; invalid domain");

    if(i >= *size)
        throw Exception("Iterator::getNode; index out of range");

    if(i <= index) {

        if(i < index - i) {

            current_node = *first;
            index = 0;

            while(index < i) {

                index++;
                current_node = current_node->next;
            }

        } else {

            while(index > i) {

                index--;
                current_node = current_node->prev;
            }
        }

    } else {

        if(i - index < *size - i - 1) {

            while(index < i) {

                index++;
                current_node = current_node->next;
            }

        } else {

            current_node = *last;
            index = *size - 1;

            while(index > i) {

                index--;
                current_node = current_node->prev;
            }
        }
    }
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
Iterator < type > ::Iterator(Container < type > &vec) {

    first = &vec.first;
    last = &vec.last;
    size = &vec.size;
    current_node = NULL;
    index = 0;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
Iterator < type > ::~Iterator() {}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Iterator < type > ::begin() {

    current_node = *first;
    index = 0;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Iterator < type > ::end() {

    current_node = *last;
    index = *size ? *size - 1 : 0;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
uint Iterator < type > ::getIndex() {

    return index;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
bool Iterator < type > ::established() {

    return current_node != NULL;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Iterator < type > ::next() {

    if(!current_node)
        throw Exception("Iterator::next; invalid domain");

    type *property = (type*)current_node->property;
    current_node = current_node->next;
    index = current_node ? index + 1 : 0;

    return *property;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Iterator < type > ::prev() {

    if(!current_node)
        throw Exception("Iterator::prev; invalid domain");

    type *property = (type*)current_node->property;
    current_node = current_node->prev;
    index = current_node ? index - 1 : 0;

    return *property;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Iterator < type > ::operator[](uint i) {

    getNode(i);

    return *(type*)current_node->property;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::create(ref p, ref n, uint i, type *value) {

    size++;

    if(ra_iter->index > i)
        ra_iter->index++;

    ref t = (ref)new node;
    t->property = new type;

    if(value)
        *(type*)t->property = *value;

    t->prev = p;
    t->next = n;

    if(p)
        p->next = t;
    else
        first = t;

    if(n) {

        if(ra_iter->current_node == n)
            ra_iter->index++;

        n->prev = t;

    } else
        last = t;

    if(size == 1)
        begin();
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::remove(ref t, uint i) {

    if(!size)
        throw Exception("Container::remove; invalid domain");

    size--;

    if(ra_iter->index && ra_iter->index > i)
        ra_iter->index--;

    if(t->prev)
        t->prev->next = t->next;
    else
        if(t->next)
            first = t->next;

    if(t->next) {

        if(ra_iter->current_node == t)
            ra_iter->current_node = t->next;

        t->next->prev = t->prev;

    } else {

        if(t->prev) {

            if(ra_iter->current_node == t) {

                ra_iter->current_node = t->prev;

                if(ra_iter->index)
                    ra_iter->index--;
            }

            last = t->prev;
        }
    }

    if(!t->prev && !t->next) {

        first = NULL;
        last = NULL;
        ra_iter->current_node = NULL;
    }

    release < type* > (t->property);
    release(t);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
Container < type > ::Container() {

    first = NULL;
    last = NULL;
    size = 0;

    basic_iter = new Iterator < type > (*this);
    ra_iter = new Iterator < type > (*this);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
Container < type > ::~Container() {

    erase();
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::begin() {

    ra_iter->begin();
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::end() {

    ra_iter->end();
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
uint Container < type > ::getIndex() {

    return ra_iter->getIndex();
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::pushBack(type *value) {

    create(last, NULL, size - 1, value);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::pushFront(type *value) {

    create(NULL, first, 0, value);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::popBack(type *value) {

    if(value && size)
        *value = *(type*)last->property;

    remove(last, size - 1);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::popFront(type *value) {

    if(value && size)
        *value = *(type*)first->property;

    remove(first, 0);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::insert(uint i, type *value) {

    basic_iter->begin();
    basic_iter->getNode(i);
    create(basic_iter->current_node->prev, basic_iter->current_node, i, value);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Container < type > ::front() {

    if(!size)
        throw Exception("Container::front; invalid domain");

    return *(type*)first->property;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Container < type > ::back() {

    if(!size)
        throw Exception("Container::back; invalid domain");

    return *(type*)last->property;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
bool Container < type > ::empty() {

    return size == 0;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
uint Container < type > ::length() {

    return size;
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::length(uint len) {

    while(size < len)
        create(last, NULL, size - 1, NULL);

    while(size > len)
        remove(last, size - 1);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::erase() {

    while(size)
        remove(first, 0);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
void Container < type > ::erase(uint i) {

    basic_iter->begin();
    basic_iter->getNode(i);
    remove(basic_iter->current_node, i);
}

// == == == == == == == == == == == == == == == == ==

template < typename type >
type &Container < type > ::operator[](uint i) {

    return (*ra_iter)[i];
}

// == == == == == == == == == == == == == == == == ==

} // Vector

} // ecCore

#endif
