#if !defined (NULL)
#define NULL 0
#endif
#if !defined (AVLTREE_H)
#define AVLTREE_H
#include "assert.h"
#include "Drawable.h"
#include "AVLTreeIterator.h"
#include "Line.h"
#include "AVL.h"
#include "Text.h"
using CSC2110::String;
#include <iostream>
using namespace std;
	/*
		This class represents a balanced binary search tree, where organization is determined by AVL tree rules.
		AVL tree rules are based on the height of the overall tree's subtreess. The algorithims are implemented in such a way to
		achieve most effiecent order notation. This class does not allow for duplicate items.	
	*/
template < class T >
class AVLTree : public Drawable
	{
	private:
		AVLTreeNode<T>* root;
		bool avlFlag;
		int sze;	
		int (*compare_items) (T* item_1, T* item_2);
		int (*compare_keys) (String* key, T* item);
		void setRootNode(AVLTreeNode<T>* tNode); //you can get by without using these methods, refer to root directly
		AVLTreeNode<T>* getRootNode();
		int getHeight(AVLTreeNode<T>* tNode);  //from lab
		bool isBalanced(AVLTreeNode<T>* tNode);
		void destroyItem(AVLTreeNode<T>* tNode);
		void destroy();
		AVLTreeNode<T>* insertItem(AVLTreeNode<T>* tNode, T* item);
		AVLTreeNode<T>* rotateLeft(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>* rotateRight(AVLTreeNode<T>* tNode);
		//write and use methods to do each of the four rotations (SR, SL, DLR, DRL)	
		AVLTreeNode<T>*singleRight(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>*singleLeft(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>*doubleLR(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>*doubleRL(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>* avlFixAddLeft(AVLTreeNode<T>* tNode);
		AVLTreeNode<T>* avlFixAddRight(AVLTreeNode<T>* tNode);
		virtual void drawRec(AVLTreeNode<T>* tNode, Cairo::RefPtr<Cairo::Context> cr, Line* line, int x_parent, int x_curr, int y_curr);
	public:
		AVLTree(int (*comp_items) (T* item_1, T* item_2), int (*comp_keys) (String* key, T* item));
		~AVLTree();
		int size();
		//pre: item represents the data to be inserted into the avl tree
		//	 items with same search keys are not allowed
		//post: if the preconditions are met, the item is inserted into the avl tree by avl tree rules
		void insert(T* item);
		//pre: sk represents the search key used to find an item
		//	each item has a unique a search key
		//post:if the preconditions are met, the item containing sk is returned
		T* retrieve(String* sk);
		bool isEmpty();
		T* getRootItem();
		AVLTreeIterator<T>* iterator();	
		//pre: none
		//post: the height of the overall tree is returned
		int getHeight();
		//pre: none
		//post: the overall balance of the tree is returned
		bool isBalanced();
		void draw(Cairo::RefPtr<Cairo::Context> cr, int width, int height);
		void mouseClicked(int x, int y);
	};
template < class T >
void AVLTree<T>::setRootNode(AVLTreeNode<T>* tNode)
	{
		root = tNode;
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::getRootNode()
	{
		return root;
	}
template < class T >
int AVLTree<T>::getHeight()
	{
		return getHeight(root);
	}
template < class T >
int AVLTree<T>::getHeight(AVLTreeNode<T>* tNode)
	{
	   int result_1 = 0;
	   int result_2 = 0;
	   if(tNode == NULL)
		{
			return 0;
		}
	   if(tNode->getLeft() != NULL)
		{
			result_1 = getHeight(tNode->getLeft());
		}
	   if(tNode->getRight() != NULL)
		{
			result_2 = getHeight(tNode->getRight());
		}
	   if(result_1 >= result_2)
		{
			return result_1 + 1;
		}
	   else
		{
			return result_2 + 1;
		}
	}
template < class T >
bool AVLTree<T>::isBalanced()
	{
		return isBalanced(root);
	}
template < class T >
bool AVLTree<T>::isBalanced(AVLTreeNode<T>* tNode)
	{
		bool result_1;
		bool result_2;
		int left_height;
		int right_height;
		if(tNode == NULL)
		{
			return true;
		}
		result_1 = isBalanced(tNode->getLeft());
		result_2 = isBalanced(tNode->getRight());
		if(result_1 == false || result_2 == false) return false;
		left_height = getHeight(tNode->getLeft());
		right_height = getHeight(tNode->getRight());
		if(abs(left_height - right_height) < 2) return true;
		return false;
	}
template < class T >
void AVLTree<T>::destroyItem(AVLTreeNode<T>* tNode)
	{
		if (tNode != NULL)
		{
			destroyItem(tNode->getLeft());
			destroyItem(tNode->getRight());
			delete tNode;
		}
	}
template < class T >
void AVLTree<T>::destroy()
	{
		destroyItem(root);
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::insertItem(AVLTreeNode<T>* tNode, T* item)
	{
		AVLTreeNode<T>* subtree;
		if (tNode == NULL)
		{
		  sze++;
		  avlFlag = true;
		  return new AVLTreeNode<T>(item);
		}
		T* node_items = tNode->getItem();
		int comp = (*compare_items) (item, node_items);
		if (comp == 0)
		{
		  return tNode;
		}
		else if (comp < 0)
		{
			subtree = insertItem(tNode->getLeft(), item);
			tNode->setLeft(subtree);
			if (avlFlag == true)
			{
				tNode->insertLeft();
			if (tNode->getBalanceFactor() == BALANCED)
				{
					avlFlag = false;
				}
				else
				{
					if(tNode->getBalanceFactor() == LEFT_UNBALANCED)
					{
						tNode = avlFixAddLeft(tNode);
						avlFlag = false;
					}
				}
			}
		}
		else
		{
		  subtree = insertItem(tNode->getRight(), item);
		  tNode->setRight(subtree);
			  if (avlFlag == true)
			  {
				tNode->insertRight();
				if (tNode->getBalanceFactor() == BALANCED)
				{
					avlFlag = false;
				}
				else
				{
					if(tNode->getBalanceFactor() == RIGHT_UNBALANCED)
					{
						tNode = avlFixAddRight(tNode);
						avlFlag = false;
					}
				}
			 }
		}
		return tNode;
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::rotateLeft(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>*right = tNode->getRight();
		AVLTreeNode<T>*right_left = right->getLeft();
		tNode->setRight(right_left);
		right->setLeft(tNode);
		return right;
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::rotateRight(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>*left = tNode->getLeft();
		AVLTreeNode<T>*left_right = left->getRight();
		tNode->setLeft(left_right);
		left->setRight(tNode);
		return left;
	}
template < class T >
AVLTreeNode<T>*AVLTree<T>::singleRight(AVLTreeNode<T>* tNode)
	{
		return rotateRight(tNode);
	}
template < class T >
AVLTreeNode<T>*AVLTree<T>::singleLeft(AVLTreeNode<T>* tNode)
	{
		return rotateLeft(tNode);
	}
template < class T >
AVLTreeNode<T>*AVLTree<T>::doubleLR(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>*subtree = tNode->getLeft();
		subtree = singleLeft(subtree);
		tNode->setLeft(subtree);
		tNode = singleRight(tNode);
		return tNode;
	}
template < class T >
AVLTreeNode<T>*AVLTree<T>::doubleRL(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>*subtree = tNode->getRight();
		subtree = singleRight(subtree);
		tNode->setRight(subtree);
		tNode = singleLeft(tNode);
		return tNode;
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::avlFixAddLeft(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>* left = tNode->getLeft();
		AVLTreeNode<T>* lefts_right = left->getRight();
		if(left->getBalanceFactor()==RIGHT_HEAVY)
		{
			if(lefts_right->getBalanceFactor()== BALANCED )
			{
				tNode->setBalanceFactor(BALANCED);
				left->setBalanceFactor(BALANCED);
				lefts_right->setBalanceFactor(BALANCED);
				return doubleLR(tNode);
			}
			else if(lefts_right->getBalanceFactor()== LEFT_HEAVY)
			{
				tNode->setBalanceFactor(RIGHT_HEAVY);
				left->setBalanceFactor(BALANCED);
				lefts_right->setBalanceFactor(BALANCED);
				return doubleLR(tNode);
			}
			else
			{
				tNode->setBalanceFactor(BALANCED);
				left->setBalanceFactor(LEFT_HEAVY);
				lefts_right->setBalanceFactor(BALANCED);
				return doubleLR(tNode);
			}
		}
		else
		{
				tNode->setBalanceFactor(BALANCED);
				left->setBalanceFactor(BALANCED);
				return singleRight(tNode);
		}
	}
template < class T >
AVLTreeNode<T>* AVLTree<T>::avlFixAddRight(AVLTreeNode<T>* tNode)
	{
		AVLTreeNode<T>* right = tNode->getRight();
		AVLTreeNode<T>* rights_left = right->getLeft();
		if(right->getBalanceFactor()==LEFT_HEAVY)
		{
			if(rights_left->getBalanceFactor()== BALANCED)
			{
				tNode->setBalanceFactor(BALANCED);
				right->setBalanceFactor(BALANCED);
				rights_left->setBalanceFactor(BALANCED);
				return doubleRL(tNode);
			}
			else if(rights_left->getBalanceFactor()== LEFT_HEAVY)
			{
				tNode->setBalanceFactor(BALANCED);
				right->setBalanceFactor(RIGHT_HEAVY);
				rights_left->setBalanceFactor(BALANCED);
				return doubleRL(tNode);
			}
			else
			{
				tNode->setBalanceFactor(LEFT_HEAVY);
				right->setBalanceFactor(BALANCED);
				rights_left->setBalanceFactor(BALANCED);
				return doubleRL(tNode);
			}
		}
		else
		{
				tNode->setBalanceFactor(BALANCED);
				right->setBalanceFactor(BALANCED);
				return singleLeft(tNode);
		}
	}
template < class T >
AVLTree<T>::AVLTree(int (*comp_items) (T* item_1, T* item_2), int (*comp_keys) (String* key, T* item))
	{
		root = NULL;
		sze = 0;
		compare_items = comp_items;
		compare_keys = comp_keys;
	}
template < class T >
AVLTree<T>::~AVLTree()
	{
		destroy();
	}
template < class T >
int AVLTree<T>::size()
	{
		return sze;
	}
template < class T >
void AVLTree<T>::insert(T* item)
	{
		root = insertItem(root,item);
	}
template < class T >
T* AVLTree<T>::retrieve(String* sk)
	{
		AVLTreeNode<T>* tNode = root;
		while (tNode != NULL)
		{
			T* node_items = tNode->getItem();
			int comp = (*compare_keys) (sk, node_items);
			if (comp == 0)
			{
				return node_items;
			}
			else if (comp < 0)
			{
				tNode = tNode->getLeft();
			}
			else
			{
				tNode = tNode->getRight();
			}
		}
		return NULL;
	}
template < class T >
bool AVLTree<T>::isEmpty()
	{
		return sze == 0;
	}
template < class T >
T* AVLTree<T>::getRootItem()
	{
		T* rootItem = root->getItem();
		return rootItem;
	}
template < class T >
AVLTreeIterator<T>* AVLTree<T>::iterator()
	{
		return new AVLTreeIterator<T>(root);
	}
	//the below GUI-related methods have been completed for you
template < class T >
void AVLTree<T>::draw(Cairo::RefPtr<Cairo::Context> cr, int width, int )
	{
		Line line(new Color(0, 0, 0), 5.0);
		drawRec(getRootNode(), cr, &line, width, width/2, 20);
	}
template < class T >
void AVLTree<T>::drawRec(AVLTreeNode<T>* tNode, Cairo::RefPtr<Cairo::Context> cr, Line* line, int x_parent, int x_curr, int y_curr)
	{
		//traversal to draw the entire binary tree
		if (tNode != NULL)
		{
		  //computing the location of the current node's two children
		  //the distance between a node's two children is the same as the horizontal distance between
		  //the current node and the current node's parent
		  //need to do this first as the drawing is from the bottom to the top
		  int separation = abs(x_curr - x_parent);
		  //need to make sure that the children are symmetrically placed with respect to the parent
		  int x_left = x_curr - separation/2;  //the location of the left child
		  int x_right = x_left + separation;   //the location of the right child
		  //compute the vertical location of the current node's parent
		  //and the current node's two children
		  int y_parent = y_curr - 50;
		  int y_children = y_curr + 50;
		  //draw the line connecting the current node to its parent
		  if (tNode != root)
		  {
			 line->draw(cr, x_curr, y_curr, x_parent, y_parent);
		  }
		  //draw the children
		  drawRec(tNode->getLeft(), cr, line, x_curr, x_left, y_children);
		  drawRec(tNode->getRight(), cr, line, x_curr, x_right, y_children);
		  //draw tNode
		  tNode->draw(cr, x_curr, y_curr);
		}
	}
template < class T >
void AVLTree<T>::mouseClicked(int , int ) {}
	#endif