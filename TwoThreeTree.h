/*@file TwoThreeTree.h*/
#ifndef _TWO_THREE_TREE_H
#define _TWO_THREE_TREE_H

#include "BalancedSearchTreeInterface.h"
#include "TriNode.h"
#include "Stack.h"
#include <iostream>

template <class ItemType>
class TwoThreeTree : public BalancedSearchTreeInterface<ItemType>
{
private:
	TriNode<ItemType>* rootPtr; //Pointer to the root of the tree


	/*
	Recursively copies the contents of otherTreePtr in preorder
	@post The tree is identical to the tree otherTreePtr
	@param otherTreePtr Pointer to the root of otherTree
	@return A pointer to the copied node
	*/
	TriNode<ItemType>* copyTree(const TriNode<ItemType>* otherTreePtr);

	/*
	Computes the height of the tree
	@param subTreePtr Pointer to the root of the subtree
	@return The height of the tree. Empty tree has 0 height.
	*/
	int getHeightHelper(TriNode<ItemType>* subTreePtr) const;


	/*
	Computes the number of items in the tree
	@param subTreePtr Pointer to the root of the subtree
	@return The number of items in the tree. Empty tree has 0 items.
	*/
	int getNumberOfItemsHelper(TriNode<ItemType>* subTreePtr) const;



	/*
	Finds the location to insert the contents of itemPtr
	@post Once found, calls the function insertInLoc to insert the contents of itemPtr
	@param subTreePtr Pointer to the root of the subtree
	itemPtr Pointer to the memory location of the item to be inserted.
	*/
	void findInsertLoc(TriNode<ItemType>* subTreePtr, ItemType* itemPtr);

	/*
	Inserts the contents of itemPtr into the tree, and then rebuilds the tree if necesary
	@post The contents of itemPtr is inserted into the tree, and the tree remains a 2-3 tree after
	@param nodePtr Pointer to the insertion node
	itemPtr Pointer to the memory location of the item to be inserted
	ptrStack Stack storing the pointers to the nodes traversed in findInsertLoc
	*/
	void insertInLoc(TriNode<ItemType>* nodePtr, ItemType* itemPtr,
				Stack<TriNode<ItemType>*>& ptrStack);


	/*
	Readjusts the contents of the 3-Node, and then returns a pointer to the passed, middle item.
	@post The 3-node pointed to by nodePtr is sorted, and its middle item is returned.
	@param nodePtr Pointer to the 3-Node
	passedItem Pointer to the passed item
	@return Returns a pointer to the middle item
	*/
	ItemType* getMiddleItem(TriNode<ItemType>* nodePtr, ItemType* passedItem);

	 /*
	Splits a 3-node into two smaller nodes n1 and n2. Returns a pointer to an empty node
	whose left and right childs are n1 and n2, respectively.
	@post The 3-node pointed to by nodePtr is split into two nodes n1 and n2. n1 houses the smaller item,
	n2 houses the larger item. nodePtr becomes connectingPtr, or the new empty node
	@param nodePtr A pointer to the 3-node
	connectingPtr A pointer to an empty node containing n1 and n2 from an earlier split, used to reconnect
	these nodes
	@return Returns a pointer to an empty node containing n1 and n2 as its left and right childs, respectively.
	*/
	TriNode<ItemType>* split(TriNode<ItemType>* nodePtr, TriNode<ItemType>* connectingPtr = NULL);

	/*
	Reconnects the left and right childs of the empty node pointed to by connectingPtr
	from an earlier 3-node split to nodePtr, the parent
	@post nodePtr has the nodes n1 and n2, the left and right childs of connetingPtr, respectively
	as its children.
	@param nodePtr Pointer to the parent node for reconnection
	connectingPtr Pointer to the empty node whose left and right children are n1 and n2, respectively.
	These are the two nodes resulting from the earlier split of a 3-Node
	*/
	void reconnect(TriNode<ItemType>* nodePtr, TriNode<ItemType>* connectingPtr);












	/*
	Merges the 2-nodes parentPtr and siblingPtr into a single 3-Node based on the cases outlined
	by the document in the design write-up/
	@post parentPtr becomes the new empty node, and siblingPtr is now a 3-node. emptyNodePtr is
	removed from the tree.
	@param emptyNodePtr Pointer to the empty node
	parentPtr Pointer to the parent node
	siblingPtr Pointer to the sibling node
	*/
	void mergeTwoParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
				TriNode<ItemType>* siblingPtr);


	/*
	Converts a 3-node parentPtr and 2-node siblingPtr into a 2-node and 3-node, respectively,
	based on the cases outlined in the design write-up. Also removes the node emptyNodePtr
	@post parentPtr is a 2-node and siblingPtr is a 3-node. emptyNodePtr is removed from the tree
	@param emptyNodePtr Pointer to the empty node
	parentPtr Pointer to the parent node
	siblingPtr Pointer to the sibling node
	*/
	void mergeThreeParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
				TriNode<ItemType>* siblingPtr);


	/*
	Redistributes the contents of the 3-node siblingPtr across emptyNodePtr and 2-node parentPtr
	according to the cases outlined in the design write-up
	@post siblingPtr and emptyNodePtr are 2-nodes. parentPtr is still a 2-node.
	@param emptyNodePtr Pointer to the empty node
	parentPtr Pointer to the parent node
	siblingPtr Pointer to the sibling node
	*/
	void redistributeTwoParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
				TriNode<ItemType>* siblingPtr);


	/*
	Redistributes the contents of the 3-node siblingPtr across emptyNodePtr and 3-node parentPtr
	according to the cases outlined in the design write-up
	@post siblingPtr and emptyNodePtr are 2-nodes. parentPtr is still a 3-node.
	@param emptyNodePtr Pointer to the empty node
	parentPtr Pointer to the parent node
	siblingPtr Pointer to the sibling node
	*/
	void redistributeThreeParent(TriNode<ItemType>* emptyNodePtr, TriNode<ItemType>* parentPtr,
				TriNode<ItemType>* siblingPtr);

	/*
	Returns a pointer to the adjacent sibling of nodePtr corresponding to the cases outlined
	in the design write-up
	@param nodePtr A pointer to the node whose sibling is needed
	parentPtr A pointer to the parent of nodePtr
	@return Returns a pointer to the adjacent sibling of nodePtr
	*/
	TriNode<ItemType>* getSiblingPtr(TriNode<ItemType>* nodePtr, TriNode<ItemType>* parentPtr);


	/*
	Attempts to remove the item, value, from the tree.
	@post The item, value, is removed from the tree if it is in the tree, otherwise nothing happens.
	If value is in a 2-node leaf, the function removeTwoNode is called. If it is a 3-node leaf, it is
	simply removed, and the 3-node leaf becomes a 2-node. If it is an internal node, it swaps the value of that
	node with the node's inorder successor, and then proceeds to remove resulting 2-node or 3-node leaf
	@param subTreePtr A pointer to the root of the tree
	value The value to be removed
	@return True if the value was removed, false otherwise
	*/
	bool removeValue(TriNode<ItemType>* subTreePtr, const ItemType& value);


	/*
	Removes the 2-node root of subTreePtr.
	@post The 2-node pointed to by subTreePtr is removed from the tree, and the tree remains a
	2-3 tree.
	@param subTreePtr Pointer to the 2-node to be removed.
	ptrStack Stack storing the pointers of the nodes traversed in removeValue.
	*/
	void removeTwoNode(TriNode<ItemType>* subTreePtr, Stack<TriNode<ItemType>*>& ptrStack);






	/*
	Deletes the contents of the tree in a postorder fashion
	@post The tree is empty
	@param subTreePtr Pointer to the root of the subtree
	*/
	void postorderDelete(TriNode<ItemType>* subTreePtr);





	/*
	Returns a pointer to the node containing the item anEntry.
	@param subTreePtr Pointer to the root of the subtree
	anEntry The item to be located
	@return Pointer to the node containing anEntry. Returns NULL if anEntry does not exist
	*/
	TriNode<ItemType>* findItem(TriNode<ItemType>* subTreePtr, const ItemType& anEntry) const;



	/*
	Traverses the tree in an inorder fashion, and executes the client-defined function visit on the items
	in each node.
	@post Visit is executed for every entry in the tree
	@param subTreePtr Pointer to the root of the subtree
	visit Client defined function
	*/
	void inorderHelper(TriNode<ItemType>* subTreePtr, void visit(ItemType&)) const;


/*
	//IGNORE printNode and levelOrderTraverseHelper, these were used for debugging.
	void printNode(TriNode<ItemType>* subTreePtr) const;
	void levelOrderTraverseHelper(TriNode<ItemType>* subTreePtr, int currentLevel,
					int maxLevel) const;
*/



	/*
	Writes the contents of the tree to the file outFile in an inorder fashion
	@post The entries of the tree are contained in outFile
	@param subTreePtr Pointer to the root of the tree
	outFile ostream variable containing the file opened
	*/
	void inorderFileWrite(TriNode<ItemType>* subTreePtr, std::ostream& outFile) const;


	/*
	Computes the number of 2-node and 3-nodes in the tree.
	@param subTreePtr Pointer to the root of the subtree
	numTwoNodes Parameter that stores the number of two nodes
	numThreeNodes Parameter that stores the number of three nodes
	*/
	void getNumNodes(TriNode<ItemType>* subTreePtr, int& numTwoNodes, int& numThreeNodes) const;


public:
	TwoThreeTree();
	TwoThreeTree(const TwoThreeTree<ItemType>& aTree);
	virtual ~TwoThreeTree();


	//These are all specified in BalancedSearchTreeInterface.h
	bool isEmpty() const;
	int getHeight() const;
	int getNumberOfItems() const;
	bool add(const ItemType& newData);
	bool remove(const ItemType& anEntry);
	void clear();
	ItemType getEntry(const ItemType& anEntry) const;
	bool contains(const ItemType& anEntry) const;
	void traverse(void visit(ItemType&)) const;




/*IGNORE	void levelOrderTraverse() const;*/


	/*
	Writes the contents of the tree to the file opened by outFile
	@post The contents of the tree are written to outFile
	@param outFile Ostream variable storing the file
	*/
	void writeToFile(std::ostream& outFile) const;


	/*
	Writes out the relevant statistics of the tree to the ostream variable os
	@post Outputs the height, number of 2 and 3-nodes, and items in the tree to os
	@param os The ostream variable for the output
	*/
	void displayStatistics(std::ostream& os) const;
};

#include "TwoThreeTree.cpp"

#endif
