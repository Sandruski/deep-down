#ifndef nTREE_H
#define nTREE_H

#include <iostream>
#include <assert.h>
using namespace std;

#include "p2List.h"

#define max(a, b) (a > b) ? a : b;

/* -- NODE CLASS -- */

template<class Type>
class node {
public:
	node();
	node(const Type& value);
	~node();

	void setParent(node* parent);

	void addChild(node* child);

	void removeChild(node* child);
	void removeChildren();

	Type getValue() const;
	node* getParent() const;
	node* getChild(uint index) const;

private:
	Type value;
	node* parent;

public:
	p2List<node*> children;
};

template<class Type>
node<Type>::node()
{
	this->parent = NULL;
}

template<class Type>
node<Type>::node(const Type& value)
{
	this->value = value;
	this->parent = NULL;
}

template<class Type>
node<Type>::~node()
{
	removeChildren();
}

template<class Type>
void node<Type>::setParent(node* parent)
{
	assert(parent);
	this->parent = parent;
}

template<class Type>
void node<Type>::addChild(node* child)
{
	assert(child);

	child->parent = this;
	children.add(child);
}

template<class Type>
void node<Type>::removeChild(node* child)
{
	children.del(children.At(children.find(child)));
}

template<class Type>
void node<Type>::removeChildren()
{
	p2List_item<node*>* children_it = children.start;

	while (children_it != nullptr) {
		node* del = children_it->data;
		RELEASE(del);
		children_it = children_it->next;
	}

	children.clear();
}

template<class Type>
Type node<Type>::getValue() const
{
	return this->value;
}

template<class Type>
node<Type>* node<Type>::getParent() const
{
	return this->parent;
}

template<class Type>
node<Type>* node<Type>::getChild(uint index) const
{
	if (children.At(index))
		return children.At(index)->data;
}

/* -- nTREE CLASS -- */

template<class Type>
class nTree {
public:
	nTree();
	nTree(const Type& value);
	~nTree();

	bool insertChild(const Type& value, const Type& parentValue);

	bool remove(const Type& value);
	node<Type>* search(const Type& value) const;
	void clear();

	bool empty() const;
	int size() const;
	int getNumLevels(node<Type>* n) const;

	void printPreOrder() const;
	void recursivePreOrderList(node<Type>* n, p2List<Type>* preorder_list) const;
	//void printInOrder() const;
	//void printPostOrder() const;
	node<Type>* getRoot();
	Type getValue(const node<Type>* node) const;

private:
	node<Type>* recursiveSearch(const Type& value, node<Type>* n) const;
	int recursiveCount(node<Type>* n, uint size) const;
	int recursiveLevelsCount(node<Type>* n) const;
	void printRecursivePreOrder(node<Type>* n) const;
	//void printRecursiveInOrder(node<Type>* n) const;
	//void printRecursivePostOrder(node<Type>* n) const;

	node<Type>* root;
};

template<class Type>
nTree<Type>::nTree()
{
	root = NULL;
}

template<class Type>
nTree<Type>::nTree(const Type& value)
{
	root = new node<Type>(value);
}

template<class Type>
nTree<Type>::~nTree()
{
	clear();
}

template<class Type>
node<Type>* nTree<Type>::getRoot()
{
	return root;
}

template<class Type>
bool nTree<Type>::insertChild(const Type& value, const Type& parentValue)
{
	node<Type>* n = search(parentValue);
	if (n != NULL)
	{
		node<Type>* child = new node<Type>(value);
		n->addChild(child);
		return true;
	}
	else
	{
		return false;
	}
}

template<class Type>
bool nTree<Type>::remove(const Type& value)
{
	node<Type>* n = search(value);
	if (n != NULL)
	{
		node<Type>* parent = n->getParent();
		if (parent != NULL)
		{
			parent->removeChild(n);
		}
		else
		{
			// We want to delete the root
			clear();
		}
		return true;
	}
	else
	{
		return false;
	}
}

template<class Type>
node<Type>* nTree<Type>::search(const Type& value) const
{
	return recursiveSearch(value, root);
}

template<class Type>
Type nTree<Type>::getValue(const node<Type>* node) const
{
	return node->getValue();
}


template<class Type>
node<Type>* nTree<Type>::recursiveSearch(const Type& value, node<Type>* n) const
{
	if (n != NULL)
	{
		if (value == n->getValue())
			return n;

		p2List_item<node<Type>*>* children_it = n->children.start;

		while (children_it != nullptr) {

			node<Type>* found = recursiveSearch(value, children_it->data);

			if (found != NULL)
				return found;

			children_it = children_it->next;
		}
	}

	return NULL;
}

template<class Type>
void nTree<Type>::clear()
{
	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
}

template<class Type>
bool nTree<Type>::empty() const
{
	return root == NULL;
}

template<class Type>
int nTree<Type>::size() const
{
	uint size = 0;
	return recursiveCount(root, size) + 1;
}

template<class Type>
int nTree<Type>::recursiveCount(node<Type>* n, uint size) const
{
	if (n != NULL)
	{
		p2List_item<node<Type>*>* children_it = n->children.start;

		while (children_it != nullptr) {
			size = 1 + recursiveCount(children_it->data, size);
			children_it = children_it->next;
		}

		return size;
	}

	return -1;
}

template<class Type>
int nTree<Type>::getNumLevels(node<Type>* n) const
{
	return recursiveLevelsCount(n) + 1;
}

template<class Type>
int nTree<Type>::recursiveLevelsCount(node<Type>* n) const
{
	if (n == NULL)
	{
		return 0;
	}
	else if (n->children.start == NULL)
	{
		return 1;
	}
	else
	{
		int depth = recursiveLevelsCount(n->getChild(0));

		return 1 + depth;
	}
}

template<class Type>
void nTree<Type>::printPreOrder() const
{
	if (root == NULL)
	{
		std::cout << "EMPTY TREE" << std::endl;
	}
	else
	{
		std::cout << "PreOrder: ";
		printRecursivePreOrder(root);
		std::cout << std::endl;
	}
}

template<class Type>
void nTree<Type>::printRecursivePreOrder(node<Type>* n) const
{
	// PreOrder: node - leftChild - rightChild

	if (n != NULL)
	{
		cout << n->getValue() << " - ";

		p2List_item<node<Type>*>* children_it = n->children.start;

		while (children_it != nullptr) {
			printRecursivePreOrder(children_it->data);
			children_it = children_it->next;
		}
	}
}

template<class Type>
void nTree<Type>::recursivePreOrderList(node<Type>* n, p2List<Type>* preorder_list) const
{
	// PreOrder: node - leftChild - rightChild

	if (n != NULL)
	{
		if (n != root)
			preorder_list->add(n->getValue());

		p2List_item<node<Type>*>* children_it = n->children.start;

		while (children_it != nullptr) {
			recursivePreOrderList(children_it->data, preorder_list);
			children_it = children_it->next;
		}
	}
}

#endif