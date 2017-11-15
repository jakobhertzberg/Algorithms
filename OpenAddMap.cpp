#include "OpenAddMap.h"
#include <iostream>

using namespace std;

OpenAddMap::OpenAddMap(ProbingScheme const scheme){
    hashtable = new Entry* [tablesize];
    for (unsigned int i = 0; i < tablesize; i++)
    {
          hashtable[i]=new Entry(-2u);
    }
    this->probing=scheme;
}
//simple hashfunction
int OpenAddMap::hashfunc(int key){
    return key % tablesize;
}
//second hashfunction
int OpenAddMap::hashfunc2(int key){
    return (key*tablesize-1)%tablesize;
}
// returns the size (reserved slots) of the table.
size_t OpenAddMap::size() const{
    return tablesize;
}
// returns the number of elements currently in the table (not including NIL markers)
size_t OpenAddMap::numElem() const{
    int counter = 0;
    for (unsigned int i=0; i<tablesize; i++){
        if(hashtable[i]->getkey()!=-2u && hashtable[i]->getkey()!=-1u){
            counter++;
        }
    }
    return counter;
}
float OpenAddMap::getloadfactor() {
    float loadfactor = (float)numElem()/(float)tablesize;
    return loadfactor;
}
//resize table if load factor too large or too small
void OpenAddMap::resize(int decision){
    unsigned int newsize = tablesize;
    if(decision==1){//too many elements
        newsize *= 2;
    }
    else if(decision==2){//too few elements
        newsize /= 2;
    }
    Entry ** oldhashtable = hashtable; //copy old table
    unsigned int originaltablesize = tablesize;
    this->tablesize=newsize; //redefine size
    hashtable = new Entry * [tablesize]; //initialize new table with new size
    for (unsigned int i = 0; i < tablesize; i++)
    {
          hashtable[i]=new Entry(-2u);
    }

    for(unsigned int i=0; i< originaltablesize;i++){//insert old entries into new table
        if(oldhashtable[i]->getkey()!=-2u && oldhashtable[i]->getkey()!=-1u){
            insert(oldhashtable[i]->getkey());
        }
    }
}

// insert a new element into the table. On success return true.
// If element was already in the table return false.
bool OpenAddMap::insert(unsigned const int key){
    int hashvalue = hashfunc(key);
    while (hashtable[hashvalue]->getkey()!=-2u && hashtable[hashvalue]->getkey()!= -1u){
        if(hashtable[hashvalue]->getkey()==key){//if element is already in the hashmap
            return false;
        }
        //probing schemes
        if(probing==ProbingScheme::LINEAR){
            hashvalue=(hashvalue+1)%tablesize;
        }
        else if (probing==ProbingScheme::QUADRATIC){
            int i= tablesize-1;
            hashvalue=(hashvalue+13*(i)+24*(i*i))%tablesize;
        }
        else if (probing==ProbingScheme::DOUBLE){//TODO implement universal hashing for this
            hashvalue=(hashvalue+hashfunc2(hashvalue))%tablesize;
        }
    }
    hashtable[hashvalue]->changekey(key); //insert new element
    if(getloadfactor()>=maxthreshold){
        resize(1);
    }
    return true;
}

//search for entry in the table. If found return true otherwise false.
bool OpenAddMap::find(unsigned const int key){
    int hashvalue = hashfunc(key);
    while (hashtable[hashvalue]->getkey()!=-2u){ //same principle as for inserting an item
        if(hashtable[hashvalue]->getkey()==key){
            return true;
        }
        //probing schemes
        if(probing==ProbingScheme::LINEAR){
            hashvalue=(hashvalue+1)%tablesize;
        }
        else if (probing==ProbingScheme::QUADRATIC){
            int i = tablesize-1;
            hashvalue=(hashvalue+13*(i)+24*(i*i))%tablesize;
        }
        else if (probing==ProbingScheme::DOUBLE){//TODO implement universal hashing for this
            hashvalue=(hashvalue+hashfunc2(hashvalue))%tablesize;
        }
    }
    if(hashtable[hashvalue]->getkey()==-2u){ //can quit when -2u is reached since there can't be an element after that
        return false;
    }
printtable(); //also debugging
return false;
}

// delete entry from table. If deleted return true.
// If element was not in the table return false.
bool OpenAddMap::remove(unsigned const int key){
    int hashvalue = hashfunc(key);
    if(find(key)){
        hashtable[hashvalue]->changekey(-2u);
        if(getloadfactor()<=minthreshold){ //resizing if the table got too small
            resize(2);
        }
        return true;
    }
    else{
        return false;
    }
}

int OpenAddMap::debug(){ //the test file calls a debug function. Don't know what to do with this.
    return 1;
}

void OpenAddMap::printtable(){ // should print the entire hashtable
    for(unsigned int i=0; i < tablesize; i++){
        hashtable[i]->printentry();
    }
}
