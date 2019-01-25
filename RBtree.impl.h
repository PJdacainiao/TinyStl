#ifndef __RBTREE_IMPL_H_
#define __RBTREE_IMPL_H_

namespace TinyStl {
	template<class T>
	typename rb_tree_node<T>::node_ptr& rb_tree_node<T>::rb_tree_node::min(node_ptr x) {
		while (x->left != nullptr) {
			x = x->left;
		}
		return x;
	}
	template<class T>
	typename rb_tree_node<T>::node_ptr& rb_tree_node<T>::rb_tree_node::max(node_ptr x) {
		while (x->right != nullptr)
			x = x->right;
		return x;
	}
	template<class T>
	void rb_tree_iterator<T>::increment() {
		if (node->right != nullptr) {       //在右子树中查找
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else {
			auto x = node->parent;  //无右子树，向上查找，找到第一个节点是父节点左儿子的节点，该父亲节点即为新的node
			while (x->right == node) {
				node = x;
				x = x->parent;
			}
			if (node->right != x)    //如果是root节点，++节点到header,否则的话++节点到满足条件的父节点
				node = x;
		}
	}
	template<class T>
	void rb_tree_iterator<T>::decrement(){
		if (node->color == red && node->parent->parent == node) //--head就等于右子节点
			node = node->right;
		else if (node->left != nullptr) {       //在左子树中查找
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}
		else {
			auto x = node->parent;  //无左子树，向上查找，找到第一个节点是父节点右儿子的节点，该父亲节点即为新的node
			while (x->left == node) {
				node = x;
				x = x->parent;
			}
			node = x;
		}
	}
	template<class T>
	typename rb_tree_iterator<T>::iterator& rb_tree_iterator<T>::operator++() {
		increment();
		return *this;
	}
	template<class T>
	typename rb_tree_iterator<T>::const_iterator& rb_tree_iterator<T>::operator++() const {
		increment();
		return *this;
	}
	template<class T>
	typename rb_tree_iterator<T>::iterator rb_tree_iterator<T>::operator++(int) {
		iterator tmp = *this;
		++*this;
		return tmp;
	}
	template<class T>
	typename rb_tree_iterator<T>::iterator& rb_tree_iterator<T>::operator--() {
		decrement();
		return *this;
	}
	template<class T>
	typename rb_tree_iterator<T>::iterator rb_tree_iterator<T>::operator--(int) {
		iterator tmp = *this;
		--*this;
		return tmp;
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyofValue, Compare, Alloc>::rb_tree(const rb_tree& x) {
		if(x.header->parent==nullptr)
			initialize();
		else {
			header = node_allocator::allocate();
			header->color = red;
			header->parent = __copy(x.header->parent, header);
			header->left = rb_tree_node<Value>::min(header->parent);
			header->right = rb_tree_node<Value>::max(header->parent);
		}
		node_num = x.node_num;
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyofValue, Compare, Alloc>::~rb_tree() {
		clear();
		node_allocator::deallocate(header);
	}
	template<class Key, class Value, class KeyofValue,  class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::nodePtr rb_tree<Key, Value, KeyofValue, Compare, Alloc>::create_node(const value_type& x) {
		nodePtr tmp= node_allocator::allocate();
		node_allocator::construct(tmp,rb_tree_node<Value>(x));
		return tmp;
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::destroy_node(nodePtr p) {
		node_allocator::destroy(p);
		node_allocator::deallocate(p);
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::initialize() {
		header =node_allocator::allocate();
		color(header) = red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::rb_tree_balance(nodePtr x, nodePtr& root) {
		while (x != root && x->parent->color == red) { //父节点为红色需要特别处理，为黑色的话无需处理
			auto p = x->parent;
			auto g = p->parent;
			auto gg = g->parent;
			if (p == g->left) {  //LL和LR情况
				auto u = g->right;
				if (u&&u->color == red) {   //叔叔节点与父亲节点同为红色，与祖父节点互换，
					p->color = black;
					u->color = black;
					g->color = red;
					x = g; //可能曾祖父与祖父同为红色，回溯x到grandpa位置，因为后续的旋转会改变g和gg的值，回溯可以使g和gg的值回到正常
				}
				else {  
					if (x == p->right)
						left_rotate(p, root);  
					p->color = black;
					g->color = red;
					right_rotate(g, root);
				}
			}
			else {  //RR,RL情况
				auto u = g->left;
				if (u&&u->color == red) {
					p->color = black;
					u->color = black;
					g->color = red;
					x = g;
				}
				else {
					if (x == p->left)
						right_rotate(p, root);
					g->color = red;
					p->color = black;
					left_rotate(g, root);
				}
			}
		}
		root->color = black;
	}
	
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_equal(const value_type& v) {
		nodePtr y = header;
		nodePtr x = root(); //header->parent就是root
		while (x != nullptr) {
			y = x;
			x = key_comp(KeyofValue()(v), key(y)) ? left(y) : right(y);
		}
		return __insert(x, y, v);
	}

	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	pair<typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator,bool> rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_unique(const value_type& v) {
		nodePtr y = header;
		nodePtr x = root();
		bool comp=true; //保证插入根节点时左插
		while (x != nullptr) {
			y = x;
			comp = key_comp(KeyofValue()(v), key(y));
			x = comp ? left(y): right(y);
		}
		iterator j = iterator(y);
		if (comp) {//comp返回的仅仅是最后一次的比较
			if (j == leftmost()) //若一直走到最小点处，则在沿途中一直是小于比较，没有等于的情况
				return pair<iterator, bool>(__insert(x, y, v), true);
			else  //如果最后插入节点是父亲的左儿子，父亲是祖父节点的右儿子，孙子节点有可能等于祖父节点，回溯到祖父节点比较
				--j;
		}
		if (key_comp(key(nodePtr(j.node)), KeyofValue()(v))) {//回溯到祖父
			return pair<iterator, bool>(__insert(x, y, v), true);
		}
		else
			return pair<iterator, bool>(j, false);
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	template<class InputIterator>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert_unique(*first);
	}
	
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyofValue, Compare, Alloc>::__insert(nodePtr x, nodePtr y, const value_type& v) {
		nodePtr z;
		if (y == header || x !=nullptr || key_comp(KeyofValue()(v), key(y))) { //1.当仅有头结点存在时，左插 2.x   3.当插入键值v小于带插入节点父节点的值时，左插
			z = create_node(v);
			left(y) = z; 
			if (y == header) {
				root() = z;
				rightmost() = z;        //第一次插入根节点时，更新leftmost和rightmost
			}
			else if (y == leftmost())   //如果y是最小的节点，则更新z为最小节点
				leftmost() = z;
		}
		else {
			z = create_node(v);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;        //更新z为最大节点
		}
		parent(z) = y;
		left(z) = nullptr;
		right(z) = nullptr;
		++node_num;
		rb_tree_balance(z, header->parent);
		return iterator(z);
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyofValue, Compare, Alloc>::find(const value_type& x) {
		nodePtr index = root();
		while (index != nullptr) {
			if (key(index) < KeyofValue()(x))
				index = right(index);
			else if (key(index) > KeyofValue()(x))
				index = left(index);
			else
				return iterator(index);
		}
		return iterator();
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::clear() {
		if (node_num != 0) {
			nodePtr x = root();
			__erase(x);
			root() = 0;
			leftmost() = header;
			rightmost() = header;
			node_num = 0;
		}
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::erase(iterator it) {
		auto x = it.node; //x为要删除的节点
		nodePtr y=x;  //y指向实际要删除的节点（x有两个儿子的情形y!=x）
		nodePtr z;   //替代删除节点的下一节点
		if (left(x) == nullptr || right(x) == nullptr) //x有一个或没有儿子节点
			y = x;
		else {
			y = rb_tree_node<Value>::min(right(y)); //这里取左子树的最大点
		}
		//寻找替换删除节点y的子节点
		if (left(y) != nullptr) 
			z= left(y);
		else
			z = right(y); //该替换节点可能为空
		if(z!=nullptr) //空指针无parent指向
			parent(z) = parent(y);
		if (y == root()) //被删除指针为根需要更新根节点
			root() = z;
		else if (y == left(y->parent))
			left(y->parent) = z;
		else
			right(y->parent) = z;
		if (y != x) //如果x!=y，用y的值交换实际要删除的x的值
			value(x) = value(y);
		//更新迭代器的范围
		if (x == leftmost()) {
			if (right(x) != nullptr)
				leftmost() = rb_tree_node<Value>::min(right(x));
			else
				leftmost() = parent(x);
		}
		if (x == rightmost()) {
			if (left(x) != nullptr)
				rightmost() = rb_tree_node<Value>::max(left(x));
			else
				rightmost() = parent(x);
		}
		//调整
		if (color(y) == black)
			rbtree_delete_fix(parent(y), z, root());
		destroy_node(y);
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::rbtree_delete_fix(nodePtr p, nodePtr x, nodePtr& root) {
		nodePtr x_parent = p; //这里的x为替换掉删除节点的节点，x_parent为被删除节点的父节点，即删除后x的实际节点
		while (x != root && (x == nullptr || color(x) == black)) {
			if (x == left(x_parent)) { //替换被删节点的节点在左边
				nodePtr w = right(x_parent);//w为删除前的叔叔节点，删除后的兄弟节点
				//case1，单纯调整为case2
				if (w->color == red) { 
					color(w) = black;
					color(x_parent) = red;
					left_rotate(x_parent, root);
					w = right(x_parent);
				}
				//case2,这种情况，左右的黑节点一样，继续上溯修改
				if ((left(w) == nullptr || color(w->left) == black) && (right(w) == nullptr || color(w->right) == black)) {
					color(w) = red;
					x = x_parent;
					x_parent = parent(x_parent);
				}
				//case3，
				else {
					if (right(w) == nullptr || color(right(w)) == black) {
						if (left(w) != nullptr)
							color(left(w)) = black;
						color(w) = red;
						right_rotate(w, root);
						x = right(x_parent);
					}
					//case4
					color(w) = color(x_parent);
					color(x_parent) = black;
					if (right(w) != nullptr)
						color(right(w)) = black;
					left_rotate(x_parent, root);
					break;
				}
			}
			else { //镜像
				nodePtr w = left(x_parent);
				if (color(w) == red) {
					color(w) = black;
					color(x_parent) = red;
					right_rotate(x_parent, root);
					w = left(x_parent);
				}
				if (color(w->left) == black && color(w->right) == black) {
					color(w) = red;
					x = x_parent;
					x_parent = parent(x_parent);
				}
				else {
					if (color(left(w)) == black) {
						color(right(w)) = black;
						color(w) = red;
						left_rotate(w, root);
						x = left(x_parent);
					}
					color(w) = color(x_parent);
					color(x_parent) = black;
					if (left(w) != nullptr)
						color(left(w)) = black;
					right_rotate(x_parent, root);
					break;
				}
			}
			if (x != 0) color(x) = black;
		}
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::__erase(nodePtr x) {
		/*while (x != nullptr) {
			__erase(x->right);
			auto y = x->left;
			destroy_node(x);
			x = y;
		}*/
		if (x != nullptr) {
			__erase(x->right);
			__erase(x->left);
			destroy_node(x);
		}
	}
	//引入parent旋转变得复杂
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::left_rotate(nodePtr x, nodePtr& root) {
		auto y = x->right;  
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::right_rotate(nodePtr x, nodePtr& root) {
		auto y = x->left;
		x->left = y->right;
		if (y->right != nullptr)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root)  //判断root情况
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::const_iterator  rb_tree<Key, Value, KeyofValue, Compare, Alloc>::begin() const {
		return change_iterator_to_const(iterator(leftmost()));
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::const_iterator  rb_tree<Key, Value, KeyofValue, Compare, Alloc>::end() const {
		return change_iterator_to_const(iterator(header));
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::const_iterator  rb_tree<Key, Value, KeyofValue, Compare, Alloc>::change_iterator_to_const(iterator it) {
		const_node node(it.node->value, it.node->color, (const_nodePtr)(it.node->left), (const_nodePtr)(it.node->right), (const_nodePtr)(it.node->parent));
		return const_iterator(&node);
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::const_iterator  rb_tree<Key, Value, KeyofValue, Compare, Alloc>::change_iterator_to_const(nodePtr n) {
		const_node node(n->value, n->color, (const_nodePtr)(n->left), (const_nodePtr)(n->right), (const_nodePtr)(n->parent));
		return const_iterator(&node);
	}
	//对于后续复制一整个节点有用，复制好了数值与颜色，在整个节点的复制过程中需要重新设置左右儿子以及父亲指向
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::nodePtr rb_tree<Key, Value, KeyofValue, Compare, Alloc>::clone_node(nodePtr x) {
		nodePtr tmp = node_allocator::allocate(); 
		tmp->value = x->value;
		tmp->color = x->color;
		tmp->left =nullptr;
		tmp->right = nullptr;
		return tmp;
	}
	template<class Key, class Value, class KeyofValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyofValue, Compare, Alloc>::nodePtr rb_tree<Key, Value, KeyofValue, Compare, Alloc>::__copy(nodePtr x, nodePtr p) {
		nodePtr top = clone_node(x); //可以将top之后的单独写成一个递归，p=top;if(x==0) return;else y=allocate,y->parent=p; 递归左边和右边
		top->parent = p;
		if (x->right)
			top->right = __copy(right(x), top);
		p = top;
		x = left(x);
		while (x != nullptr) {
			nodePtr y = clone_node(x);
			p->left = y;
			y->parent = p;
			if (x->right)
				y->right = __copy(right(x), y);
			p = y;
			x = left(x);
		}
		return top;
	}
}

#endif
