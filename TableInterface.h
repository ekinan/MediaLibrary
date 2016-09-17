/*@file TableInterface.h*/
#ifndef _TABLE_INTERFACE_H
#define _TABLE_INTERFACE_H

template <class ItemType>
class TableInterface
{
public:
	/*
	Checks to see if the table is empty
	@return True if the table is empty, false otherwise
	*/
	virtual bool isEmpty() const = 0;

	/*
	Returns the number of items in the table
	@return The number of items in the table
	*/
	virtual int getNumberOfItems() const = 0;

	/*
	Adds a new entry into the table
	@post If successful, newItem is stored in the table
	@param newItem The data to be added as the new entry
	@return True if addition was successful, false if not
	*/
	virtual bool add (const ItemType& newItem) = 0;

	/*
	Removes an entry from the tree
	@post If successful, data is removed from the tree. If not, nothing happens.
	@param entry The item to be removed.
	@return True if the removal was successful, false if not.
	*/
	virtual bool remove(const ItemType& entry) = 0;

	/*
	Returns the table size
	@return Return the table size
	*/
	virtual int getTableSize() const = 0;

	/*
	Erases the contents of the table
	@post Table contains no items
	*/
	virtual void clear() = 0;

	/*
	Retrieves an entry if it exists
	@param entry The item to be retrieved
	@return The entry if it's in the table, otherwise an exception is thrown.
	*/
	virtual ItemType getEntry(const ItemType& entry) const = 0;

	/*
	Checks to see if an item is in the tree
	@param entry The item to be checked
	@return True if the item is in the tree, false if not.
	*/
	virtual bool contains(const ItemType& entry) const = 0;

	/*
	Traverses the table
	@post Visit is executed for every entry in the table
	@param visit(ItemType&) A client defined function that performs an action
	on an item of the tree.
	*/
	virtual void traverse(void visit(ItemType&)) const = 0;
};

#endif
