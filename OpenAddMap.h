#ifndef _OPENADDMAP_H_
#define _OPENADDMAP_H_

#include <cstddef>
#include "Entry.h"
using namespace std;

class OpenAddMap
{
private:
    /* insert anything else you might need */
    Entry **hashtable; //hashtable with pointers to entrypointer
    float maxthreshold=0.75; //thresholds for the load factor
    float minthreshold=0.25;
    unsigned int tablesize=4; //default of 4
    unsigned int numelem=0;

public:

    /* insert anything else you might need */

    enum class ProbingScheme : uint8_t
    {
        LINEAR,
        QUADRATIC,
        DOUBLE
    };
    ProbingScheme probing;
    // Constructor, takes a ProbingScheme as argument
    OpenAddMap(ProbingScheme const scheme);
    //functions
    int hashfunc(int key);
    int hashfunc2(int key);
    size_t size() const;
    size_t numElem() const;
    float getloadfactor();
    void resize(int decision);
    int get_hashvalue(unsigned const int key);
    bool insert(unsigned const int key, bool new_insert = true);
    bool find(unsigned const int key);
    bool remove(unsigned const int key);
    int debug();
    void printtable();
};

#endif
