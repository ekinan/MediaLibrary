/*@file TriNode.h*/
#ifndef _TRI_NODE_H
#define _TRI_NODE_H

template <class ItemType>
class TriNode
{
private:
	ItemType* smallItem; //Small item of the node
	ItemType* largeItem; //Large item, if it is a 3-Node
	TriNode<ItemType>* leftChildPtr; //Pointer to the node's left child
	TriNode<ItemType>* midChildPtr; //Pointer to the node's middle child (for a 3-Node)
	TriNode<ItemType>* rightChildPtr; //Pointer to the node's right child

public:
	TriNode();
	virtual ~TriNode();

	/*
	Checks if the node is empty
	@return True if it is empty, false if not
	*/
	bool isEmpty() const;

	/*
	Checks if the node is a leaf
	@return True if it is, false if not
	*/
	bool isLeaf() const;

	/*
	Checks if the node is a 2-Node or 3-Node
	@return True if it is, false if not
	*/
	bool isTwoNode() const;
	bool isThreeNode() const;

	/*
	Returns a pointer to the node's small or large item.
	@return The pointer to the item, NULL if the item does not exist.
	*/
	ItemType* getSmallItem() const;
	ItemType* getLargeItem() const;


	/*
	Sets the small or large item of the node to anItem
	@post The small or large item of the node is equal to anItem
	@param anItem The item to be inserted
	*/
	void setSmallItem(const ItemType& anItem);
	void setLargeItem(const ItemType& anItem);


	/*
	Removes the small or large item of the node
	@post The small or large item of the node is removed if it exists, otherwise nothing happens
	*/
	void removeSmallItem();
	void removeLargeItem();

	/*
	Retrieves the left, mid, or right child pointers of the node
	@return The pointer to the left, mid, or right child of the node
	*/
	TriNode<ItemType>* getLeftChildPtr() const;
	TriNode<ItemType>* getMidChildPtr() const;
	TriNode<ItemType>* getRightChildPtr() const;


	/*
	Sets the left, mid, or right child pointers to newNode, midNode and rightNode, respectively.
	@post The left, mid, or right child pointers are set to newNode, midNode, and rightNode, respectively
	@param newNode, midNode, rightNode The pointer to the new child
	*/
	void setLeftChildPtr(TriNode<ItemType>* newNode);
	void setMidChildPtr(TriNode<ItemType>* midNode);
	void setRightChildPtr(TriNode<ItemType>* rightNode);
};

#include "TriNode.cpp"

#endif
