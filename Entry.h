#include <iostream>

using namespace std;

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
