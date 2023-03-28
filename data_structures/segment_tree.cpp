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
const int MAX_N = 1e6 + 10;
ll a[MAX_N];
ll s[4 * MAX_N];
ll lazy[4 * MAX_N];
struct Tree {
#define lson (node<<1)
#define rson ((node<<1)|1)
	void pushup(int node) {
		s[node] = s[lson] + s[rson];
	}
	void pushdown(int node, int l, int r) {
		if(lazy[node] == 0) return ;
		int mid = l + ((r-l)>>1);
		s[lson] += 1ll*(mid - l + 1) * lazy[node];
		s[rson] += 1ll*(r - (mid + 1) + 1) * lazy[node];
		lazy[lson] += lazy[node];
		lazy[rson] += lazy[node];
		lazy[node] = 0;
	}
	void build(int node, int l, int r) {
		if(l == r) {
			s[node] = a[l];
			return;
		}
		int mid = l + ((r-l)>>1);
		build(lson, l, mid);
		build(rson, mid + 1, r);
		pushup(node);
	}
	void interval_modify(int node, int l, int r, int L, int R, int val) {
		if(l == L && r == R) {
			s[node] += 1ll * (r - l + 1) * val;
			lazy[node] += val;
			return ;
		}
		pushdown(node, l, r);
		int mid = l + ((r-l)>>1);
		if(R <= mid) interval_modify(lson, l, mid, L, R, val);
		else if(L >= mid + 1) interval_modify(rson, mid + 1, r, L, R, val);
		else {
			interval_modify(lson, l, mid, L, mid, val);
			interval_modify(rson, mid + 1, r, mid + 1, R, val);
		}
		pushup(node);
	}
	ll interval_query(int node, int l, int r, int L, int R) {
		if(l == L && r == R) {
			return s[node];
		}
		pushdown(node, l, r);
		int mid = l + ((r-l)>>1);
		if(R <= mid) return interval_query(lson, l, mid, L, R);
		else if(L >= mid + 1) return interval_query(rson, mid + 1, r, L, R);
		else return interval_query(lson, l, mid, L, mid) + interval_query(rson, mid + 1, r, mid + 1, R);
	}

}tree;

int main() {
	int n, q;
	cin >> n >> q;
	for(int i = 1; i <= n; ++i) {
		scanf("%lld",a+i);
	}
	tree.build(1, 1, n);
	while(q--) {
		ll op, l, r, x ,i;
		scanf("%lld",&op);
		if(op == 1) {
			scanf("%lld%lld%lld", &l, &r, &x);
			tree.interval_modify(1, 1, n, l, r, x);
		} else {
			scanf("%lld%lld", &l, &r);
			printf("%lld\n", tree.interval_query(1, 1, n, l, r));
		}
	}
	return 0;
}
