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

vector<int> v;
int visited[100000+5];
void pf(int n, int m, int deep) {
	if(deep >= m) {
		for(int i = 0; i < v.size(); ++i) {
			printf("%d ", v[i]);
		}
		cout << endl;
		return ;
	}
	for(int i = 1; i <= n; ++i) {
		if(visited[i] == 0) {
			visited[i] = 1;
			v.push_back(i);
			pf(n, m, deep + 1);
			visited[i] = 0;
			v.pop_back();
		}
	}
}

void cf(int n, int m, int deep) {
	if(deep >= m) {
		for(int i = 0; i < v.size(); ++i) {
			printf("%d ", v[i]);
		}
		cout << endl;
		return ;
	}
	for(int i = 1; i <= n; ++i) {
		if(visited[i] == 0 && (v.size() == 0 || i > v[v.size() - 1])) {
			visited[i] = 1;
			v.push_back(i);
			cf(n, m, deep + 1);
			visited[i] = 0;
			v.pop_back();
		}
	}
}

int main() {
	memset(visited, 0, sizeof(visited));
	cout << "combination:" << endl;
	cf(4, 2, 0);
	cout << "permutation:" << endl;
	pf(4, 2, 0);
}
