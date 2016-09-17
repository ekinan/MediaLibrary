/*@file BalancedSearchTreeInterface.h*/
#ifndef _BALANCED_SEARCH_TREE_INTERFACE_H
#define _BALANCED_SEARCH_TREE_INTERFACE_H


template <class ItemType>
class BalancedSearchTreeInterface
{
public:
	/* Checks to see if the tree is empty
	@return True if the tree is empty, false otherwise.
	*/
	virtual bool isEmpty() const = 0;

	/* Calculates the height of the tree, where an empty tree has a depth of 0
	@return The height of the tree
	*/
	virtual int getHeight() const = 0;

	/* Returns the number of items in the tree
	@return The number of items in the tree
	*/
	virtual int getNumberOfItems() const = 0;

	/* Adds a new entry into the tree
	@post If successful, newData is stored in the tree
	@param newData The data to be added as the new entry
	@return True if addition was successful, false if not
	*/
	virtual bool add(const ItemType& newData) = 0;

	/* Removes an entry from the tree
	@post If successful, data is removed from the tree. If not, nothing happens.
	@param data The item to be removed
	@return True if the removal was successful, false if not
	*/
	virtual bool remove(const ItemType& data) = 0;

	/* Erases the contents of the tree
	@post Tree contains no items
	*/
	virtual void clear() = 0;

	/* Retrieves an entry if it exists
	@param anEntry The item to be retrieved
	@return The entry if it's in the tree, otherwise an exception is thrown
	*/
	virtual ItemType getEntry(const ItemType& anEntry) const = 0;

	/* Checks to see if an item is in the tree
	@param anEntry The item to be checked
	@return True if the item is in the tree, false if not
	*/
	virtual bool contains(const ItemType& anEntry) const = 0;


	/* Conducts an inorder traversal of the tree
	@post Visit is executed for every node in the tree
	@param visit(ItemType&) A client defined function that performs an action on
	an item of the tree
	*/
	virtual void traverse(void visit(ItemType&)) const = 0;
};

#endif
