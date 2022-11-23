#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

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

struct AVL_tree {
	Node *root;
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
		if(h(p->left_son->left_son) > h(p->left_son->right_son)) {
			r_rotate(root);
		} else {
			l_r_rotate(root);
		}
	} else if(h(p->right_son) - h(p->left_son) > 1) {
		if(h(p->right_son->left_son) > h(p->right_son->right_son)) {
			r_l_rotate(root);
		} else {
			l_rotate(root);
		}
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

Node *max_node(Node *root) {
	if(root != NULL && root->right_son != NULL) {
		return max_node(root->right_son);
	} else if(root != NULL && root->right_son == NULL) {
		return root;
	} else {
		return NULL;
	}
}
Node *min_node(Node *root) {
	if(root != NULL && root->left_son != NULL) {
		return min_node(root->left_son);
	} else if(root != NULL && root->left_son == NULL) {
		return root;
	} else {
		return NULL;
	}
}



//插入一个节点，平衡以该节点为根的树，更新该树的根，更新该树的高度（即层数）。
//使用双重指针，就不需要返回根节了，所以返回值为void，参数node为待插入结点。
void insert(Node **root, Node *node) {
	Node *p = *root;
	if(*root == NULL) {
		*root = node;
	} else if(node->key < p->key) {//插入左子树
		insert(&p->left_son, node);
		if(h(p->left_son) - h(p->right_son) > 1) {//检查平衡因子是否超过1
			if(node->key < p->left_son->key) {//对应于LL的情况，所以右旋。
				r_rotate(root);
			} else {//对应于LR的情况，所以先左旋，再右旋。
				l_r_rotate(root);
			}
		}
	} else if(node->key > p->key) {//插入右子树
		insert(&p->right_son, node);
		if(h(p->right_son) - h(p->left_son) > 1) {//检查平衡因子是否超过1
			if(node->key > p->right_son->key) {//对应于RR的情况，所以左旋。
				l_rotate(root);
			} else {//对应于RL的情况，所以先右旋，再左旋。
				r_l_rotate(root);
			}
		}

	} else {//插入了重复的值。
		printf("error:key:%d has existed, no permiss to enter same value.\n", node->key);
	}
	//更新当前节点的高度（层数）
	(*root)->height = max(h((*root)->left_son), h((*root)->right_son)) + 1;
}

void delete(Node **root, Node *node) {
	Node *p = *root;
	if(p == NULL || node == NULL) {
		return ;
	}
	if(node->key < p->key) {
		delete(&p->left_son, node);
	} else if(node->key > p->key) {
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


void print(char *ch, int num) {
	for(int i = 1; i <= num; ++i) {
		printf("%c",ch[0]);
	}
}
//可视化一颗二插树
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


int main() {
	int n;
	scanf("%d", &n);
	Node *root = NULL;
	while(n--) {
		int x;
		Node *p = (Node *)malloc(sizeof(Node));
		scanf("%d", &x);
		p->height = 0;
		p->key = x;
		p->left_son = NULL;
		p->right_son = NULL;
		insert(&root, p);
	}
	visualize(root, 0);
	scanf("%d",&n);
	while(n--) {
		int x;
		scanf("%d", &x);
		delete(&root, query(root, x));
	}
	printf("\n");
	visualize(root, 0);
}
