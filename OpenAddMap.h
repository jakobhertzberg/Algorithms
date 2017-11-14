#include <cstddef>
#include <iostream>
using namespace std;

//class of the Hashentries
class Entry{
private:
    unsigned int key;
public:
    Entry(unsigned const int key){
        this->key=key;
    }
    unsigned int getkey(){
        return key;
    }
    void changekey(int newkey){
        this->key=newkey;
    }
    void printentry(){
        cout << to_string(key) << endl;
    }
};


class OpenAddMap
{
private:
    /* insert anything else you might need */
    Entry **hashtable; //hashtable with pointers to entrypointer
    float maxthreshold=0.75; //thresholds for the load factor
    float minthreshold=0.25;
    unsigned int tablesize=4; //default of 4
    uint8_t probing

public:

    /* insert anything else you might need */
    // defines which probing scheme are available
    enum class ProbingScheme : uint8_t
    {
        LINEAR,
        QUADRATIC,
        DOUBLE
    };
    // Constructor, takes a ProbingScheme as argument
    OpenAddMap(ProbingScheme const){
        hashtable = new Entry* [tablesize];
        for (unsigned int i = 0; i < tablesize; i++)
        {
              hashtable[i]=new Entry(-2u);
        }
        this->probing=ProbingScheme;
    }

    //simple hashfunction
    int hashfunc(int key){
        return key % tablesize;
    }

    // returns the size (reserved slots) of the table.
    size_t size() const{
        return tablesize;
    }
    // returns the number of elements currently in the table (not including NIL markers)
    size_t numElem() const{
        int counter = 0;
        for (unsigned int i=0; i<tablesize; i++){
            if(hashtable[i]->getkey()!=-2u && hashtable[i]->getkey()!=-1u){
                counter++;
            }
        }
        return counter;
    };

    float getloadfactor() {
        float loadfactor = (float)numElem()/(float)tablesize;
        return loadfactor;
    }

    //resize table if load factor too large or too small
    void resize(int decision){
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
    bool insert(unsigned const int key){
        int hashvalue = hashfunc(key);
        while (hashtable[hashvalue]->getkey()!=-2u && hashtable[hashvalue]->getkey()!= -1u){
            hashtable[hashvalue]->printentry();
            if(hashtable[hashvalue]->getkey()==key){//if element is already in the hashmap
                return false;
            }
            //probing schemes
            if(probing=LINEAR){
                hashvalue=(hashvalue+1)%tablesize;
            }
            else if {probing=QUADRATIC}{
                hashvalue=(hashvalue+[i]*[i])%tablesize;
            }
            else if {probing=QUADRATIC}{//TODO implement universal hashing for this
                hashvalue=(hashvalue+[i]*[i])%tablesize;
            }
        }
        hashtable[hashvalue]->changekey(key); //insert new element
        if(getloadfactor()>=maxthreshold){
            resize(1);
        }
        return true;
    };

    //search for entry in the table. If found return true otherwise false.
    bool find(unsigned const int key){
        int hashvalue = hashfunc(key);
        while (hashtable[hashvalue]->getkey()!=-2u){ //same principle as for inserting an item
            if(hashtable[hashvalue]->getkey()==key){
                return true;
            }
            hashvalue=(hashvalue+1)%tablesize;
        }
        if(hashtable[hashvalue]->getkey()==-2u){ //can quit when -2u is reached since there can't be an element after that
            return false;
        }
    printtable(); //also debugging
    return false;
}

    // delete entry from table. If deleted return true.
    // If element was not in the table return false.
    bool remove(unsigned const int key){
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

    int debug(){ //the test file calls a debug function. Don't know what to do with this.
        return 1;
    }

    void printtable(){ // should print the entire hashtable
        for(unsigned int i=0; i < tablesize; i++){
            hashtable[i]->printentry();
        }
    }
};
