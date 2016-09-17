#ifndef _STACK_CPP
#define _STACK_CPP

#include "Stack.h"
#include <iostream>

template <class ItemType>
Stack<ItemType>::Stack()
{
	topPtr = NULL;
}

template <class ItemType>
Stack<ItemType>::Stack(const Stack& otherStack)
{
	topPtr = NULL;
	if (otherStack.topPtr != NULL) //Copy the contents of the other stack. Similar to
	{			       //the copy constructor of the HashTable for the linked chains.
		topPtr = new SNode<ItemType>; //Refer to HashTable.cpp for more details
		topPtr->item = otherStack.topPtr->item;

		SNode<ItemType>* otherCurrent = otherStack.topPtr->next;
		SNode<ItemType>* storeFirstPtr = topPtr;


		while (otherCurrent != NULL)
		{
			topPtr->next = new SNode<ItemType>;
			topPtr = topPtr->next;
			topPtr->item = otherCurrent->item;
			otherCurrent = otherCurrent->next;
		}

		topPtr->next = NULL;
		topPtr = storeFirstPtr;
	}
}

template <class ItemType>
Stack<ItemType>::~Stack()
{
	while (topPtr != NULL) //Deletes the contents of the stack
	{
		SNode<ItemType>* storeNext = topPtr->next;
		delete topPtr;
		topPtr = storeNext;
	}
}

template <class ItemType>
bool Stack<ItemType>::empty() const
{
	return (topPtr == NULL);
}

template <class ItemType>
bool Stack<ItemType>::push(const ItemType& item)
{
	SNode<ItemType>* prev = topPtr; //Stores the previous top of the stack
	topPtr = new SNode<ItemType>;	//and inserts item on top
	topPtr->item = item;
	topPtr->next = prev;

	return true;
}

template <class ItemType>
bool Stack<ItemType>::pop()
{
	bool ableToPop = (!empty());
	if (ableToPop) //Pops the top of the stack if it is nonempty
	{
		SNode<ItemType>* storeNext = topPtr->next;
		delete topPtr;
		topPtr = storeNext;
	}

	return ableToPop;
}

template <class ItemType>
ItemType Stack<ItemType>::top() const
{
	if (!empty()) //Returns top of the stack if stack is nonempty
		return topPtr->item;
	else //Throws an exception
		throw(NotFoundException("top() called with an empty stack"));
}


#endif
