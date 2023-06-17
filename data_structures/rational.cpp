#include<cstdio>
#include<algorithm>
#include<cstdlib>
#include<cstring>
#include<iostream>
using namespace std;
const int maxStr = 10000;
const int maxCode = 10000;
typedef int CodeType;
typedef int CountType;
typedef int SignType;
typedef struct BigInt{//非负; 
	CodeType code[maxCode];
	CountType count;
}BigInt;
BigInt wei={{0},1},ten={{0,1},2},zero={{0},1};

typedef struct Big_Int{
	SignType sign;//0正1负 ;
	BigInt data;
}Big_Int;
Big_Int Wei={0,{{0},1}},Ten={0,{{0,1},2}},Zero={0,{{0},1}};

typedef struct Rational{//denominator is +;
	Big_Int numerator, denominator;
}Rational;
Rational ZEro={{0,{{0},1}},{0,{{1},1}}};

int absCmp(const BigInt &a, const BigInt &b){//a > b:return 1; a == b return 0; a < b return -1; 
	if(a.count > b.count)return 1;
	else if(a.count < b.count)return -1;
	else {
		for(int i = a.count - 1;i >= 0;--i){
			if(a.code[i] > b.code[i])return 1;
			else if(a.code[i] < b.code[i])return -1;
		}
		return 0;
	}
}
BigInt operator+(BigInt a, BigInt b){
	BigInt ans;
	memset(ans.code,0,sizeof(ans.code));
	if(a.count < b.count) swap(a,b);
	for(int i = 0;i < b.count;++i) ans.code[i] = a.code[i] + b.code[i];
	for(int i = b.count;i < a.count;++i) ans.code[i] = a.code[i];
	int carry=0,i=0;
	while(1){
		ans.code[i] = carry + ans.code[i];
		carry = ans.code[i] / 10;
		ans.code[i] = ans.code[i] % 10;
		++i;
		if(i >= a.count)break;
	}
	if(carry > 0){
		ans.code[i] = carry;
		carry = 0;
		++i;
	}
	ans.count = i;
	return ans;
}
BigInt operator-(BigInt a, BigInt b){
	BigInt ans;
	memset(ans.code,0,sizeof(ans.code));
	if(absCmp(a,b) < 0) swap(a, b);
	for(int i = 0;i < b.count;++i) ans.code[i] = a.code[i] - b.code[i];
	for(int i = b.count;i < a.count;++i) ans.code[i] = a.code[i];
	int carry=0, i=0;
	while(1){
		ans.code[i] += carry;
		carry = 0;
		if(ans.code[i] < 0){
			carry = -1;
			ans.code[i]+=10;
		}
		++i;
		if(i >= a.count)break;
	}
	for(int i = a.count - 1;i >= 0;--i){
		if(ans.code[i] > 0 || i == 0){
			ans.count = i + 1;
			break;
		}
	}
	return ans;
}
BigInt operator*(BigInt a, BigInt b){
	BigInt ans;
	memset(ans.code,0,sizeof(ans.code));
	for(int i = 0;i < a.count;++i){
		for(int j = 0;j < b.count;++j){
			ans.code[i+j] += a.code[i] * b.code[j];
		}
	}
	int i = 0, carry = 0;
	while(1){
		ans.code[i] = ans.code[i] + carry;
		carry = ans.code[i] / 10;
		ans.code[i] = ans.code[i] % 10;
		++i;
		if(i > a.count + b.count -2)break;
	}
	while(1){
		if(carry == 0)break;
		ans.code[i] += carry;
		carry = ans.code[i] / 10;
		ans.code[i] = ans.code[i] % 10;
		++i;
	}
	ans.count = i;
	for(int i = ans.count - 1;i >= 0;--i){
		if(ans.code[i] > 0 || i == 0){
			ans.count = i + 1;
			break;
		}
	}
	return ans;
}
BigInt operator/(BigInt a, BigInt b){
	if(absCmp(b,zero) == 0){
		printf("错误：除数为零。\n");
		exit(1);
	}
	BigInt ans,temp,R;
	memset(ans.code,0,sizeof(ans.code));
	memset(temp.code,0,sizeof(temp.code));
	memset(R.code,0,sizeof(R.code));
	R.count = 1;
	for(int i = a.count - 1;i >= 0;--i){
		wei.code[0] = a.code[i];
		temp = R * ten + wei;
		int count = 0;
		while(absCmp(temp,b) >= 0){
			temp = temp - b;
			++count;
		}
		ans.code[i] = count;
		R = temp;
	}
	for(int i = a.count - 1;i >= 0;--i){
		if(ans.code[i] > 0 || i == 0){
			ans.count = i + 1;
			break;
		}
	}
	return ans;
}
BigInt operator%(BigInt a, BigInt b){
	if(absCmp(b,zero) == 0){
		printf("错误：模数为零。\n");
		exit(1);
	}
	BigInt temp,R;
	memset(temp.code,0,sizeof(temp.code));
	memset(R.code,0,sizeof(R.code));
	R.count = 1;
	for(int i = a.count - 1;i >= 0;--i){
		wei.code[0] = a.code[i];
		temp = R * ten + wei;
		while(absCmp(temp,b) >= 0){
			temp = temp - b;
		}
		R = temp;
	}
	return R;
}
BigInt gcd(BigInt a, BigInt b){
	if(absCmp(a,b) < 0) swap(a, b);
	BigInt r;
	while(absCmp(b, zero) != 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}
BigInt lcm(BigInt a, BigInt b){
	return (a * b) / gcd(a, b);
}

char str[maxStr];
void input(Big_Int &A){
	char str[maxStr];
	scanf(" %s",str);
	int count=strlen(str);
	if(str[0] == '-'){
		A.sign = 1;
		A.data.count = count - 1;
	}
	else{
		A.sign = 0;
		A.data.count = count;
	}
	if(A.sign == 0)for(int i = count - 1;i >= 0;--i) A.data.code[count - 1 - i] = str[i] - '0';
	else for(int i = count - 1;i >= 1;--i) A.data.code[count - 1 - i] = str[i] - '0';
	return;
}
void output(Big_Int &A){
	if(A.sign == 1)printf("-");
	for(int i = A.data.count-1;i >= 0;--i){
		printf("%d",A.data.code[i]);
	}
}
int cmp(Big_Int a, Big_Int b){
	if(a.sign == 0 && b.sign == 0) return absCmp(a.data, b.data);
	if(a.sign == 0 && b.sign == 1) return 1;
	if(a.sign == 1 && b.sign == 0) return -1;
	if(a.sign == 1 && b.sign == 1) return -absCmp(a.data, b.data);
}
Big_Int operator+(Big_Int a, Big_Int b){
	Big_Int ans;
	memset(&ans,0,sizeof(ans));
	if(a.sign == 0 && b.sign == 0){
		ans.data = a.data + b.data;
		ans.sign = 0;
		return ans;
	}
	if(a.sign == 0 && b.sign == 1){
		if(absCmp(a.data, b.data) >= 0) ans.sign = 0;
		else ans.sign = 1;
		ans.data = a.data - b.data;
		return ans;
	}
	if(a.sign == 1 && b.sign == 0){
		if(absCmp(a.data, b.data) <= 0) ans.sign = 0;
		else ans.sign = 1;
		ans.data = a.data - b.data;
		return ans;
	}
	if(a.sign == 1 && b.sign == 1){
		ans.sign = 1;
		ans.data = a.data + b.data;
		return ans;
	}
}
Big_Int operator-(Big_Int a, Big_Int b){
	Big_Int ans;
	memset(&ans,0,sizeof(ans));
	if(a.sign == 0 && b.sign == 0){
		if(absCmp(a.data, b.data) >=0) ans.sign = 0;
		else ans.sign = 1;
		ans.data = a.data - b.data;
		return ans;
	}
	if(a.sign == 0 && b.sign == 1){
		ans.sign = 0;
		ans.data = a.data + b.data;
	}
	if(a.sign == 1 && b.sign == 0){
		ans.sign = 1;
		ans.data = a.data + b.data;
		return ans;
	}
	if(a.sign == 1 && b.sign == 1){
		if(absCmp(a.data, b.data) > 0) ans.sign = 1;
		else ans.sign = 0;
		ans.data = a.data - b.data;
	}
}
Big_Int operator*(Big_Int a, Big_Int b){
	Big_Int ans;
	memset(&ans,0,sizeof(ans));
	if(a.sign == b.sign) ans.sign = 0;
	else{
		if(cmp(a, Zero) == 0 || cmp(b, Zero) == 0) ans.sign = 0;
		else ans.sign = 1;
	}
	ans.data = a.data * b.data;
	return ans;
}
Big_Int operator/(Big_Int a, Big_Int b){
	if(cmp(b, Zero) == 0){
		printf("错误: 除数为零\n");
		exit(1);
	}
	if(cmp(a, Zero) == 0) return Zero;
	Big_Int ans;
	memset(&ans,0,sizeof(ans));
	if(a.sign == b.sign) ans.sign = 0;
	else ans.sign = 1;
	ans.data = a.data / b.data;
	if(absCmp(ans.data, zero) == 0) return Zero;
	return ans;
}
Big_Int operator%(Big_Int a, Big_Int b){
	if(cmp(b, Zero) == 0){
		printf("错误: 模数为零\n");
		exit(1);
	}
	if(cmp(a, Zero) == 0) return Zero;
	Big_Int ans;
	memset(&ans,0,sizeof(ans));
	ans.sign = a.sign;
	ans.data = a.data % b.data;
	if(absCmp(ans.data, zero) == 0) return Zero;
	return ans;
}
Big_Int gcd(Big_Int &a, Big_Int &b){
	Big_Int ans;
	ans.sign = 0;
	ans.data = gcd(a.data, b.data);
	return ans;
}
Big_Int lcm(Big_Int &a, Big_Int &b){
	Big_Int ans;
	ans.sign = 0;
	ans.data = lcm(a.data, b.data);
	return ans;
}

int cmp(const Rational &a, const Rational &b){
	if(cmp(a.numerator * b.denominator, b.numerator * a.denominator) > 0) return 1;
	if(cmp(a.numerator * b.denominator, b.numerator * a.denominator) < 0) return -1;
	return 0;
}
void update(Rational &A){
	if(A.denominator.sign == 1){
		A.denominator.sign = 0;
		A.numerator.sign = (A.numerator.sign == 0 ? 1 : 0);
	}
	Big_Int G = gcd(A.numerator,A.denominator);
	A.numerator = A.numerator / G;
	A.denominator = A.denominator / G;
}
Rational operator+(Rational a, Rational b){
	Rational ans;
	ans.numerator = a.numerator * b.denominator + b.numerator * a.denominator;
	ans.denominator = a.denominator * b.denominator;
	if(cmp(ans.numerator, Zero) == 0) return ZEro;
	update(ans);
	return ans;
}
Rational operator-(Rational a, Rational b){
	Rational ans;
	ans.numerator = a.numerator * b.denominator - b.numerator * a.denominator;
	ans.denominator = a.denominator * b.denominator;
	if(cmp(ans.numerator, Zero) == 0) return ZEro;
	update(ans);
	return ans;
}
Rational operator*(Rational a, Rational b){
	Rational ans;
	ans.numerator = a.numerator * b.numerator;
	ans.denominator = a.denominator * b.denominator;
	if(cmp(ans.numerator, Zero) == 0) return ZEro;
	update(ans);
	return ans;
}
Rational operator/(Rational a, Rational b){
	if(cmp(b.numerator, Zero) == 0){
		printf("错误: 除数为零\n");
		exit(1);
	}
	Rational ans;
	ans.numerator = a.numerator * b.denominator;
	ans.denominator = a.denominator * b.numerator;
	update(ans);
	if(cmp(ans.numerator, Zero) == 0) return ZEro;
	update(ans);
	return ans;
}

void input(Rational &A){ //输入以后可以再优化一下 
	input(A.numerator);
	char ch;
	scanf(" %c",&ch);
	input(A.denominator);
	update(A);
}
void output(Rational &A){
	if(cmp(A.numerator, Zero) == 0){
		printf("0");
		return ;
	}
	output(A.numerator);
	printf("/");
	output(A.denominator);
}

int main(){
	Rational a, b, c;
	input(a);
	input(b);
	c = a + b;
	cout << "+ :";
	output(c);
	printf("\n");
	c = a - b;
	cout << "- :";
	output(c);
	printf("\n");
	c = a * b;
	cout << "* :";
	output(c);
	printf("\n");
	cout << "/ :";
	c = a / b;
	output(c);
	printf("\n");
//	c = a % b;
//	cout << "\% :";
//	output(c);
//	printf("\n");
//	c = gcd(a, b);
//	cout << "gcd :";
//	output(c);
//	printf("\n");
//	c = lcm(a, b);
//	cout << "lcm :";
//	output(c);
//	printf("\n");
}
