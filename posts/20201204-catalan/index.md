---
title: "卡特兰数 题解"
date: 2020/12/04
date-modified: 2020/12/04
categories:
  - algorithm
  - combinatorics
  - solution
  
format:
  html: default
---

[bsoj7107](https://oj.bashu.com.cn/code/problempage.php?problem_id=7107) ，来源不明。

>**卡特兰数**
>
>**题目描述**
>
>今天，接触信息学不久的小A刚刚学习了卡特兰数。
>
>卡特兰数的一个经典定义是，将 $n$ 个数依次入栈，合法的出栈序列个数。
>
>小A觉得这样的情况太平凡了。于是，他给出了 $m$ 组限制，每个限制形如 $(f_i,g_i)$ ，表示 $f_i$ 不能在 $g_i$ 之后出栈。
>
>他想求出：在满足了这 $m$ 组限制的前提下，共有多少个合法的出栈序列。他不喜欢大数，你只需要求出答案在模 $998244353$ 意义下的值即可。
>
>**输入格式**
>
>输入第一行为两个非负整数 $n$ 、 $m$ ，含义题面已给出。
>
>接下来 $m$ 行，每行两个正整数， $(f_i,g_i)$ 表示一组限制。
>
>**输出格式**
>
>输出一行，为一个非负整数，表示你求得的答案 $\mod 998244353$ 。
>
>**样例输入**
>
>```
>3 1
>2 3
>```
>
>**样例输出**
>
>```
>3
>```
>
>**样例解释**
>
>可以验证 $\{1,2,3 \}$ ， $\{2,1,3 \}$ ， $\{2,3,1 \}$ 都是合乎条件的。
>
>**数据规模**
>
>$n \le 300$ , $m \le \frac{n(n-1)}{2}$ , $f_i,g_i \le n$ 。
>
>部分数据的 $m$ 较小。

~~考前一天写题解会不会涨RP~~

套路但是还是感觉很巧妙的转换。关键在于把限制形式化的写出来方便观察。

考虑Catalan数的递推公式推导方式——枚举最后出栈的元素 $k$ 。其递推式为
$$
\begin{aligned}
c_0 &= 1 \\
c_n &= \sum_{k=1}^n c_{k-1} c_{n-k}
\end{aligned}
$$
加入限制后无法使用排列的相对顺序dp，故需改造原来的递推。定义 $h_{l,r}$ 为排列 $[l,r]$ 不同的出栈序列方案数，若暂不考虑限制，有
$$
h_{l,r} = \sum_{k=l}^r h_{l,k-1} h_{k+1,r}
$$
显然 $c_n = h_{1,n}$ 。

递推中，出栈序列的相对顺序为 $([l,k-1],[k+1,r],\{ k \})$ 。限制 $(f_i, g_i)$ 要求 $f_i$ 在 $g_i$ 之前出栈。故满足 $g_i \in [l,k-1], \ f_i \in [k,r]$ 或者 $g_i \in [k+1,r], \ f_i = k$ 的 $k$ 都不合法，不应对 $h_{l,r}$ 产生贡献。故在此dp中枚举所有限制判断合法性即可获得 $O(n^3 m)$ 的算法。

这种方法看着就很naive，考虑优化合法性判断。容易发现把 $(f_i,g_i)$ 投到二维平面上后，判断合法性只需询问 $(k,l) - (r,k-1)$ 、 $(k,k+1) - (k,r)$ 两个矩形中是否有点。二维前缀和处理即可 $O(1)$ 判断。于是这样就可以 $O(n^3)$ 通过本题了。

```c++
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstdlib>
using namespace std;
typedef long long ll;
ll Rd(){
	ll ans=0;bool fh=0;char c=getchar();
	while(c<'0'||c>'9'){if(c=='-') fh=1; c=getchar();}
	while(c>='0'&&c<='9') ans=ans*10+c-'0',c=getchar();
	if(fh) ans=-ans;
	return ans;
}
const ll MOD=998244353;
#define _ %MOD
ll PMod(ll x){
	if(x>=MOD) return x-MOD;
	else if(x<0) return x+MOD;
	else return x;
}

const ll MXN=305;
ll N,M;
ll A[MXN][MXN];
ll S[MXN][MXN];
void SpawnSum(){
	for(ll i=1;i<=N;i++)
		for(ll j=1;j<=N;j++)
			S[i][j]=S[i-1][j]+S[i][j-1]-S[i-1][j-1]+A[i][j];
}
bool Check(ll x1,ll y1,ll x2,ll y2){
	return S[x2][y2]-S[x1-1][y2]-S[x2][y1-1]+S[x1-1][y1-1];
}
ll f[MXN][MXN];
void Solve(){
	for(ll i=0;i<=N;i++) f[i+1][i]=1;
	for(ll len=1;len<=N;len++){
		for(ll l=1;l+len-1<=N;l++){
			ll r=l+len-1;
			for(ll k=l;k<=r;k++){
				if(Check(k,l,r,k-1)) continue;
				if(Check(k,k+1,k,r)) continue;
				f[l][r]=(f[l][r]+f[l][k-1]*f[k+1][r])_;
			}
		}
	}
	cout<<f[1][N];
}
int main(){
	N=Rd();M=Rd();
	for(ll i=0;i<=N;i++) for(ll j=0;j<=N;j++) A[i][j]=S[i][j]=0;
	for(ll i=1;i<=M;i++){
		ll x=Rd(),y=Rd();
		A[x][y]=1;
	}
	SpawnSum();
	Solve();
	return 0;
}
```

