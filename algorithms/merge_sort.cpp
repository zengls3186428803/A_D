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

template<typename T>
bool cmp1(T &a, T &b) {
	return a < b;
}

template<typename T> 
bool cmp2(T &a, T &b){
	return a > b;
}

template<typename T>
void merge(T *a, int L, int mid, int R, bool cmp(T &, T &), T *b) {
	int l = L, r = mid + 1, k = 0;
	while(l <= mid && r <= R) {
		if(cmp(a[l], a[r])) {
			b[k++] = a[l++];
		}
		else {
			b[k++] = a[r++];
		}
	}
	while(l <= mid) b[k++] = a[l++];
	while(r <= R) b[k++] = a[r++];
	for(int i = 0; i < k; ++i) {
		a[L + i] = b[i];
	}
}

template<typename T>
void merge_sort_(T *a, int l, int r, bool cmp(T&, T&), T *b) {
	if(l >= r) return ;
	int mid = (l + r) >> 1;
	merge_sort_(a, l, mid, cmp, b);
	merge_sort_(a, mid + 1, r, cmp, b);
	merge(a, l, mid, r, cmp,b);
}

template<typename T>
void merge_sort(T *start, T *end, bool cmp(T &, T &)) {
	int len = end - start;
	T *b = (T *)malloc(sizeof(T) * (len + 1));
	merge_sort_(start, 0, end - start - 1, cmp, b);
}

int main() {
	int a[20] = {0,9,8,7,6,5,4,1,2,3};
	int b[20];
	merge_sort(a, a + 10, cmp2);
	for(int i = 0; i < 10; ++i) {
		cout << a[i] << " ";
	}
}
