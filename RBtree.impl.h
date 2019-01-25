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
		if (node->right != nullptr) {       //���������в���
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else {
			auto x = node->parent;  //�������������ϲ��ң��ҵ���һ���ڵ��Ǹ��ڵ�����ӵĽڵ㣬�ø��׽ڵ㼴Ϊ�µ�node
			while (x->right == node) {
				node = x;
				x = x->parent;
			}
			if (node->right != x)    //�����root�ڵ㣬++�ڵ㵽header,����Ļ�++�ڵ㵽���������ĸ��ڵ�
				node = x;
		}
	}
	template<class T>
	void rb_tree_iterator<T>::decrement(){
		if (node->color == red && node->parent->parent == node) //--head�͵������ӽڵ�
			node = node->right;
		else if (node->left != nullptr) {       //���������в���
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}
		else {
			auto x = node->parent;  //�������������ϲ��ң��ҵ���һ���ڵ��Ǹ��ڵ��Ҷ��ӵĽڵ㣬�ø��׽ڵ㼴Ϊ�µ�node
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
		while (x != root && x->parent->color == red) { //���ڵ�Ϊ��ɫ��Ҫ�ر���Ϊ��ɫ�Ļ����账��
			auto p = x->parent;
			auto g = p->parent;
			auto gg = g->parent;
			if (p == g->left) {  //LL��LR���
				auto u = g->right;
				if (u&&u->color == red) {   //����ڵ��븸�׽ڵ�ͬΪ��ɫ�����游�ڵ㻥����
					p->color = black;
					u->color = black;
					g->color = red;
					x = g; //�������游���游ͬΪ��ɫ������x��grandpaλ�ã���Ϊ��������ת��ı�g��gg��ֵ�����ݿ���ʹg��gg��ֵ�ص�����
				}
				else {  
					if (x == p->right)
						left_rotate(p, root);  
					p->color = black;
					g->color = red;
					right_rotate(g, root);
				}
			}
			else {  //RR,RL���
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
		nodePtr x = root(); //header->parent����root
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
		bool comp=true; //��֤������ڵ�ʱ���
		while (x != nullptr) {
			y = x;
			comp = key_comp(KeyofValue()(v), key(y));
			x = comp ? left(y): right(y);
		}
		iterator j = iterator(y);
		if (comp) {//comp���صĽ��������һ�εıȽ�
			if (j == leftmost()) //��һֱ�ߵ���С�㴦��������;��һֱ��С�ڱȽϣ�û�е��ڵ����
				return pair<iterator, bool>(__insert(x, y, v), true);
			else  //���������ڵ��Ǹ��׵�����ӣ��������游�ڵ���Ҷ��ӣ����ӽڵ��п��ܵ����游�ڵ㣬���ݵ��游�ڵ�Ƚ�
				--j;
		}
		if (key_comp(key(nodePtr(j.node)), KeyofValue()(v))) {//���ݵ��游
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
		if (y == header || x !=nullptr || key_comp(KeyofValue()(v), key(y))) { //1.������ͷ������ʱ����� 2.x   3.�������ֵvС�ڴ�����ڵ㸸�ڵ��ֵʱ�����
			z = create_node(v);
			left(y) = z; 
			if (y == header) {
				root() = z;
				rightmost() = z;        //��һ�β�����ڵ�ʱ������leftmost��rightmost
			}
			else if (y == leftmost())   //���y����С�Ľڵ㣬�����zΪ��С�ڵ�
				leftmost() = z;
		}
		else {
			z = create_node(v);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;        //����zΪ���ڵ�
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
		auto x = it.node; //xΪҪɾ���Ľڵ�
		nodePtr y=x;  //yָ��ʵ��Ҫɾ���Ľڵ㣨x���������ӵ�����y!=x��
		nodePtr z;   //���ɾ���ڵ����һ�ڵ�
		if (left(x) == nullptr || right(x) == nullptr) //x��һ����û�ж��ӽڵ�
			y = x;
		else {
			y = rb_tree_node<Value>::min(right(y)); //����ȡ������������
		}
		//Ѱ���滻ɾ���ڵ�y���ӽڵ�
		if (left(y) != nullptr) 
			z= left(y);
		else
			z = right(y); //���滻�ڵ����Ϊ��
		if(z!=nullptr) //��ָ����parentָ��
			parent(z) = parent(y);
		if (y == root()) //��ɾ��ָ��Ϊ����Ҫ���¸��ڵ�
			root() = z;
		else if (y == left(y->parent))
			left(y->parent) = z;
		else
			right(y->parent) = z;
		if (y != x) //���x!=y����y��ֵ����ʵ��Ҫɾ����x��ֵ
			value(x) = value(y);
		//���µ������ķ�Χ
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
		//����
		if (color(y) == black)
			rbtree_delete_fix(parent(y), z, root());
		destroy_node(y);
	}
	template<class Key, class Value,class KeyofValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyofValue, Compare, Alloc>::rbtree_delete_fix(nodePtr p, nodePtr x, nodePtr& root) {
		nodePtr x_parent = p; //�����xΪ�滻��ɾ���ڵ�Ľڵ㣬x_parentΪ��ɾ���ڵ�ĸ��ڵ㣬��ɾ����x��ʵ�ʽڵ�
		while (x != root && (x == nullptr || color(x) == black)) {
			if (x == left(x_parent)) { //�滻��ɾ�ڵ�Ľڵ������
				nodePtr w = right(x_parent);//wΪɾ��ǰ������ڵ㣬ɾ������ֵܽڵ�
				//case1����������Ϊcase2
				if (w->color == red) { 
					color(w) = black;
					color(x_parent) = red;
					left_rotate(x_parent, root);
					w = right(x_parent);
				}
				//case2,������������ҵĺڽڵ�һ�������������޸�
				if ((left(w) == nullptr || color(w->left) == black) && (right(w) == nullptr || color(w->right) == black)) {
					color(w) = red;
					x = x_parent;
					x_parent = parent(x_parent);
				}
				//case3��
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
			else { //����
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
	//����parent��ת��ø���
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
		if (x == root)  //�ж�root���
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
	//���ں�������һ�����ڵ����ã����ƺ�����ֵ����ɫ���������ڵ�ĸ��ƹ�������Ҫ�����������Ҷ����Լ�����ָ��
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
		nodePtr top = clone_node(x); //���Խ�top֮��ĵ���д��һ���ݹ飬p=top;if(x==0) return;else y=allocate,y->parent=p; �ݹ���ߺ��ұ�
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
