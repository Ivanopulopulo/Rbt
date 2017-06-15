#include <iostream>

using namespace std;
template <class T>
class RBT {
	struct node_t
	{
		node_t *left, *right;
		T value;
		bool RED;
		void Show(ostream &out, int level) const;
	};
	node_t *tree_root;
	int nodes_count;
private:
	node_t* NewNode(T value)
	{
		nodes_count++;
		node_t *node = new node_t;
		node->value = value;
		node->left = nullptr;
		node->right = nullptr;
		node->RED = true;
		return node;
	}
	void DelNode(node_t*);
	void Clean(node_t*);
	node_t* Rotate21(node_t* node)
	{
		node_t *right = node->right;
		node_t *root_st = right->left;
		right->left = node;
		node->right = root_st;
		return right;
	}
	node_t* Rotate12(node_t* node)
	{
		node_t *left = node->left;
		node_t *root_st = left->right;
		left->right = node;
		node->left = root_st;
		return left;
	}
	void BInsert(node_t**);
	bool BRemove1(node_t**);
	bool BRemove2(node_t**);
	bool Insert(T&, node_t**);
	bool GetMin(node_t**, node_t**);
	bool Remove(node_t**, T&);
	bool Equal(node_t* root2, const node_t* root1)
	{
		return (root2&&root1 ? root2->value == root1->value&&Equal(root2->left, root1->left) && Equal(root2->right, root1->right) : !root2 && !root1);
	};

public:
	bool Empty()
	{
		node_t* root1 = nullptr;
		return Equal(tree_root, root1);
	}
	void Show(ostream &out) const;
	RBT(std::initializer_list<T> list)
	{
		tree_root = nullptr;
		for (auto& item : list)
		{
			Insert(item);
		}

	}
	RBT();
	~RBT();
	void Clean();
	void Insert(T);
	void Remove(T);
	int GetNodesCount() const;
	bool operator ==(const RBT<T> &a)
	{

		return Equal(tree_root, a.tree_root);
	}
};


template <class T>
RBT<T>::RBT()
{
	tree_root = nullptr;
	nodes_count = 0;
}
template <class T>
RBT<T>::~RBT()
{
	Clean(tree_root);
}
template <class T>
int RBT<T>::GetNodesCount() const
{
	return nodes_count;
}
template <class T>
void RBT<T>::DelNode(node_t *node)
{
	nodes_count--;
	delete node;
}
template <class T>
void RBT<T>::Clean(node_t *node)
{
	if (!node) return;
	Clean(node->left);
	Clean(node->right);
	DelNode(node);
}
template <class T>
void RBT<T>::node_t::Show(ostream &out, int level) const
{
	const node_t *tr = this;

	if (tr) tr->right->Show(out, level + 1);
	for (int i = 0; i<level; i++)
		out << "   ";

	if (tr)
	{
		char *is_RED = "BLACK";
		if (RED) is_RED = "RED";
		out << tr->value << "\n";
		for (int i = 0; i<level; i++)
			out << "   ";
		out << is_RED << endl;
	}
	else out << "End" << endl;
	if (tr) tr->left->Show(out, level + 1);
}
template <class T>
void RBT<T>::BInsert(node_t **root)
{
	node_t *left, *right, *lx1, *rx1;
	node_t *node = *root;
	if (node->RED) return;
	left = node->left;
	right = node->right;
	if (left && left->RED)
	{
		rx1 = left->right;
		if (rx1 && rx1->RED) left = node->left = Rotate21(left);
		lx1 = left->left;
		if (lx1 && lx1->RED) {
			node->RED = true;
			left->RED = false;
			if (right && right->RED) {
				lx1->RED = true;
				right->RED = false;
				return;
			}
			*root = Rotate12(node);
			return;
		}
	}

	if (right && right->RED) {
		lx1 = right->left;
		if (lx1 && lx1->RED) right = node->right = Rotate12(right);
		rx1 = right->right;
		if (rx1 &&rx1->RED) {
			node->RED = true;
			right->RED = false;
			if (left && left->RED) {
				rx1->RED = true;
				left->RED = false;
				return;
			}
			*root = Rotate21(node);
			return;
		}
	}
}
template <class T>
bool RBT<T>::BRemove1(node_t **root)
{
	node_t *node = *root;
	node_t *left = node->left;
	node_t *right = node->right;
	if (left && left->RED)
	{
		left->RED = false;
		return false;
	}
	if (right && right->RED)
	{
		node->RED = true;
		right->RED = false;
		node = *root = Rotate21(node);
		if (BRemove1(&node->left)) node->left->RED = false;
		return false;
	}

	unsigned int mask = 0;
	node_t *pa = right->left;
	node_t *pb = right->right;
	if (pa && pa->RED) mask |= 1;
	if (pb && pb->RED) mask |= 2;
	switch (mask)
	{
	case 0:
		right->RED = true;
		return true;
	case 1:
	case 3:
		right->RED = true;
		pa->RED = false;
		right = node->right = Rotate12(right);
		pb = right->right;
	case 2:
		right->RED = node->RED;
		pb->RED = node->RED = false;
		*root = Rotate21(node);
	}
	return false;
}
template <class T>
bool RBT<T>::BRemove2(node_t **root)
{
	node_t *node = *root;
	node_t *left = node->left;
	node_t *right = node->right;
	if (right && right->RED)
	{
		right->RED = false;
		return false;
	}
	if (left && left->RED) {
		node->RED = true;
		left->RED = false;
		node = *root = Rotate12(node);
		if (BRemove2(&node->right)) node->right->RED = false;
		return false;
	}
	unsigned int mask = 0;
	node_t *pa1 = left->left;
	node_t *pa2 = left->right;
	if (pa1 && pa1->RED) mask |= 1;
	if (pa2 && pa2->RED) mask |= 2;
	switch (mask) {
	case 0:
		left->RED = true;
		return true;
	case 2:
	case 3:
		left->RED = true;
		pa2->RED = false;
		left = node->left = Rotate21(left);
		pa1 = left->left;
	case 1:
		left->RED = node->RED;
		pa1->RED = node->RED = false;
		*root = Rotate12(node);
	}
	return false;
}
template <class T>
bool RBT<T>::Insert(T &value, node_t **root)
{
	node_t *node = *root;
	if (!node) *root = NewNode(value);
	else {
		if (value == node->value) return true;
		if (Insert(value, value<node->value ? &node->left : &node->right)) return true;
		BInsert(root);
	}
	return false;
}
template <class T>
bool RBT<T>::GetMin(node_t **root, node_t **res)
{
	node_t *node = *root;
	if (node->left) {
		if (GetMin(&node->left, res)) return BRemove1(root);
	}
	else {
		*root = node->right;
		*res = node;
		return !node->RED;
	}
	return false;
}
template <class T>
bool RBT<T>::Remove(node_t **root, T &value)
{
	node_t *t, *node = *root;
	if (!node) return false;
	if (node->value<value) {
		if (Remove(&node->right, value))
			return BRemove2(root);
	}
	else if (node->value>value) {
		if (Remove(&node->left, value))	return BRemove1(root);
	}
	else {
		bool res;
		if (!node->right) {
			*root = node->left;
			res = !node->RED;
		}
		else {
			res = GetMin(&node->right, root);
			t = *root;
			t->RED = node->RED;
			t->left = node->left;
			t->right = node->right;
			if (res) res = BRemove2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}
template <class T>
void RBT<T>::Show(ostream &out) const
{
	tree_root->Show(out, 0);
}
template <class T>
void RBT<T>::Insert(T value)
{
	Insert(value, &tree_root);
	if (tree_root) tree_root->RED = false;
}
template <class T>
void RBT<T>::Remove(T value)
{
	Remove(&tree_root, value);
}
template <class T>
void RBT<T>::Clean()
{
	Clean(tree_root);
	tree_root = nullptr;
}
