#ifndef _TRI_NODE_CPP
#define _TRI_NODE_CPP

#include "TriNode.h"
#include <cstdlib>
#include <iostream>

template <class ItemType>
TriNode<ItemType>::TriNode() //Set the default state of the node to an empty leaf
{
	smallItem = NULL;
	largeItem = NULL;
	leftChildPtr = NULL;
	midChildPtr = NULL;
	rightChildPtr = NULL;
}

template <class ItemType>
TriNode<ItemType>::~TriNode()
{
	if (smallItem != NULL) //Deallocate the dynamic memory
		delete smallItem;
	if (largeItem != NULL)
		delete largeItem;
}

template <class ItemType>
bool TriNode<ItemType>::isEmpty() const //An empty node is a node that has no items
{
	return (smallItem == NULL && largeItem == NULL);
}

template <class ItemType>
bool TriNode<ItemType>::isLeaf() const //A leaf is a node that has no child
{
	if ((leftChildPtr == NULL) && (midChildPtr == NULL) && (rightChildPtr == NULL))
		return true;
	else
		return false;
}

template <class ItemType>
bool TriNode<ItemType>::isTwoNode() const //A 2-node has a small item, but no large item
{
	if (smallItem != NULL && largeItem == NULL)
		return true;
	else
		return false;
}

template <class ItemType>
bool TriNode<ItemType>::isThreeNode() const //A 3-node has a small and large item
{
	if (smallItem != NULL && largeItem != NULL)
		return true;
	else
		return false;
}

template <class ItemType>
ItemType* TriNode<ItemType>::getSmallItem() const //Returning a pointer avoids having to deal with exceptions
{
	return smallItem;
}

template <class ItemType>
ItemType* TriNode<ItemType>::getLargeItem() const
{
	return largeItem;
}

template <class ItemType>
void TriNode<ItemType>::setSmallItem(const ItemType& anItem)
{
	if (smallItem == NULL) //Creates a new memory location for the item if it does not exist
		smallItem = new ItemType;

	*smallItem = anItem;
}

template <class ItemType>
void TriNode<ItemType>::setLargeItem(const ItemType& anItem)
{
	if (largeItem == NULL)
		largeItem = new ItemType;

	*largeItem = anItem;
}

template <class ItemType>
void TriNode<ItemType>::removeSmallItem()
{
	if (smallItem != NULL) //Checks if there is an entry prior to deallocating the dynamic memory
	{
		delete smallItem;
		smallItem = NULL;
	}
}

template <class ItemType>
void TriNode<ItemType>::removeLargeItem()
{
	if (largeItem != NULL)
	{
		delete largeItem;
		largeItem = NULL;
	}
}

template <class ItemType>
TriNode<ItemType>* TriNode<ItemType>::getLeftChildPtr() const
{
	return leftChildPtr;
}

template <class ItemType>
TriNode<ItemType>* TriNode<ItemType>::getMidChildPtr() const
{
	return midChildPtr;
}

template <class ItemType>
TriNode<ItemType>* TriNode<ItemType>::getRightChildPtr() const
{
	return rightChildPtr;
}

template <class ItemType>
void TriNode<ItemType>::setLeftChildPtr(TriNode<ItemType>* newNode)
{
	leftChildPtr = newNode;
}

template <class ItemType>
void TriNode<ItemType>::setMidChildPtr(TriNode<ItemType>* newNode)
{
	midChildPtr = newNode;
}

template <class ItemType>
void TriNode<ItemType>::setRightChildPtr(TriNode<ItemType>* newNode)
{
	rightChildPtr = newNode;
}

#endif


