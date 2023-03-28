#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<iostream>
#include<map>
#include<queue>
#include<vector>
#include<set>

typedef long long ll;
typedef unsigned long long ull;
const int INF = 0x3f3f3f3f;
const long long INF_L = 0x3f3f3f3f3f3f3f3f;
using namespace std;
const int MAX_N = 1e5+10;

ull p = 131;//base
ull b[MAX_N];//ull natural overflow
void pre_b() {
	b[0] = 1;
	for(int i = 1; i <= MAX_N - 1; ++i) {
		b[i] = p * b[i - 1];
	}
}

ull a[MAX_N];
struct Tree {
#define lson (node<<1)
#define rson ((node<<1)|1)
	ull s[4 * MAX_N];
	ull lazy[4 * MAX_N];
	void pushup(int node, int l, int r) {
		int mid = l + ((r-l)>>1);
		s[node] = b[(r - l + 1) - (mid - l + 1)] * s[lson] + s[rson];//hash[n] = (b)^(n-m) * hash[1...m] + hash[m+1...n];
	}
	void build(int node, int l, int r) {
		if(l == r) {
			s[node] = a[l];
			return;
		}
		int mid = l + ((r-l)>>1);
		build(lson, l, mid);
		build(rson, mid + 1, r);
		pushup(node, l ,r);
	}
	void modify(int node, int l, int r, int index,  ull val) {
		if(l == r) {
			s[node] = val;
			return ;
		}
		int mid = l + ((r-l)>>1);
		if(index <= mid) {
			modify(lson, l, mid, index, val);
		} else {
			modify(rson, mid + 1, r, index, val);
		}
		pushup(node, l, r);
	}
	ull interval_query(int node, int l, int r, int L, int R) {
		if(l == L && r == R) {
			return s[node];
		}
		int mid = l + ((r-l)>>1);
		if(R <= mid) return interval_query(lson, l, mid, L, R);
		else if(L >= mid + 1) return interval_query(rson, mid + 1, r, L, R);
		else return b[(R - L + 1) - (mid - L + 1)] * interval_query(lson, l, mid, L, mid) + interval_query(rson, mid + 1, r, mid + 1, R);
	}

}tree, treei;//treei is inverse_str_segment_tree
char str[MAX_N];
char stri[MAX_N];
int main() {
	pre_b();
	scanf("%s", str + 1);
	int len = strlen(str+1);
	a[0] = 0;
	for(int i = 1; i <= len; ++i) {
		a[i] = ull(str[i]);
	}
	tree.build(1, 1, len);
	for(int i = 1; i <= len; ++i) {
		stri[i] = str[len-i+1];
	}
	for(int i = 1; i <= len; ++i) {
		a[i] = ull(stri[i]);
	}
	treei.build(1, 1, len);
	int m;
	cin >> m;
	char q[20];
	int l, r, i;
	char ch;
	ull h1,h2;
	while(m--) {
		scanf(" %s",q);
		if(q[0] == 'p') {
			scanf("%d%d",&l,&r);
			if(l == r) {
				printf("Yes\n");
				continue;
			}
			int n = l - r + 1;
			int mid = l + ((r-l)>>1);
			if(n&1) {
				h1 = tree.interval_query(1, 1, len, l, mid-1);
				h2 = treei.interval_query(1, 1, len, (len-r+1), (len-(mid+1)+1));
			} else {
				h1 = tree.interval_query(1, 1, len, l, mid);
				h2 = treei.interval_query(1, 1, len, (len-r+1), (len-(mid+1)+1));
			}
			if(h1 == h2) {
				printf("Yes\n");
			} else {
				printf("No\n");
			}
		} else {
			scanf("%d %c",&i,&ch);
			tree.modify(1, 1, len, i, ull(ch));
			treei.modify(1, 1, len, (len-i+1), ull(ch));
		}
	}
	return 0;
}

