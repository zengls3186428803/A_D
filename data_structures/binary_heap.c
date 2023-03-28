#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_N 10000000
typedef long long ll;

typedef ll T;
T h[MAX_N];//maximum-heap
int size = 0;

void swap(T *a, T *b) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

T max(T a, T b) {
	return a > b ? a : b;
}

void up(int cur) {
	if(cur == 1) return;
	int par = cur / 2;
	if(h[par] < h[cur]) {
		swap(h + par, h + cur);
		up(par);
	}
}

void insert(T e) {
	h[++size]=e;
	up(size);
}

void down(int cur) {
	int lson = 2 * cur;
	if(lson > size) return;
	int rson = lson + 1;
	int target = lson;
	if(rson <= size && h[rson] > h[lson]) target = rson;
	if(h[target] > h[cur]) {
		swap(h + target, h + cur);
		down(target);
	}
}

void delete() {
	if(size == 0) {
		return ;
	}
	swap(h + 1, h + size);
	--size;
	down(1);
}

void visualize(width, cur) {
	int i = 0;
	for(i = 1; i <= width; ++i) {
		printf(" ");
	}
	d = printf("<%lld>", h[cur]);
	
}


int main() {
	int n;
	scanf("%d", &n);
	int i;
	T x;
	for(i = 1; i <= n; ++i) {
		scanf("%lld", &x);
		insert(x);
	}
	for(i = 1; i <= n; ++i) {
		printf("%lld\n", h[1]);
		delete();
	}
	return 0;
}
