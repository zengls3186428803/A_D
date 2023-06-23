#include<cstdio>
#include<iostream>
#include<queue>

#define MAX_N 100010
typedef long long ll;
typedef unsigned long long ull;


struct Node {
	double weight;
	int id;//id == 0 represent this is a interior node, else left node;
	bool operator<(const Node &b) const {
		return this->weight > b.weight;
	}
};
//typedef struct Node Node;
Node node[MAX_N];
int idx_Node = 0;

struct Edge {
	int to;
};
Edge edge[MAX_N];
int idx_Edge = 0;

using namespace std;
priority_queue<Node> q;
vector<Edge> v[MAX_N];

void add_edge(int from, int to) {
	edge[++idx_Edge].to = to;
	v[from].push_back(edge[idx_Edge]);
}
void insert(int root, int lson, int rson) {
	node[root].weight = node[lson].weight + node[rson].weight;
	if(node[lson].weight < node[rson].weight) {
		add_edge(root, lson);
		add_edge(root, rson);
	} else {
		add_edge(root, rson);
		add_edge(root, lson);
	}
}

void print(int root, int width=0) {
	for(int i = 1; i <= width; ++i) {
		printf(" ");
	}
	int delta = printf("<nid:%04d>", root, node[root].weight);
	printf("\n");
	for(int i = 0; i < v[root].size(); ++i) {
		print(v[root][i].to, width + delta);
	}
	return;
}

double get_wpl(int root, int deep = 0) {
	if(v[root].size() == 0) {
		return node[root].weight * deep;
	}
	double ans = 0;
	for(int i = 0; i < v[root].size(); ++i) {
		ans += get_wpl(v[root][i].to, deep+1);
	}
	return ans;
}

ll cnt[MAX_N];
double freq[MAX_N];
int main() {
	int n;
	cin >> n;
	ll sum = 0;
	Node a;
	for(int i = 0; i < MAX_N; ++i) {
		node[i].id = i;
	}
	for(int i = 1; i <= n; ++i) {
		scanf("%lld", cnt+i);
		sum += cnt[i];
	}
	for(int i = 1; i <= n; ++i) {
		freq[i] = (double)cnt[i] / sum;
	}
	for(int i = 1; i <= n; ++i) {
		node[++idx_Node].id = i;
		node[idx_Node].weight = freq[i];
		q.push(node[idx_Node]);
	}
	while(q.size() > 1) {
		Node x = q.top();
		q.pop();
		Node y = q.top();
		q.pop();
		++idx_Node;
		insert(idx_Node, x.id, y.id);
		q.push(node[idx_Node]);
	}
	Node root = q.top();
	q.pop();
	printf("root.id=%d, root.weight=%lf\n",root.id, root.weight);
	printf("打印huffman树，nid 为结点编号\n"); 
	print(root.id);
	printf("WPL = %lf", get_wpl(root.id) * sum);
}
