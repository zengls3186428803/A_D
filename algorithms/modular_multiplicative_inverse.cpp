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
const int MAX_N = 5e6 + 10;
ll p = 1e9 + 7;
ll exgcd(ll a, ll b, ll &x, ll &y) {
	if(b == 0) {
		x = 1; y = 0; return a;
	}
	ll g = exgcd(b, a%b,y,x);
	y = y - (a/b) * x;
	return g;
}

ll qpow(ll a, ll b) {
	ll ans = 1;
	while(b) {
		if(b&1) ans = ans * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return ans;
}

ll t = 998244353;

ll a[MAX_N];
ll s[MAX_N];
ll sv[MAX_N];
ll inv[MAX_N];

template <typename T>
void read(T &x) {
	x = 0; int f = 1;
	char c = getchar();
	for (; !isdigit(c); c = getchar()) if (c == '-') f = -f;
	for (; isdigit(c); c = getchar()) x = x * 10 + c - '0';
	x *= f;
}

int main() {
	int n;
	cin >> n;
	for(int i = 1; i <= n; ++i) {
		scanf("%lld",a+i);
		//read(a[i]);
	}
	s[0] = 1;
	for(int i = 1; i <= n; ++i) {
		s[i] = s[i - 1] * a[i] % p;
	}
	ll x, y;
	/*
	for(int i = 1; i <= n; ++i) {
		exgcd(s[i],p,x,y);
		sv[i] = ((x%p)+p)%p;
	}
	*/
	sv[n] = qpow(s[n],p-2);
	//cout << sv[n] << endl;
	for(int i = n; i >= 2; --i) {
		sv[i - 1] = sv[i] * a[i] % p;
	}
	for(int i = 1; i <= n; ++i) {
		inv[i] = sv[i] * s[i-1] % p;
		//cout << "inv " << inv[i] << endl;
	}
	ll ans = 0;
	ll tt = 1;
	for(int i = n; i >= 1; --i) {
		ans = (ans + inv[i] * tt) % p;
		tt = tt * t % p;
	}
	cout << ans;
}
