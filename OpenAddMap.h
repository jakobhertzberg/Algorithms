/* insert any headers you might need */

class OpenAddMap
{
private:
    /* insert anything else you might need */

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
    OpenAddMap(ProbingScheme const);

    // returns the size (reserved slots) of the table.
    size_t size() const;
    // returns the number of elements currently in the table (not including NIL markers)
    size_t numElem() const;

    // insert a new element into the table. On success return true.
    // If element was already in the table return false.
    bool insert(unsigned const int);

    //search for entry in the table. If found return true otherwise false.
    bool find(unsigned const int);

    // delete entry from table. If deleted return true.
    // If element was not in the table return false.
    bool remove(unsigned const int);
};
