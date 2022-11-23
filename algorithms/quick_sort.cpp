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

int *par(int *start, int *end) {
	int *lp = start, *rp = end - 1;
	int *p = start + rand() % (end - start);
	swap(*start, *p);
	while(lp < rp) {
		while(lp < rp && *rp >= *start) rp--;
		while(lp < rp && *lp <= *start) lp++;
		swap(*lp, *rp);
	}
	swap(*start, *lp);
	return lp;
}

void quick_sort(int *start, int *end) {
	if(end - start <= 1) return ;
	int *flag = par(start, end);
	quick_sort(start, flag);
	quick_sort(flag + 1, end);
}

int main() {
	int a[20] = {1,2,0,5,8,7,6,5,4,3};
	srand((unsigned int)time(NULL));
	quick_sort(a, a + 10);
	for(int i = 0; i < 10; ++i) {
		cout << a[i] << " ";
	}
}
