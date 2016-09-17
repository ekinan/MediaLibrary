#ifndef _HASH_TABLE_CPP
#define _HASH_TABLE_CPP

#include <cctype>
#include <cmath>
#include <cstdlib>
#include "HashTable.h"
#include "NotFoundException.h"

template <class ItemType>
HashTable<ItemType>::HashTable(int initialSize)
{
	tableSize = initialSize;
	table = new Node<ItemType>*[tableSize];

	numCollisions = 0;
	numEntries = 0;

	for (int i = 0; i < tableSize; i++) //Initialize the table to NULL state
		table[i] = NULL;
}

template <class ItemType>
HashTable<ItemType>::HashTable(const HashTable<ItemType>& otherTable) //Copy contents of the other table
{
	tableSize = otherTable.tableSize;
	numCollisions = otherTable.numCollisions;
	numEntries = otherTable.numEntries;

	table = new Node<ItemType>*[tableSize];
	for (int i = 0; i < tableSize; i++) //Copy the contents in otherTable
	{
		if (otherTable.table[i] != NULL) //There's an entry at this address, so we copy the linked chain
		{
			Node<ItemType>* otherCurrent = otherTable.table[i];
			Node<ItemType>* thisCurrent = NULL;
			Node<ItemType>* trailCurrent = NULL;

			table[i] = new Node<ItemType>; //Create the first node
			table[i]->item = otherCurrent->item;
			table[i]->next = NULL;

			trailCurrent = table[i];
			otherCurrent = otherCurrent->next;

			while (otherCurrent != NULL) //Traverse the linked chain and create copies
			{			     //of the nodes, being sure to connect them with the
				thisCurrent = new Node<ItemType>; //previous one
				thisCurrent->item = otherCurrent->item;
				trailCurrent->next = thisCurrent;

				trailCurrent = thisCurrent;
				otherCurrent = otherCurrent->next;
			}

			trailCurrent->next = NULL; //Connet the NULL pointer of the chain
		}
		else //No entry exists, so we set the address to NULL
			table[i] = NULL;

	}
}

template <class ItemType>
HashTable<ItemType>::~HashTable()
{
	eraseTable(); //Erase all of the chains in the table

	delete [] table; //Deallocate the dynamic memory
}

template <class ItemType>
int HashTable<ItemType>::getCharCode(char c) const
{
	if (c != ' ')
		return (toupper(c) - 39); //Covert the character to uppercase and return its code
	else //Blank character ASCII is 155, so it needs its own distinct code
		return 52;
}

template <class ItemType>
int HashTable<ItemType>::h(const ItemType& item, int index) const
{
	if (index == 0) //Base case, see design write-up for more details
		return ((getCharCode(item[0])) % tableSize);
	else //Recursive definition utilizing Horner's rule and modulus properties
		return (((h(item, index-1)*(64 % tableSize)) % tableSize
				 + (getCharCode(item[index])) % tableSize) % tableSize);
}


template <class ItemType>
int HashTable<ItemType>::getNextPrime(int num)
{
	int newPrime;

	if (num % 2 == 0) //Even number, we add one to get an odd number.
		newPrime = num + 1;
	else	//Odd number, add 2 to get the next odd.
		newPrime = num + 2;

	while (!isPrime(newPrime)) //While the newPrime isn't a prime number
		newPrime += 2; //Check the next prime number

	return newPrime;
}


template <class ItemType>
bool HashTable<ItemType>::isPrime(int num) //A number n is prime if no numbers between 3
{					//and sqrt(n) divide n
	bool isPrime = true;
	int sqrtNum = ceil(sqrt(num)); //Compute the square root

	bool canDivide = false;
	int nextNum = 3;

	while (!canDivide && nextNum <= sqrtNum)
	{
		canDivide = ((num % nextNum) == 0);
		nextNum += 2; //Go to the next odd divisor
	}

	if (canDivide) //There is a number between 3 and sqrt(n) that divides n
		isPrime = false; //so it is not prime

	return isPrime;
}

template <class ItemType>
int HashTable<ItemType>::getNumCollisions() const
{
	return numCollisions;
}

template <class ItemType>
int HashTable<ItemType>::getNumberOfItems() const
{
	return numEntries;
}

template <class ItemType>
int HashTable<ItemType>::getTableSize() const
{
	return tableSize;
}

template <class ItemType>
bool HashTable<ItemType>::isEmpty() const
{
	return (numEntries == 0);
}



template <class ItemType>
void HashTable<ItemType>::insertIntoChain(int tableIndex, const ItemType& item)
{
	Node<ItemType>* newNode = new Node<ItemType>; //Insert item into a new node
	newNode->item = item;

	newNode->next = table[tableIndex]; //Connect the node with the headPtr of the table
	table[tableIndex] = newNode; //Set the headPtr to the newNode
}

template <class ItemType>
void HashTable<ItemType>::expandTable()
{
	int oldTableSize = tableSize;
	Node<ItemType>** oldTable = table; //Store the old table

	//Get the next prime number for the new table size
	tableSize = getNextPrime(2*tableSize);

	table = new Node<ItemType>*[tableSize]; //Create the new table

	numCollisions = 0; //Reset the number of collisions and entries
	numEntries = 0;

	for (int i = 0; i < tableSize; i++) //Reallocate the new values
		table[i] = NULL;

	for (int i = 0; i < oldTableSize; i++) //Rehash the entries in the old table to the new table
	{
		if (oldTable[i] != NULL) //Entry exists
		{
			Node<ItemType>* current = oldTable[i];
			Node<ItemType>* trailCurrent = NULL;
			do //Rehash entries in old table while simultaneously deleting the chain
			{
				add(current->item); //Call the function add
				trailCurrent = current;
				current = current->next;

				delete trailCurrent; //Delete the nodes along the way
			} while (current != NULL);
		}
	}

	delete [] oldTable; //Deallocate the old table's dynamic memory

	numEntries--; //Since it will pop back to the original add function, one item will be counted twice.
}

template <class ItemType>
bool HashTable<ItemType>::add(const ItemType& newItem)
{
	int tableIndex = h(newItem, newItem.length()-1);
	if (table[tableIndex] == NULL) //No entry exists, so create a new head node
	{
		table[tableIndex] = new Node<ItemType>;
		table[tableIndex]->item = newItem;
		table[tableIndex]->next = NULL;
	}
	else //Collision exists
	{
		numCollisions++;
		insertIntoChain(tableIndex, newItem); //Put item in the first slot of the linked chain

		int listLength = listSize(tableIndex);
		if ((listLength-1) == MAX_COL_SIZE) //Check if the new entry has exceeded the max allowable
		{				   //number of collisions. Resize the table if so
			expandTable();
		}
	}

	numEntries++;

	return true;
}

template <class ItemType>
int HashTable<ItemType>::listSize(int index) const
{
	int size = 0;
	Node<ItemType>* current = table[index];
	while (current != NULL) //Count the number of nodes encountered in the table
	{
		size++;
		current = current->next;
	}

	return size;
}


template <class ItemType>
bool HashTable<ItemType>::removeFromChain(int tableIndex, const ItemType& item)
{
	bool removed = false;

	Node<ItemType>* current = table[tableIndex];
	Node<ItemType>* trailCurrent = NULL;

	while (!removed && current != NULL) //Look for the node until it is found, or the end of the chain
	{				    //is reached.
		if (current->item == item) //Found the node
		{
			if (listSize(tableIndex) > 1) //There's a collision at this index, so decrement
				numCollisions--; //numCollisions

			if (current == table[tableIndex]) //If item is at the head of the chain
				table[tableIndex] = table[tableIndex]->next; //Move the head node
			else //Connect previous item to this item's next node
				trailCurrent->next = current->next;



			delete current;	//Remove the node

			removed = true;
			numEntries--;
		}
		else //Item is not found, so move to the next node
		{
			trailCurrent = current;
			current = current->next;
		}
	}

	return removed;

}

template <class ItemType>
bool HashTable<ItemType>::remove(const ItemType& entry)
{
	int tableIndex = h(entry, entry.length()-1); //Get table address
	bool hasEntry = (table[tableIndex] != NULL); //Check if entry exists
	if (hasEntry) //There is an entry, so we search the corresponding chain to see if it's in there.
		return removeFromChain(tableIndex, entry);
	else //Entry does not exist
		return false;
}


template <class ItemType>
void HashTable<ItemType>::eraseTable()
{
	for (int i = 0; i < tableSize; i++)
	{
		if (table[i] != NULL) //Delete the entire linked list in the table entry if it exists
		{
			Node<ItemType>* currentPtr = table[i];
			Node<ItemType>* nextPtr = NULL;
			do
			{
				nextPtr = currentPtr->next;
				delete currentPtr;
				currentPtr = nextPtr;
			} while (currentPtr != NULL);
			table[i] = NULL; //Reset the address to NULL
		}

	}
}

template <class ItemType>
void HashTable<ItemType>::clear()
{
	eraseTable();

	numCollisions = 0;
	numEntries = 0;
}

template <class ItemType>
ItemType HashTable<ItemType>::getEntry(const ItemType& entry) const
{
	int tableIndex = h(entry, entry.length()-1); //Get table address
	Node<ItemType>* current = table[tableIndex];

	while (current != NULL && (current->item != entry)) //Search the chain for the entry if there is a chain
		current = current->next;

	if (current != NULL) //If the item is in the table or the linked chain,
		return current->item;
	else //Otherwise, throw an exception
		throw(NotFoundException("getEntry() called with a nonexistant entry"));
}

template <class ItemType>
bool HashTable<ItemType>::contains(const ItemType& entry) const //Same as getEntry
{
	int tableIndex = h(entry, entry.length()-1);
	Node<ItemType>* current = table[tableIndex];

	while (current != NULL && (current->item != entry))
		current = current->next;

	return (current != NULL);
}


template <class ItemType>
void HashTable<ItemType>::traverse(void visit(ItemType&)) const
{
	for (int i = 0; i < tableSize; i++)
	{
		Node<ItemType>* current = table[i];
		if (current != NULL) //If chain exists, traverse the chain and execute visit on all the nodes
		{		     //in it.
			do
			{
				visit(current->item);
				current = current->next;
			} while (current != NULL);
		}
	}
}

/* IGNORE
template <class ItemType>
void HashTable<ItemType>::displayTable() const
{
	for (int i = 0; i < tableSize; i++)
	{
		Node<ItemType>* current = table[i];
		if (current != NULL)
		{
			do
			{
				std::cout << current->item << "-->";
				current = current->next;
			} while (current != NULL);
		}
		std::cout << "[]" << std::endl;
	}
}
*/

template <class ItemType>
void HashTable<ItemType>::writeToFile(std::ostream& outFile) const
{
	for (int i = 0; i < tableSize; i++)
	{
		Node<ItemType>* current = table[i];
		if (current != NULL)
		{
			do //Write the contents of the item to the file outFile
			{ //by calling the function write file for each item in the chain
				(current->item).writeToFile(outFile);
				current = current->next;
			} while (current != NULL);
		}
	}
}

template <class ItemType>
int HashTable<ItemType>::getOtherStats(int& numOccupied) const
{
	int max = 0; //Assume that the table is empty initially
	for (int i = 0; i < tableSize; i++)
	{
		if (table[i] != NULL) //Get the maximum collision size while simultaneously
		{                     //getting the number of occupied entries
			numOccupied++;
			int listLength = listSize(i);
			if ((--listLength) > max)
				max = listLength;
		}
	}

	return max;
}

template <class ItemType>
void HashTable<ItemType>::displayStatistics(std::ostream& os) const
{
	int maxColSize = 0;
	int numOccupiedEntries = 0;

	maxColSize = getOtherStats(numOccupiedEntries);

	os << "Table size: " << tableSize << std::endl;
	os << "Number of collisions: " << numCollisions << std::endl;
	os << "Maximum collision size: " << maxColSize << std::endl;
	os << "Number of occupied entries: " << numOccupiedEntries << std::endl;
	os << "Number of items: " << numEntries << std::endl;
	os << std::endl << std::endl;
}


#endif
