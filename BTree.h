#pragma once

template<class TKey, class TVal>
class BTreeNode
{
public:
	TKey key;
	TVal value;
	BTreeNode* left = nullptr;
	BTreeNode* right = nullptr;

	BTreeNode(TKey key, TVal val) : key(key), value(val) {}

	void print()
	{
		if (left != nullptr) left->print();
		//cout << value << " ";
		value.print();
		if (right != nullptr) right->print();
	}

	void save()
	{
		if (left != nullptr) left->save();

		value.save();

		if (right != nullptr) right->save();
	}



	void del()
	{
		if (left  != nullptr) left->del();
		if (right != nullptr) right->del();
		delete this;
	}

	TVal* getValue(const TKey& key)
	{
		if (key == this->key) return &value;
		if (key < this->key && left  != nullptr) return left->getValue(key);
		if (key > this->key && right != nullptr) return right->getValue(key);
		return nullptr;
	}
};


template<class TKey, class TVal>
class BTree
{
	BTreeNode<TKey, TVal>* root = nullptr;

	BTreeNode<TKey, TVal>* push_r(TKey key, TVal val, BTreeNode<TKey, TVal>*& node);

public:
	~BTree();
	bool push(TKey key, TVal val);
	void print();
	void save();
	bool isEmpty();
	TVal* getValue(const TKey& key);
	void clear();
	BTreeNode<TKey, TVal>* rpush(TKey key, TVal val);




};

template<class TKey, class TVal>
BTree<TKey, TVal>::~BTree()
{
	this->clear();
}

template<class TKey, class TVal>
bool BTree<TKey, TVal>::push(TKey key, TVal val)
{
	if (!root)
	{
		root = new BTreeNode<TKey, TVal>(key, val);
		return true;
	}
	BTreeNode<TKey, TVal>* next = root;
	do
	{
		if (key < next->key)
		{
			if (next->left)
			{
				next = next->left;
			}
			else
			{
				next->left = new BTreeNode<TKey, TVal>(key, val);
				return true;
			}
		}
		else if (key > next->key)
		{
			if (next->right)
			{
				next = next->right;
			}
			else
			{
				next->right = new BTreeNode<TKey, TVal>(key, val);
				return true;
			}
		}
		else
		{
			return false;
		}
	} while (true);
}

template<class TKey, class TVal>
void BTree<TKey, TVal>::print()
{
	if (root) root->print();
}

template<class TKey, class TVal>
void BTree<TKey, TVal>::save()
{
	if (root) root->save();
}

template<class TKey, class TVal>
bool BTree<TKey, TVal>::isEmpty()
{
	return root == nullptr;
}

template<class TKey, class TVal>
TVal* BTree<TKey, TVal>::getValue(const TKey& key)
{
	if (root)
		return root->getValue(key);
	return nullptr;
}

template<class TKey, class TVal>
void BTree<TKey, TVal>::clear()
{
	if (root) root->del();
	root = nullptr;
}

template<class TKey, class TVal>
BTreeNode<TKey, TVal>* BTree<TKey, TVal>::rpush(TKey key, TVal val)
{
	return push_r(key, val, root);
}


template<class TKey, class TVal>
BTreeNode<TKey, TVal>* BTree<TKey, TVal>::push_r(TKey key, TVal val, BTreeNode<TKey, TVal>*& node)
{
	if (!node)
	{
		node = new BTreeNode<TKey, TVal>(key, val);
		return node;
	}
	if (key < node->key) node->left  = push_r(key, val, node->left);
	if (key > node->key) node->right = push_r(key, val, node->right);
	return node;
}
