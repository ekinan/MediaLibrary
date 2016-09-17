#ifndef _TWO_THREE_CPP
#define _TWO_THREE_CPP

#include <algorithm>
#include "TwoThreeTree.h"
#include "NotFoundException.h"

template <class ItemType>
TwoThreeTree<ItemType>::TwoThreeTree() //Default state of the tree is empty
{
	rootPtr = NULL;
}

template <class ItemType>
TriNode<ItemType>* TwoThreeTree<ItemType>::copyTree(const TriNode<ItemType>* otherTreePtr)
{
	if (otherTreePtr == NULL) //Returns NULL if otherTree is empty
		return NULL;
	else //Does a preorder copy
	{
		TriNode<ItemType>* newNode = new TriNode<ItemType>;
		newNode->setSmallItem(*(otherTreePtr->getSmallItem()));

		if (otherTreePtr->isThreeNode())
			newNode->setLargeItem(*(otherTreePtr->getLargeItem()));

		//Sets the corresponding childpointers of newNode to match those in otherTreePtr
		newNode->setLeftChildPtr(copyTree(otherTreePtr->getLeftChildPtr()));
		newNode->setMidChildPtr(copyTree(otherTreePtr->getMidChildPtr()));
		newNode->setRightChildPtr(copyTree(otherTreePtr->getRightChildPtr()));

		return newNode;
	}
}

template <class ItemType>
TwoThreeTree<ItemType>::TwoThreeTree(const TwoThreeTree<ItemType>& aTree) //Copy constructor
{
	rootPtr = copyTree(aTree.rootPtr);
}

template <class ItemType>
TwoThreeTree<ItemType>::~TwoThreeTree() //Destructor
{
	clear();
}

template <class ItemType>
bool TwoThreeTree<ItemType>::isEmpty() const
{
	return rootPtr == NULL;
}

template <class ItemType>
int TwoThreeTree<ItemType>::getHeightHelper(TriNode<ItemType>* subTreePtr) const
{
	if (subTreePtr == NULL) //Empty tree height is 0
		return 0;
	else if (subTreePtr->isTwoNode()) //2-Node case, computes the height of the left and right subtrees
	{
		return 1 + std::max(getHeightHelper(subTreePtr->getLeftChildPtr()),
					getHeightHelper(subTreePtr->getRightChildPtr()));
	}
	else //3-Node case, computes height of the left, middle and right subtrees
	{
		return 1 + std::max(std::max(getHeightHelper(subTreePtr->getLeftChildPtr()),
					getHeightHelper(subTreePtr->getMidChildPtr())),
						getHeightHelper(subTreePtr->getRightChildPtr()));
	}
}

template <class ItemType>
int TwoThreeTree<ItemType>::getHeight() const
{
	return getHeightHelper(rootPtr);
}


template <class ItemType>
int TwoThreeTree<ItemType>::getNumberOfItemsHelper(TriNode<ItemType>* subTreePtr) const
{
	if (subTreePtr == NULL)
		return 0;
	else if (subTreePtr->isTwoNode()) //2-node case, adds 1 to the # of items in the left and right
		return 1 + getNumberOfItemsHelper(subTreePtr->getLeftChildPtr()) //subtrees
				+ getNumberOfItemsHelper(subTreePtr->getRightChildPtr());
	else //3-node case, adds 2 to the # of items in the left, middle, and right subtrees.
		return 2 + getNumberOfItemsHelper(subTreePtr->getLeftChildPtr())
				+ getNumberOfItemsHelper(subTreePtr->getRightChildPtr())
					+ getNumberOfItemsHelper(subTreePtr->getMidChildPtr());
}



template <class ItemType>
int TwoThreeTree<ItemType>::getNumberOfItems() const
{
	return getNumberOfItemsHelper(rootPtr);
}


template <class ItemType>
void TwoThreeTree<ItemType>::findInsertLoc(TriNode<ItemType>* subTreePtr, ItemType* itemPtr)
{
	Stack<TriNode<ItemType>*> ptrStack; //Used to store the pointers of the nodes traversed
	while (!subTreePtr->isLeaf()) //during the search. Search stops when a leaf is found.
	{
		TriNode<ItemType>* nextPtr = NULL;
		if (*itemPtr < *(subTreePtr->getSmallItem()))
			nextPtr = subTreePtr->getLeftChildPtr();
		else if (subTreePtr->isThreeNode() && *itemPtr < *(subTreePtr->getLargeItem()))
			nextPtr = subTreePtr->getMidChildPtr();
		else
			nextPtr = subTreePtr->getRightChildPtr();

		ptrStack.push(subTreePtr);
		subTreePtr = nextPtr;
	}

	insertInLoc(subTreePtr, itemPtr, ptrStack); //Inserts the contents of itemPtr into the leaf pointed to
						    //by subTreePtr
}

template <class ItemType>
void TwoThreeTree<ItemType>::insertInLoc(TriNode<ItemType>* nodePtr, ItemType* itemPtr,
						Stack<TriNode<ItemType>*>& ptrStack)
{
	if (nodePtr->isTwoNode()) //2-node leaf becomes a 3-node
	{
		if (*itemPtr > *(nodePtr->getSmallItem()))
			nodePtr->setLargeItem(*itemPtr);
		else //It is smaller than the current item in the 2-Node
		{
			nodePtr->setLargeItem(*(nodePtr->getSmallItem()));
			nodePtr->setSmallItem(*itemPtr);
		}
	}
	else //We have a three node
	{
		itemPtr = getMiddleItem(nodePtr, itemPtr); //Reconfigure the node for splitting and pass
		TriNode<ItemType>* connectingPtr = split(nodePtr);//the middle item to nodePtr.
		while (!ptrStack.empty() && itemPtr != NULL) //While there is still an item passed
		{
			nodePtr = ptrStack.top(); //Get the parent pointer from the top of the stack of
			ptrStack.pop();	//nodes traversed.
			if (nodePtr->isTwoNode()) //If it is a 2-node, insert contents
			{			 //of itemPtr and make it a 3-node
				if (*itemPtr > *(nodePtr->getSmallItem()))
					nodePtr->setLargeItem(*itemPtr);
				else //It is smaller than the current item in the 2-Node
				{
					nodePtr->setLargeItem(*(nodePtr->getSmallItem()));
					nodePtr->setSmallItem(*itemPtr);
				}
				itemPtr = NULL; //Tree is now rebuilt, so there is no passed item.
				reconnect(nodePtr, connectingPtr); //Reconnect n1 and n2 from any previous
				connectingPtr = NULL;		  //split of a 3-node
			}
			else //We have a 3-node. Split this node, and then pass the middle item
			{
				itemPtr = getMiddleItem(nodePtr, itemPtr);
				connectingPtr = split(nodePtr, connectingPtr);
			}
		}
		if (ptrStack.empty() && itemPtr != NULL) //We've reached the root, so the empty node
		{					 //pointed to by connetingPtr becomes the new root
			connectingPtr->setSmallItem(*itemPtr); //containing the passed item.
			rootPtr = connectingPtr;
		}
	}
}

template <class ItemType>
void TwoThreeTree<ItemType>::reconnect(TriNode<ItemType>* nodePtr, TriNode<ItemType>* connectingPtr)
{
	if (connectingPtr == nodePtr->getLeftChildPtr()) //Case A in reconnect
	{
		nodePtr->setLeftChildPtr(connectingPtr->getLeftChildPtr());
		if (nodePtr->isThreeNode()) //Case A2 from "Reconnect Cases"
			nodePtr->setMidChildPtr(connectingPtr->getRightChildPtr());
		else //Case A1 from "Reconnect Cases"
			nodePtr->setRightChildPtr(connectingPtr->getRightChildPtr());
	}
	else //Connecting pointer is the right child
	{
		nodePtr->setRightChildPtr(connectingPtr->getRightChildPtr());
		if (nodePtr->isThreeNode()) //Case B2 from "Reconnect Cases"
			nodePtr->setMidChildPtr(connectingPtr->getLeftChildPtr());
		else //Case B1 from "Reconnect Cases"
			nodePtr->setLeftChildPtr(connectingPtr->getLeftChildPtr());
	}

	delete connectingPtr; //Delete the node occupied by connectingPtr
}

template <class ItemType>
TriNode<ItemType>* TwoThreeTree<ItemType>::split(TriNode<ItemType>* nodePtr, TriNode<ItemType>* connectingPtr)
{
	TriNode<ItemType>* n1 = new TriNode<ItemType>; //Create the nodes n1 and n2
	TriNode<ItemType>* n2 = new TriNode<ItemType>;


	n1->setSmallItem(*(nodePtr->getSmallItem())); //n1 gets nodePtr's small item
	nodePtr->removeSmallItem();
	n2->setSmallItem(*(nodePtr->getLargeItem())); //n2 gets nodePtr's large item
	nodePtr->removeLargeItem();

	if (connectingPtr != NULL) //Check if there are n1 and n2 from an earlier split and reconnect
	{				//accordingly
		if (connectingPtr == nodePtr->getLeftChildPtr()) //Case A in "Split Cases"
		{
			n1->setLeftChildPtr(connectingPtr);
			reconnect(n1, connectingPtr);
			n2->setLeftChildPtr(nodePtr->getMidChildPtr());
			n2->setRightChildPtr(nodePtr->getRightChildPtr());
		}
		else if (connectingPtr == nodePtr->getRightChildPtr()) //Case B in "Split Cases"
		{
			n2->setRightChildPtr(connectingPtr);
			reconnect(n2, connectingPtr);
			n1->setLeftChildPtr(nodePtr->getLeftChildPtr());
			n1->setRightChildPtr(nodePtr->getMidChildPtr());
		}
		else //Case C in "Split Cases"
		{
			n1->setLeftChildPtr(nodePtr->getLeftChildPtr());
			n1->setRightChildPtr(connectingPtr->getLeftChildPtr());
			n2->setLeftChildPtr(connectingPtr->getRightChildPtr());
			n2->setRightChildPtr(nodePtr->getRightChildPtr());
			delete connectingPtr;
		}
	}


	nodePtr->setMidChildPtr(NULL);
	nodePtr->setLeftChildPtr(n1);
	nodePtr->setRightChildPtr(n2);

	return nodePtr; //Return the new empty node
}

template <class ItemType>
ItemType* TwoThreeTree<ItemType>::getMiddleItem(TriNode<ItemType>* nodePtr, ItemType* passedItem)
{

	if (*passedItem < *(nodePtr->getSmallItem())) //The small item is in the middle
	{
		ItemType temp = *passedItem;
		*passedItem = *(nodePtr->getSmallItem());
		nodePtr->setSmallItem(temp);
	}
	else if (*passedItem > *(nodePtr->getLargeItem())) //The large item is the middle
	{
		ItemType temp = *passedItem;
		*passedItem = *(nodePtr->getLargeItem());
		nodePtr->setLargeItem(temp);
	}

	return passedItem;
}

template <class ItemType>
bool TwoThreeTree<ItemType>::add(const ItemType& newData)
{
	if (isEmpty()) //Special case if the tree is empty, create a single node that stores the item
	{
		rootPtr = new TriNode<ItemType>;
		rootPtr->setSmallItem(newData);
	}
	else //Nonempty tree
	{
		ItemType* itemPtr = new ItemType; //Create pointer that stores the new item,
		*itemPtr = newData;		 //used to facilitate passing items during
		findInsertLoc(rootPtr, itemPtr); //rebuilding of the tree
	}

	return true;
}




template <class ItemType>
void TwoThreeTree<ItemType>::mergeTwoParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
						TriNode<ItemType>* siblingPtr)
{
	if (emptyNodePtr == parentPtr->getLeftChildPtr()) //Case A of "Merging Nodes: Two Node Parent Cases"
	{
		siblingPtr->setLargeItem(*(siblingPtr->getSmallItem()));
		siblingPtr->setSmallItem(*(parentPtr->getSmallItem()));


		siblingPtr->setMidChildPtr(siblingPtr->getLeftChildPtr());
		siblingPtr->setLeftChildPtr(emptyNodePtr->getMidChildPtr());
	}
	else //Case B of "Merging Nodes: Two Node Parent Cases"
	{
		siblingPtr->setLargeItem(*(parentPtr->getSmallItem()));

		siblingPtr->setMidChildPtr(siblingPtr->getRightChildPtr());
		siblingPtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());
	}


	//Making the parent the new empty node
	parentPtr->removeSmallItem();
	parentPtr->setLeftChildPtr(NULL);
	parentPtr->setRightChildPtr(NULL);
	parentPtr->setMidChildPtr(siblingPtr);


	delete emptyNodePtr; //Removing the original empty node
}

template <class ItemType>
void TwoThreeTree<ItemType>::mergeThreeParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
						TriNode<ItemType>* siblingPtr)
{
	if (emptyNodePtr != parentPtr->getRightChildPtr())
	{
		if (emptyNodePtr == parentPtr->getLeftChildPtr()) //Case A of "Merging Nodes: 3-Node Parent Cases"
		{
			siblingPtr->setLargeItem(*(siblingPtr->getSmallItem()));
			siblingPtr->setSmallItem(*(parentPtr->getSmallItem()));

			siblingPtr->setMidChildPtr(siblingPtr->getLeftChildPtr());
			siblingPtr->setLeftChildPtr(emptyNodePtr->getMidChildPtr());
		}
		else //Case B of "Merging Nodes: 3-Node Parent Cases"
		{
			siblingPtr->setLargeItem(*(parentPtr->getSmallItem()));

			siblingPtr->setMidChildPtr(siblingPtr->getRightChildPtr());
			siblingPtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());
		}

		parentPtr->setSmallItem(*(parentPtr->getLargeItem()));

		parentPtr->setLeftChildPtr(siblingPtr);

	}
	else //Case C of "Merging Nodes: 3-Node Parent Cases"
	{
		siblingPtr->setLargeItem(*(parentPtr->getLargeItem()));

		siblingPtr->setMidChildPtr(siblingPtr->getRightChildPtr());
		siblingPtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());

		parentPtr->setRightChildPtr(siblingPtr);
	}


	//Turning the parent into a 2-Node
	parentPtr->setMidChildPtr(NULL);
	parentPtr->removeLargeItem();


	delete emptyNodePtr; //Removing the original, empty node
}

template <class ItemType>
void TwoThreeTree<ItemType>::redistributeTwoParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
							TriNode<ItemType>* siblingPtr)
{
	//Turning empty node back into a 2-Node
	emptyNodePtr->setSmallItem(*(parentPtr->getSmallItem()));

	if (emptyNodePtr == parentPtr->getLeftChildPtr()) //Case A of "Redistribute: 2-Node Parent Cases"
	{
		parentPtr->setSmallItem(*(siblingPtr->getSmallItem()));

		siblingPtr->setSmallItem(*(siblingPtr->getLargeItem()));

		emptyNodePtr->setLeftChildPtr(emptyNodePtr->getMidChildPtr());
		emptyNodePtr->setRightChildPtr(siblingPtr->getLeftChildPtr());

		siblingPtr->setLeftChildPtr(siblingPtr->getMidChildPtr());
	}
	else //Case B of "Redistribute: 2-Node Parent Cases"
	{
		parentPtr->setSmallItem(*(siblingPtr->getLargeItem()));

		emptyNodePtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());
		emptyNodePtr->setLeftChildPtr(siblingPtr->getRightChildPtr());

		siblingPtr->setRightChildPtr(siblingPtr->getMidChildPtr());
	}


	//Turning sibling into a 2-Node
	siblingPtr->removeLargeItem();
	siblingPtr->setMidChildPtr(NULL);


	//Resetting empty node's middle child
	emptyNodePtr->setMidChildPtr(NULL);
}


template <class ItemType>
void TwoThreeTree<ItemType>::redistributeThreeParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
							TriNode<ItemType>* siblingPtr)
{
	if (emptyNodePtr == parentPtr->getLeftChildPtr() || siblingPtr == parentPtr->getLeftChildPtr())
	{
		emptyNodePtr->setSmallItem(*(parentPtr->getSmallItem()));

		if (emptyNodePtr == parentPtr->getLeftChildPtr()) //Case A of "Redistribute: 3-Node Parent Cases"
		{
			parentPtr->setSmallItem(*(siblingPtr->getSmallItem()));

			siblingPtr->setSmallItem(*(siblingPtr->getLargeItem()));

			emptyNodePtr->setLeftChildPtr(emptyNodePtr->getMidChildPtr());
			emptyNodePtr->setRightChildPtr(siblingPtr->getLeftChildPtr());

			siblingPtr->setLeftChildPtr(siblingPtr->getMidChildPtr());
		}
		else //Case B of "Redistribute: 3-Node Parent Cases"
		{
			parentPtr->setSmallItem(*(siblingPtr->getLargeItem()));


			emptyNodePtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());
			emptyNodePtr->setLeftChildPtr(siblingPtr->getRightChildPtr());

			siblingPtr->setRightChildPtr(siblingPtr->getMidChildPtr());
		}
	}
	else if (emptyNodePtr == parentPtr->getRightChildPtr() || siblingPtr == parentPtr->getRightChildPtr())
	{
		emptyNodePtr->setSmallItem(*(parentPtr->getLargeItem()));

		if (emptyNodePtr == parentPtr->getRightChildPtr()) //Case C of "Redistribute: 3-Node Parent Cases"
		{
			parentPtr->setLargeItem(*(siblingPtr->getLargeItem()));


			emptyNodePtr->setRightChildPtr(emptyNodePtr->getMidChildPtr());
			emptyNodePtr->setLeftChildPtr(siblingPtr->getRightChildPtr());

			siblingPtr->setRightChildPtr(siblingPtr->getMidChildPtr());
		}
		else //Case D of "Redistribute: 3-Node Parent Cases"
		{

			parentPtr->setLargeItem(*(siblingPtr->getSmallItem()));

			siblingPtr->setSmallItem(*(siblingPtr->getLargeItem()));

			emptyNodePtr->setLeftChildPtr(emptyNodePtr->getMidChildPtr());
			emptyNodePtr->setRightChildPtr(siblingPtr->getLeftChildPtr());

			siblingPtr->setLeftChildPtr(siblingPtr->getMidChildPtr());
		}
	}

	//Convert sibling into a 2-Node
	siblingPtr->removeLargeItem();
	siblingPtr->setMidChildPtr(NULL);


	//Reset empty node into a 2-Node
	emptyNodePtr->setMidChildPtr(NULL);
}


template <class ItemType>
TriNode<ItemType>* TwoThreeTree<ItemType>::getSiblingPtr(TriNode<ItemType>* nodePtr, TriNode<ItemType>* parentPtr)
{
	TriNode<ItemType>* siblingPtr = NULL;

	if (nodePtr == parentPtr->getLeftChildPtr()) //Node is the parent's left child
	{
		if (parentPtr->isTwoNode()) //Sibling is the right child if the parent is a 2-node
			siblingPtr = parentPtr->getRightChildPtr();
		else	//Parent is a 3-Node
			siblingPtr = parentPtr->getMidChildPtr(); //Sibling is the middle child
	}
	else if (nodePtr == parentPtr->getRightChildPtr()) //Node is the parent's right child
	{
		if (parentPtr->isTwoNode()) //Sibling is the left child if the parent is a 2-node
			siblingPtr = parentPtr->getLeftChildPtr();
		else	//Parent is a 3-Node
			siblingPtr = parentPtr->getMidChildPtr(); //Sibling is the middle child
	}
	else //Node is the middle child of a 3-node parent
	{    //By default, adjacent sibling is the parent's left child
		if ((parentPtr->getRightChildPtr())->isThreeNode()) //But if right child is a 3-node sibling
			siblingPtr = parentPtr->getRightChildPtr(); //sibling becomes the right child instead
		else
			siblingPtr = parentPtr->getLeftChildPtr();
	}

	return siblingPtr; //Return the sibling pointer
}

template <class ItemType>
bool TwoThreeTree<ItemType>::removeValue(TriNode<ItemType>* subTreePtr, const ItemType& value)
{
	bool canRemove = false;
	Stack<TriNode<ItemType>*> ptrStack; //Stack to store the pointers of the nodes traversed.

	while (!canRemove && subTreePtr != NULL) //While the node containing the item hasn't been found,
	{					 //Or the end of the tree hasn't been reached
		TriNode<ItemType>* nextPtr = NULL;

		if (value < *(subTreePtr->getSmallItem())) //Move to the left subtree
			nextPtr = subTreePtr->getLeftChildPtr();
		else if (value == *(subTreePtr->getSmallItem())) //Found the value
			canRemove = true;
		else if (!subTreePtr->isThreeNode()) //Move to the right child if root of subtree isn't a 3-node
			nextPtr = subTreePtr->getRightChildPtr();
		else //Root of the subtree is a 3-node
		{
			if (*(subTreePtr->getSmallItem()) < value && value < *(subTreePtr->getLargeItem()))
				nextPtr = subTreePtr->getMidChildPtr(); //Move to the middle child
			else if (value == *(subTreePtr->getLargeItem())) //Found the item
				canRemove = true;
			else //Move to the right child
				nextPtr = subTreePtr->getRightChildPtr();
		}

		if (!canRemove) //Store subTreePtr in the stack if the node has not been found
		{
			ptrStack.push(subTreePtr);
			subTreePtr = nextPtr;
		}
	}

	if (canRemove) //If the item was found, we need to remove it accordingly
	{
		if (subTreePtr->isLeaf()) //Item is at a leaf
		{
			if (subTreePtr->isThreeNode()) //3-node leaf, simply remove the corresponding
			{			       //small or large item
				if (value == *(subTreePtr->getSmallItem()))
					subTreePtr->setSmallItem(*(subTreePtr->getLargeItem()));

				subTreePtr->removeLargeItem(); //3-node becomes a 2-node
			}
			else //We have a 2-Node leaf
				removeTwoNode(subTreePtr, ptrStack);
		}
		else //We have an internal node, so find the inorder successor
		{
			ptrStack.push(subTreePtr);
			ItemType* itemPtr = NULL; //Pointer to the item that's going to be swapped

			if (subTreePtr->isThreeNode()) //3-node case
			{
				if (value == *(subTreePtr->getSmallItem()))
				{
					itemPtr = subTreePtr->getSmallItem();
					subTreePtr = subTreePtr->getMidChildPtr(); //Successor is in the
				}						   //middle child.
				else //It is the large item of the 3-Node
				{
					itemPtr = subTreePtr->getLargeItem();
					subTreePtr = subTreePtr->getRightChildPtr(); //Successor is
				}						     //in the right child.
			}
			else //2-Node case
			{
				itemPtr = subTreePtr->getSmallItem();
				subTreePtr = subTreePtr->getRightChildPtr(); //Successor is in the right child
			}


			while (subTreePtr->getLeftChildPtr() != NULL) //Search for the inorder successor
			{
				ptrStack.push(subTreePtr); //Place the traversed nodes into the stack
				subTreePtr = subTreePtr->getLeftChildPtr();
			}

			if (subTreePtr->isThreeNode()) //Smaller item of the 3-node is the successor
			{
				*itemPtr = *(subTreePtr->getSmallItem());

				subTreePtr->setSmallItem(*(subTreePtr->getLargeItem())); //Convert to 2-Node
				subTreePtr->removeLargeItem();
			}
			else //Successor is in a 2-node
			{
				*itemPtr = *(subTreePtr->getSmallItem());
				removeTwoNode(subTreePtr, ptrStack); //Rebuild the tree by removing empty 2-Node
			}

		}
	}

	return canRemove;
}

template <class ItemType>
void TwoThreeTree<ItemType>::removeTwoNode(TriNode<ItemType>* subTreePtr, Stack<TriNode<ItemType>*>& ptrStack)
{
	if (subTreePtr == rootPtr) //Case of a single node tree
	{
		delete subTreePtr;
		rootPtr = NULL;
	}
	else //2-Node leaf
	{
		subTreePtr->removeSmallItem(); //Make the node empty

		bool isRemoved = false;

		while (!ptrStack.empty() && !isRemoved) //Stack isn't empty (i.e. root hasn't been reached)
		{					//or there is still an empty 2-node
			TriNode<ItemType>* parentPtr = ptrStack.top(); //Get the parent pointer from stack
			ptrStack.pop();

			TriNode<ItemType>* siblingPtr = getSiblingPtr(subTreePtr, parentPtr); //Get the sibling

			if (parentPtr->isTwoNode())
			{
				if (siblingPtr->isTwoNode()) //Merge if the sibling is a 2-node
				{
					mergeTwoParent(subTreePtr, parentPtr, siblingPtr);
					subTreePtr = parentPtr; //The parent is now the empty node
				}
				else //3-node sibling, redistribute
				{
					isRemoved = true;
					redistributeTwoParent(subTreePtr, parentPtr, siblingPtr);
				}
			}
			else //3-Node parent
			{
				isRemoved = true;
				if (siblingPtr->isTwoNode()) //Merge if the sibling is a 2-node
					mergeThreeParent(subTreePtr, parentPtr, siblingPtr);
				else //3-Node sibling, redistribute
					redistributeThreeParent(subTreePtr, parentPtr, siblingPtr);
			}
		}

		if (rootPtr->isEmpty()) //The empty node is now the root, simply connect rootPtr
		{			//with its middle child
			rootPtr = subTreePtr->getMidChildPtr();
			delete subTreePtr; //Remove the node
		}
	}
}


template <class ItemType>
bool TwoThreeTree<ItemType>::remove(const ItemType& anEntry)
{
	return removeValue(rootPtr, anEntry);
}







template <class ItemType>
void TwoThreeTree<ItemType>::postorderDelete(TriNode<ItemType>* subTreePtr)
{
	if (subTreePtr != NULL)
	{
		if (subTreePtr->isTwoNode()) //If it's a 2-node, postorder is the same as a normal binary tree
		{
			postorderDelete(subTreePtr->getLeftChildPtr());
			postorderDelete(subTreePtr->getRightChildPtr());
			delete subTreePtr;
		}
		else //3-Node, same except we now add the middle child
		{
			postorderDelete(subTreePtr->getLeftChildPtr());
			postorderDelete(subTreePtr->getMidChildPtr());
			postorderDelete(subTreePtr->getRightChildPtr());
			delete subTreePtr;
		}
	}
}

template <class ItemType>
void TwoThreeTree<ItemType>::clear()
{
	postorderDelete(rootPtr);
	rootPtr = NULL;
}


template <class ItemType>
TriNode<ItemType>* TwoThreeTree<ItemType>::findItem(TriNode<ItemType>* subTreePtr, const ItemType& anEntry) const
{
	if (subTreePtr == NULL) //Item does not exist in the tree
	{
		return NULL;
	}
	else
	{
		if (anEntry < *(subTreePtr->getSmallItem())) //Move to left child if item < small item of node
			return findItem(subTreePtr->getLeftChildPtr(), anEntry);
		else if (anEntry == *(subTreePtr->getSmallItem())) //Found the item
			return subTreePtr;
		else if (subTreePtr->isThreeNode()) //Check 3-node case
		{
			if (anEntry < *(subTreePtr->getLargeItem())) //Move to the middle child if item
				return findItem(subTreePtr->getMidChildPtr(), anEntry); //< large item
			else if (anEntry == *(subTreePtr->getLargeItem())) //Found the item
				return subTreePtr;
			else //Move to the right child
				return findItem(subTreePtr->getRightChildPtr(), anEntry);
		}
		else //Move to the right child
			return findItem(subTreePtr->getRightChildPtr(), anEntry);
	}
}


template <class ItemType>
ItemType TwoThreeTree<ItemType>::getEntry(const ItemType& anEntry) const
{
	TriNode<ItemType>* nodePtr = findItem(rootPtr, anEntry);
	if (nodePtr != NULL)
		return anEntry;
	else //Throw exception if the entry does not exist
		throw(NotFoundException("getEntry() called with a nonexistant item."));
}

template <class ItemType>
bool TwoThreeTree<ItemType>::contains(const ItemType& anEntry) const
{
	TriNode<ItemType>* nodePtr = findItem(rootPtr, anEntry);
	if (nodePtr != NULL)
		return true;
	else
		return false;
}



template <class ItemType>
void TwoThreeTree<ItemType>::inorderHelper(TriNode<ItemType>* subTreePtr, void visit(ItemType&)) const
{
	if (subTreePtr != NULL)
	{
		if (subTreePtr->isTwoNode()) //If it's a 2-node, inorder is the same as a normal binary tree
		{
			inorderHelper(subTreePtr->getLeftChildPtr(), visit);
			visit(*(subTreePtr->getSmallItem()));
			inorderHelper(subTreePtr->getRightChildPtr(), visit);
		}
		else //Account for the extra item in a 3-node, as well as its middle child
		{
			inorderHelper(subTreePtr->getLeftChildPtr(), visit);
			visit(*(subTreePtr->getSmallItem()));
			inorderHelper(subTreePtr->getMidChildPtr(), visit);
			visit(*(subTreePtr->getLargeItem()));
			inorderHelper(subTreePtr->getRightChildPtr(), visit);
		}
	}
}

/* IGNORE
template <class ItemType>
void TwoThreeTree<ItemType>::printNode(TriNode<ItemType>* subTreePtr) const
{
	std::cout << "<" << *(subTreePtr->getSmallItem());
	if (subTreePtr->isThreeNode())
		std::cout << " " << *(subTreePtr->getLargeItem());
	std::cout << ">" << "  ";
}

template <class ItemType>
void TwoThreeTree<ItemType>::levelOrderTraverseHelper(TriNode<ItemType>* subTreePtr, int currentLevel,
								int maxLevel) const
{
	if (subTreePtr != NULL)
	{
		if (currentLevel == maxLevel)
		{
			printNode(subTreePtr);
		}
		else
		{
			levelOrderTraverseHelper(subTreePtr->getLeftChildPtr(), ++currentLevel, maxLevel);
			--currentLevel;
			levelOrderTraverseHelper(subTreePtr->getMidChildPtr(), ++currentLevel, maxLevel);
			--currentLevel;
			levelOrderTraverseHelper(subTreePtr->getRightChildPtr(), ++currentLevel, maxLevel);
			--currentLevel;
				std::cout << "  |  ";
		}
	}
}

template <class ItemType>
void TwoThreeTree<ItemType>::levelOrderTraverse() const
{
	int height = getHeight();
	for (int i = 1; i <= height; i++)
	{
		levelOrderTraverseHelper(rootPtr, 1, i);
		std::cout << std::endl;
	}
}

*/
template <class ItemType>
void TwoThreeTree<ItemType>::traverse(void visit(ItemType&)) const
{
	inorderHelper(rootPtr, visit);
}

template <class ItemType>
void TwoThreeTree<ItemType>::inorderFileWrite(TriNode<ItemType>* subTreePtr, std::ostream& outFile) const
{
	//Same as inorderhelper, except every item in the node has a function writeToFile
	//(see the class MediaEntry for more details)
	if (subTreePtr != NULL)
	{
		if (subTreePtr->isTwoNode()) //If it's a 2-node, inorder is the same as a normal binary tree
		{
			inorderFileWrite(subTreePtr->getLeftChildPtr(), outFile);
			(*(subTreePtr->getSmallItem())).writeToFile(outFile);
			inorderFileWrite(subTreePtr->getRightChildPtr(), outFile);
		}
		else //Modify for 3-node
		{
			inorderFileWrite(subTreePtr->getLeftChildPtr(), outFile);
			(*(subTreePtr->getSmallItem())).writeToFile(outFile);
			inorderFileWrite(subTreePtr->getMidChildPtr(), outFile);
			(*(subTreePtr->getLargeItem())).writeToFile(outFile);
			inorderFileWrite(subTreePtr->getRightChildPtr(), outFile);
		}
	}
}

template <class ItemType>
void TwoThreeTree<ItemType>::writeToFile(std::ostream& outFile) const
{
	inorderFileWrite(rootPtr, outFile);
}

template <class ItemType>
void TwoThreeTree<ItemType>::getNumNodes(TriNode<ItemType>* subTreePtr, int& numTwoNodes,
						int& numThreeNodes) const
{
	if (subTreePtr != NULL)
	{
		if (subTreePtr->isTwoNode())
		{
			numTwoNodes++;
			getNumNodes(subTreePtr->getLeftChildPtr(), numTwoNodes, numThreeNodes);
			getNumNodes(subTreePtr->getRightChildPtr(), numTwoNodes, numThreeNodes);
		}
		else //3-node
		{
			numThreeNodes++;
			getNumNodes(subTreePtr->getLeftChildPtr(), numTwoNodes, numThreeNodes);
			getNumNodes(subTreePtr->getMidChildPtr(), numTwoNodes, numThreeNodes);
			getNumNodes(subTreePtr->getRightChildPtr(), numTwoNodes, numThreeNodes);

		}
	}
}

template <class ItemType>
void TwoThreeTree<ItemType>::displayStatistics(std::ostream& os) const
{
	int numTwoNodes = 0;
	int numThreeNodes = 0;
	int height = getHeight();
	getNumNodes(rootPtr, numTwoNodes, numThreeNodes);

	os << "Tree height: " << height << std::endl;
	os << "Number of two nodes: " << numTwoNodes << std::endl;
	os << "Number of three nodes: " << numThreeNodes << std::endl;
	os << "Number of items: " << getNumberOfItems() << std::endl;
	os << std::endl << std::endl;
}


#endif

