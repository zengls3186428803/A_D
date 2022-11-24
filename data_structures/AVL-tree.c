#include<stdio.h>
#include<stdlib.h>

typedef long long ll;
typedef unsigned long long ull;
const int MAX_N = 1e5 + 10;


typedef int T_key;

struct Node {
	T_key key;
	int height;
	struct Node *left_son, *right_son;
};
typedef struct Node Node;
//通过函数指针，模拟面向对象
struct AVL_tree {
	Node *root;
	int (*h)(Node *root);
	void (*l_rotate)(Node **root);
	void (*r_rotate)(Node **root);
	void (*l_r_rotate)(Node **root);
	void (*r_l_rotate)(Node **root);
	void (*balance)(Node **root);
	Node *(*max_node)(Node *root);
	Node *(*min_node)(Node *root);


	void (*insert)(Node **root, Node *node);
	Node *(*query)(Node *root, T_key x);
	void (*delete)(Node **root, Node *node);
	void (*visualize)(Node *root, int width);


};
typedef struct AVL_tree AVL_tree;

T_key max(T_key a,T_key b) {
	return (a > b ? a : b);
}
//得到该树的高度。
int h(Node *root) {
	if(root != NULL) {
		return root->height;
	} else {
		return 0;
	}
}
//左旋，更新高度，更新根结点。
void l_rotate(Node **root) {
	Node *node = *root;
	Node *rson = node->right_son;
	Node *rlson = rson->left_son;
	node->right_son = rlson;
	rson->left_son = node;
	node->height = max(h(node->left_son), h(node->right_son)) + 1;
	rson->height = max(h(rson->left_son), h(node->right_son)) + 1;
	*root = rson;
}
//右旋，更新高度，更新根结点。
void r_rotate(Node **root) {
	Node *node =  *root;
	Node *lson = node->left_son;
	Node *lrson = lson->right_son;
	node->left_son = lrson;
	lson->right_son = node;
	node->height = max(h(node->left_son), h(node->right_son)) + 1;
	lson->height = max(h(lson->left_son), h(lson->right_son)) + 1;
	*root = lson;
}
//对当前结点的左子结点左旋，对当前结点右旋。
void l_r_rotate(Node **root) {
	Node *node = *root;
	l_rotate(&node->left_son);
	r_rotate(root);
}
//对当先结点的右子结点右旋，对当前结点左旋。
void r_l_rotate(Node **root) {
	Node *node = *root;
	r_rotate(&node->right_son);
	l_rotate(root);
}

//使当前结点平衡
void balance(Node **root) {
	Node *p = *root;
	if(p == NULL) return;
	if(h(p->left_son) - h(p->right_son) > 1) {
		if(h(p->left_son->left_son) > h(p->left_son->right_son)) {//LL
			r_rotate(root);
		} else {//LR
			l_r_rotate(root);
		}
	} else if(h(p->right_son) - h(p->left_son) > 1) {
		if(h(p->right_son->left_son) > h(p->right_son->right_son)) {//RL
			r_l_rotate(root);
		} else {//RR
			l_rotate(root);
		}
	}
}


//获取一个树的最大值结点
Node *max_node(Node *root) {
	if(root != NULL && root->right_son != NULL) {
		return max_node(root->right_son);
	} else if(root != NULL && root->right_son == NULL) {
		return root;
	} else {
		return NULL;
	}
}
//获取一个树的最小值结点
Node *min_node(Node *root) {
	if(root != NULL && root->left_son != NULL) {
		return min_node(root->left_son);
	} else if(root != NULL && root->left_son == NULL) {
		return root;
	} else {
		return NULL;
	}
}

//查询x,返回答案结点的地址，若没有查询到，则返回NULL。
Node *query(Node *root, T_key x) {
	if(root == NULL) return NULL;
	if(x < root->key) {
		return query(root->left_son, x);
	} else if(x > root->key) {
		return query(root->right_son, x);
	} else {
		return root;
	}
}

//插入一个节点，更新该树的高度（即层数），平衡root所对应的树。
//使用双重指针，就不需要返回根节了，所以返回值为void，参数node为待插入结点。
//采用递归，在插入操作完成进行逐级向上地 更新高度 和 平衡子树。
void insert(Node **root, Node *node) {
	Node *p = *root;
	if(*root == NULL) {
		*root = node;
	} else if(node->key < p->key) {//插入左子树
		insert(&p->left_son, node);	
	} else if(node->key > p->key) {//插入右子树
		insert(&p->right_son, node);
	} else {//插入了重复的值。
		printf("error:key:%d has existed, no permiss to enter same value.\n", node->key);
	}
	//更新当前节点的高度（层数）
	(*root)->height = max(h((*root)->left_son), h((*root)->right_son)) + 1;
	balance(root);
}

//删除一个结点，更新树的高度，平衡root所对应的树
//采用递归，在删除操作完成进行逐级向上地 更新高度 和 平衡子树。
void delete(Node **root, Node *node) {
	Node *p = *root;
	if(p == NULL || node == NULL) {//特判空树和空结点
		return ;
	}
	if(node->key < p->key) {//递归地在对应的左子树中删除node
		delete(&p->left_son, node);
	} else if(node->key > p->key) {//递归地在对应的右子树中删除node
		delete(&p->right_son, node);
	}else {
		if(node->left_son != NULL && node->right_son != NULL) {
			Node *tmp = min_node(node->right_son);
			node->key = tmp->key;
			delete(&node->right_son, tmp);
		} else if(node->left_son != NULL && node->right_son == NULL) {
			*root = node->left_son;
			free(node);
		} else if(node->left_son == NULL && node->right_son != NULL){
			*root = node->right_son;
			free(node);
		} else {
			*root = NULL;
			free(node);
		}
	}
	p = *root;
	if(p != NULL) {
		p->height = max(h(p->left_son), h(p->right_son)) + 1;
	}
	balance(root);
}

//连续打印num个ch
void print(char *ch, int num) {
	for(int i = 1; i <= num; ++i) {
		printf("%c",ch[0]);
	}
}
//可视化一颗二插树,width 初始应该为0。
void visualize(Node* root, int width){
	if(root != NULL) {
		int len = printf("<%d>", root->key);
		int flag = 0;
		if(root->left_son != NULL) {
			visualize(root->left_son, width + len);
			flag += 1;
		}
		if(root->right_son != NULL) {
			if(flag > 0) {
				printf("\n");
				print(" ", width + len);
			}
			visualize(root->right_son, width + len);
		}
	}
}

//模拟AVL_tree的构造函数，返回一个空的AVL-tree的指针（不是空指针）。
AVL_tree *C_AVL_tree() {
	AVL_tree *avl_tree = (AVL_tree *)malloc(sizeof(AVL_tree));
	avl_tree->root = NULL;
	avl_tree->h = h;
	avl_tree->l_rotate = l_rotate;
	avl_tree->r_rotate = r_rotate;
	avl_tree->l_r_rotate = l_r_rotate;
	avl_tree->r_l_rotate = r_l_rotate;
	avl_tree->balance = balance;
	avl_tree->max_node = max_node;
	avl_tree->min_node = min_node;
	avl_tree->insert = insert;
	avl_tree->query = query;
	avl_tree->delete = delete;
	avl_tree->visualize = visualize;
	return avl_tree;
}


int main() {
	int n;
	printf("please enter the number of nodes that you want insert:\n");
	scanf("%d", &n);
	AVL_tree *avl_tree = C_AVL_tree();
	while(n--) {
		int x;
		Node *p = (Node *)malloc(sizeof(Node));
		printf("please enter the key of node:");
		scanf("%d", &x);
		p->height = 0;
		p->key = x;
		p->left_son = NULL;
		p->right_son = NULL;
		avl_tree->insert(&avl_tree->root, p);
	}
	printf("insert operation has completed.the structure of the avl-tree is as follow.\n");
	avl_tree->visualize(avl_tree->root, 0);
	printf("\n=========================================================\n");
	printf("please enter the number of nodes that you want delete:");
	scanf("%d",&n);
	while(n--) {
		int x;
		scanf("%d", &x);
		printf("please enter the key of node:");
		avl_tree->delete(&avl_tree->root, avl_tree->query(avl_tree->root, x));
	}
	printf("delete operation has completed.the structure of the avl-tree is as follow.\n");
	avl_tree->visualize(avl_tree->root, 0);
}
