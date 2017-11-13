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

public:

    /* insert anything else you might need */
    Entry** returnhashtable(){ //get the hastable off an openaddmap object
        return hashtable;
    }
    // defines which probing scheme are available
    enum class ProbingScheme : uint8_t //havn't done anything with this yet
    {
        LINEAR,
        QUADRATIC,
        DOUBLE
    };

    // Constructor, takes a ProbingScheme as argument
    OpenAddMap(ProbingScheme const){
        initialize(tablesize);//initializes table with tablesize
    }

    void initialize(unsigned int tablesize){ //every entry equals -2u in the beginning
        hashtable = new Entry* [tablesize];
        for (unsigned int i = 0; i < tablesize; i++)
        {
              hashtable[i]->changekey(-2u);

        }
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
        int counter=0;
        for(unsigned int i=0;i<tablesize;i++){ //counts all elements not equal to -2u or -1u
            if(i!=-2u || i!=-1u){
                counter++;
            }
        }
        return counter;
    };

    //resize table if load factor too large or too small
    void resize(int decision){
        unsigned int newsize = tablesize;
        unsigned int originaltablesize = tablesize;
        if(decision==1){//too many elements
            newsize *= 2;
        }
        else if(decision==2){//too few elements
            newsize /= 2;
        }

        OpenAddMap newtable(OpenAddMap::ProbingScheme::LINEAR);
        newtable.initialize(newsize);//create new table with doubled/halved size
        for(unsigned int i=0; i< originaltablesize;i++){//insert old entries into new table
            if(hashtable[i]->getkey()!=-2u || hashtable[i]->getkey()!=-1u){
                newtable.insert(hashtable[i]->getkey());
            }
            else if(hashtable[i]->getkey()==-2u){
                hashtable=newtable.returnhashtable();
                break; //break if all entries !=-2u have been looked at
            }
        }
    }

    // insert a new element into the table. On success return true.
    // If element was already in the table return false.
    bool insert(unsigned const int key){
        int hashvalue = hashfunc(key);
        while (hashtable[hashvalue]->getkey()!=-2u || hashtable[hashvalue]->getkey()!= -1u){
            if(hashtable[hashvalue]->getkey()==key){//if element is already in the hashmap
                return false;
            }
            hashvalue=(hashvalue+1)%tablesize;
        }
        if (hashtable[hashvalue]->getkey()==-2u || hashtable[hashvalue]->getkey()== -1u){
            hashtable[hashvalue]=new Entry(key); //insert new element
            if((numElem()/tablesize>=0.75)){
                resize(1);
            }
            return true;
        }
        printtable(); //debugging
        return false;
    };

    //search for entry in the table. If found return true otherwise false.
    bool find(unsigned const int key){
        int hashvalue = hashfunc(key);
        while (hashtable[hashvalue]->getkey()!=-2u){ //same principle as for inserting an item
            if(hashtable[hashvalue]->getkey()==key){
                return true;
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
    bool remove(unsigned const int key){
        int hashvalue = hashfunc(key);
        if(find(key)){
            hashtable[hashvalue]->changekey(-2u);
            if((numElem()/tablesize<=0.25)){ //resizing if the table got too small
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
