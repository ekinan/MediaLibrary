/*@file HashTable.h*/
#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include "TableInterface.h"
#include <iostream>

const int MAX_COL_SIZE = 10; //Maximum allowable collision size for any entry in the table
const int DEFAULT_SIZE = 31; //Default table size, make sure it is a prime number

template <class ItemType> //Struct for the node used in separate chaining
struct Node
{
	ItemType item;
	Node<ItemType>* next;
};

template <class ItemType>
class HashTable : public TableInterface<ItemType>
{
private:
	Node<ItemType>** table; //The table itself
	int tableSize; //Size of the table
	int numCollisions; //Total number of collisions in the table
	int numEntries; //Total number of entries

	/*
	Returns a code ranging from 0 - 50 for a character, c to use in the hash function
	@param c The character whose code is to be retrieved
	@return A value ranging from 0 - 50
	*/
	int getCharCode(char c) const;

	/*
	Hash function to compute the table address of an item containing a string phrase
	@param item The item whose address is to be computed
	index Used to facilitate the recursive definition of h. See the design write-up for more details
	@return Table address for item to be inserted
	*/
	int h(const ItemType& item, int index) const;

	/*
	Function to compute the next prime number after num for a num > 2.
	@param num The number whose next prime is to be calculated
	@return Returns the next prime number after num
	*/
	int getNextPrime(int num);

	/*
	Function that checks if num is prime or not.
	@param num The number checked
	@return True if num is prime, otherwise false
	*/
	bool isPrime(int num);

	/*
	Function inserts an item into the linked chain at address tableIndex
	@post item is inserted first into the linked chain at tableIndex
	@param tableIndex The address for insertion
	item The item to be inserted
	*/
	void insertIntoChain(int tableIndex, const ItemType& item);

	/*
	Function removes an item from the linked chain at address tableIndex
	@post item is removed from the chain if it exists, otherwise nothing happens
	@param tableIndex The address for removal
	item The item to be removed
	@return True if the removal was successful, false otherwise
	*/
	bool removeFromChain(int tableIndex, const ItemType& item);


	/*
	Function clears the table of all entries.
	@post Table is empty and all linked chains have been deleted
	*/
	void eraseTable();


	/*
	Function creates a new table 2x the size of the original.
	@post A new table 2x the size of the original is created, with all of the entries from the original
	rehashed into their new addresses. The old table is deleted.
	*/
	void expandTable();

	/*
	Returns the length of the chain at address index
	@param index The address of the table for which the length is to be calculated
	@return The length of the chain at table[index].
	*/
	int listSize(int index) const;


	/*
	Returns the maximum collision size of the table, and the number of occupied entries
	@param numOccupied The number of occupied entries
	@return The maximum collision size in the table
	*/
	int getOtherStats(int& numOccupied) const;

public:
	HashTable(int initialSize = DEFAULT_SIZE);
	HashTable(const HashTable<ItemType>& otherTable); //Copy constructor
	virtual ~HashTable(); //Destructor


	/*
	Function returns the number of collisions in the table
	@return The total number of collisions in the table
	*/
	int getNumCollisions() const;

	/*See Table Interface for these*/
	bool isEmpty() const;
	int getNumberOfItems() const;
	bool add(const ItemType& newItem);
	bool remove(const ItemType& entry);
	int getTableSize() const;
	void clear();
	ItemType getEntry(const ItemType& entry) const;
	bool contains(const ItemType& entry) const;
	void traverse(void visit(ItemType&)) const;


/*
IGNORE USED FOR DEBUGGING
	void displayTable() const;
*/

	/*
	Writes out the contents of the table to the file opened by outFile
	@post The contents of the table are written to outFile
	@param outFile Ostream variable storing the file
	*/
	void writeToFile(std::ostream& outFile) const;

	/*
	Writes out the relevant statistics of the table to the ostream variable os
	@post Outputs the table size, number of collisions, maximum collision size,
	number of occupied entries, and the number of entries in the table to the
	ostream variable os
	@param os Ostream variable for the output
	*/
	void displayStatistics(std::ostream& os) const;

};


#include "HashTable.cpp"

#endif
