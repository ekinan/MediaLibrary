/*@file Stack.h*/
#ifndef _STACK_H
#define _STACK_H

#include "NotFoundException.h"

template <class ItemType> //Struct for the stack node, renamed to SNode so it won't conflict
struct SNode		  //with existing Node structs.
{
	ItemType item;
	SNode<ItemType>* next;
};


template <class ItemType>
class Stack
{
private:
	SNode<ItemType>* topPtr; //Pointer to the top of the stack

public:
	Stack();
	Stack(const Stack& otherStack); //Copy constructor
	virtual ~Stack();

	/*
	Checks to see if the stack is empty
	@return True if it is empty, false otherwise
	*/
	bool empty() const;

	/*
	Pushes an item to the top of the stack
	@post top of the stack contains the new item
	@param item The item to be pushed
	@return True if successful, false otherwise
	*/
	bool push(const ItemType& item);

	/*
	Pops an item from the top of the stack
	@post Top item is removed from the stack if the stack isn't empty, otherwise
	nothing happens
	@return True if the pop was successful, otherwise false
	*/
	bool pop();

	/*
	Returns the top item of the stack or throws an exception if the stack is empty
	@return Top of the stack if the stack is nonempty, otherwise an exception is thrown
	*/
	ItemType top() const;
};

#include "Stack.cpp"

#endif
