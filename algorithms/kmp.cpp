#include<cstdio>
#define MAX_N 1000000+10

int next[MAX_N];
int a[MAX_N], b[MAX_N];
int n, m;

void calc_next() {
	next[1] = 0;
	for(int i = 1, j = 0 ; i <= m - 1; ++i) {
		while(j > 0 && b[i + 1] != b[j + 1]) {
			j = next[j];
		}
		if(b[i + 1] == b[j + 1]) {
			j += 1;
			next[i + 1] = j;
		} else {
			next[i + 1] = 0;
		}	
	}
}

int kmp() {
	for(int i = 1, j = 1; i <= n; ++i) {
		while(j > 1 && a[i] != b[j]) {
			j = next[j - 1] + 1;
		}
		if(a[i] == b[j]) {
//			printf("i=%d,j=%d\n",i, j);
			j += 1;
		}
		if(j > m) return i - m + 1;
	}
	return -1;
}

int main() {
	int t;
	scanf("%d",&t);
	while(t--) {
		scanf("%d%d",&n, &m);
		for(int i = 1; i <= n; ++i) {
			scanf("%d",a + i);
//			printf("%d ",a[i]);
		}
//		printf("\n");
		for(int i = 1; i <= m; ++i) {
			scanf("%d", b + i);
//			printf("%d ",b[i]);
		}
//		printf("\n");
		calc_next();
		for(int i = 1; i <= m; ++i) {
//			printf("%d ",next[i]);
		}
//		printf("\n");
		int ans = kmp();
		printf("%d\n",ans);
	}
}
