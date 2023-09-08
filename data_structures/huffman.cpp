#include<cstdio>
#include<iostream>
#include<queue>
#include<vector>

typedef long long ll;
typedef unsigned long long ull;

using std::cin;
using std::cout;
using std::endl;

class Node {
public:
    double weight;
    char character;
    int next;
};

class Edge {
public:
    int next;
    int to;
};

class Graph {
public:
    static const int MAX_NODE = 1e4 + 10;
    static const int MAx_EDGE = 1e4 + 10;
    Node head[MAX_NODE];
    Edge edge[MAx_EDGE];
    int idx_h;
    int idx_e;
    int num_node;
public:
    Graph() {
        idx_h = 0;
        idx_e = 0;
        for (int i = 0; i < MAX_NODE; ++i) {
            head[i].next = 0;
            head[i].weight = 0;
            head[i].character = 0;
            num_node = MAX_NODE - 1;
        }
        for (int i = 0; i < MAx_EDGE; ++i) {
            edge[i].to = 0;
        }
    }

    virtual void add_edge(int from, int to) {
        idx_e += 1;
        edge[idx_e].to = to;
        edge[idx_e].next = head[from].next;
        head[from].next = idx_e;
    }

    virtual void add_bi_edge(int from, int to) {
        add_edge(from, to);
        add_edge(to, from);
    }
};

class Tree : public Graph {
public:
    int root;

    virtual void print(int root, int width) {
        for (int i = 1; i <= width; ++i) {
            printf(" ");
        }
        printf("\n");
        int dw = printf("{id:%d}", root);
        for (int i = head[root].next; i != 0; i = edge[i].to) {
            print(i, width + dw);
        }
    }
};


template<typename T>
class Greater {
public:
    bool operator()(T n1, T n2) {
        return n1.weight > n2.weight;
    }
};

using std::vector;
using std::priority_queue;


class Huffman : public Tree {
public:
    class Cmp_node {
    public:
        int id;
        double weight;
    } cmp_node[MAX_NODE];

    int idx_c;
    priority_queue<Cmp_node, vector<Cmp_node>, Greater<Cmp_node> > q;
public:
    void build() {
        for (int i = 1; i <= num_node; ++i) {
            cmp_node[i].weight = head[i].weight;
            cmp_node[i].id = i;
        }
        for (int i = 1; i <= num_node; ++i) {
            q.push(cmp_node[i]);
        }
        idx_c = num_node;
        if (q.empty()) return;
        while (q.size() > 1) {
            Cmp_node n1 = q.top();
            q.pop();
            Cmp_node n2 = q.top();
            q.pop();
            idx_c += 1;
            cmp_node[idx_c].weight = n1.weight + n2.weight;
            cmp_node[idx_c].id = idx_c;
            head[idx_c].weight = cmp_node[idx_c].weight;
            add_edge(idx_c, n1.id);
            add_edge(idx_c, n2.id);
            q.push(cmp_node[idx_c]);
        }
        Cmp_node n = q.top();
        q.pop();
        root = n.id;
    }

    void print(int root, int width) {
        int dw = printf("{id:%d,weight:%.0lf}", root, head[root].weight);
        width += dw;
        int flag = 0;
        for (int i = head[root].next; i != 0; i = edge[i].next) {
            if(flag == 1) {
                cout << endl;
                for (int i = 1; i <= width; ++i) {
                    printf(" ");
                }
            }
            print(edge[i].to, width);
            flag = 1;
        }
    }

    void print() {
        printf("huffman-tree:\n");
        print(root, 0);
        printf("\n");
    }
    double get_WPL(int root, int depth) {
        double ans = 0;
        int flag = 0;
        for(int i = head[root].next; i != 0; i = edge[i].next) {
            flag = 1;
            ans += get_WPL(edge[i].to, depth + 1);
        }
        if(flag == 0) {
            ans = head[root].weight * depth;
        }
        return ans;
    }
    void print_WPL() {
        printf("WPL of huffman-tree:%lf\n", get_WPL(root, 0));
    }
};

int main() {
    Huffman huffman;
    int n;
    cout << "please enter the number of nodes:" << endl;
    cin >> n;
    cout << "please enter weight of nodes:" << endl;
    double w;
    for (int i = 1; i <= n; ++i) {
        scanf("%lf", &w);
        huffman.head[i].weight = w;
    }
    huffman.num_node = n;
    huffman.build();
    huffman.print();
    huffman.print_WPL();
    return 0;
}
