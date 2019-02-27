#ifndef __RB_TREE_
#define __RB_TREE_

#include "iterator.h"
#include "Allocator.h"
#include "Function.h"
#include "utility.h"
namespace TinyStl {
	typedef enum rb_tree_color { black, red } colortype;

	template<class T>
	struct rb_tree_node {
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const pointer const_pointer;
		typedef const T& const_reference;
		typedef rb_tree_node<T>* node_ptr;
		T value;
		colortype color;
		node_ptr left;
		node_ptr right;
		node_ptr parent;
		rb_tree_node(T v, colortype c = red, node_ptr l = nullptr, node_ptr r = nullptr, node_ptr p = nullptr) :value(v), color(c), left(l), right(r), parent(p) {}
		node_ptr& operator=(const node_ptr& x) { value = x->value; left = x->left; right = x->right; parent = x->parent; }
		bool operator==(const node_ptr& n) { return color == n->color&&left == n->left&&right == n->right&&parent == n->parent&&value == n->value; }
		bool operator!=(const node_ptr& n) { return !(*this == n); }
		static node_ptr&  min(node_ptr x);
		static node_ptr& max(node_ptr y);
	};

	template<class T>
	struct rb_tree_iterator {
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const pointer const_pointer;
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef rb_tree_iterator<T> iterator;
		typedef rb_tree_iterator<const T> const_iterator;
		typedef typename rb_tree_node<T>* nodePtr;
		typedef typename rb_tree_node<const T>* const_nodePtr;

		nodePtr node;

	public:
		rb_tree_iterator() {}
		rb_tree_iterator(nodePtr n) { node = n; } 
		rb_tree_iterator(const iterator& it) { node = it.node; }
		
		reference operator*() const { return node->value; }
		pointer operator->() const { return &(operator*()); }
		iterator& operator++();
		const_iterator& operator++() const;
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		bool operator==(const iterator& it) { return node == it.node; }
		bool operator!=(const iterator& it) { return !(*this == it); }
	private:
		void increment();
		void decrement();
	};

	template<class Key,class Value, class KeyofValue=key_of_value<Value>, class Compare = less<Value>, class Alloc=allocator<rb_tree_node<Value>>> //该K值对以后做map有用
	class rb_tree {
	public:
		typedef Key key_value;
		typedef Value value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const pointer const_pointer;
		typedef const reference const_reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Alloc node_allocator;

		typedef typename rb_tree_node<const value_type> const_node;
		typedef typename rb_tree_node<value_type>* nodePtr;
		typedef typename rb_tree_node<const value_type>* const_nodePtr;
		typedef rb_tree_iterator<value_type> iterator;
		typedef rb_tree_iterator<const value_type> const_iterator;
	protected:
		size_type node_num;
		nodePtr header;
		Compare key_comp;
	protected:
		nodePtr create_node(const value_type& x);
		void destroy_node(nodePtr p); 
		void initialize();
		nodePtr clone_node(nodePtr x);
		nodePtr __copy(nodePtr x, nodePtr y);
		iterator __insert(nodePtr x, nodePtr y, const value_type& v);
		static nodePtr& left(nodePtr n) { return n->left; }
		static nodePtr& right(nodePtr n) { return n->right; }
		static nodePtr& parent(nodePtr n) { return n->parent; }
		static reference value(nodePtr n) { return n->value; }
		static const Key& key(nodePtr n) { return KeyofValue()(value(n)); }
		nodePtr & root() { return header->parent; }  //需要进行类型转换，header->parent仍然是rbtree_node_base*类型（和继承下来有关？）
		nodePtr& leftmost() { return header->left; } //指向左侧最小的元素
		nodePtr& rightmost() { return header->right; } //指向右侧最小的元素
		static typename colortype& color(nodePtr n) { return n->color; }
		void rb_tree_balance(nodePtr x,nodePtr& root);
		void rbtree_delete_fix(nodePtr p, nodePtr x, nodePtr& root);
		void __erase(nodePtr x);
		void left_rotate(nodePtr x, nodePtr& root);
		void right_rotate(nodePtr x, nodePtr& root);

	public:
		rb_tree() { initialize(); }
		rb_tree(const rb_tree& x);
		~rb_tree();
		iterator begin() { return leftmost(); }
		const_iterator begin() const;
		iterator end() { return header; }
		const_iterator end() const;
		bool empty() { return node_num==0; }
		size_type size() { return node_num; }
		iterator insert_equal(const value_type& v);
		pair<iterator,bool> insert_unique(const value_type& v);
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last);
		void erase(iterator it);
		iterator find(const value_type& x) ;
		void clear();
		static const_iterator change_iterator_to_const(iterator it);
		static const_iterator change_iterator_to_const(nodePtr n);

	};
}
#include "RBtree.impl.h"
#endif
