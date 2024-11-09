---
title: "原创OI题目 GCD卷积 Problem and Solution"
date: 2020/12/06
date-modified: 2020/12/06
categories:
  - algorithm
  - math
  - combinatorics
  - original problem
  - solution
  
format:
  html: default
---

比赛用题面、题解、标程和数据生成器均已挂在 [git@github.com:sun123zxy/gcdconv.git](https://github.com/sun123zxy/gcdconv) 上。

## Problem

>**GCD卷积 (gcdconv.cpp/.in/.out) (1s,512MB)**
>
>**Description**
>
>定义一种新的卷积 —— GCD卷积，其接受两个长度为 $n$ 的序列 $f,g$ ，依据下式生成长度为 $n$ 的序列 $h$ ：
>$$
>h_k = \sum_{\gcd(i,j) = k} f_i g_j
>$$
>现给定序列 $f,g$ ，求各 $h_i$ 对 $998244353$ 取模后的值。
>
>**Input**
>
>第一行输入一个正整数 $n$ ，表示 $f,g$ 的长度。
>
>第二行输入 $n$ 个整数 $f_i$ 。
>
>第三行输入 $n$ 个整数 $g_i$ 。
>
>**Output**
>
>为减少输出量，只需输出1个整数，表示各 $h_i$ 对 $998244353$ 取模后的异或和。
>
>**Sample 1**
>
>**Sample 1 Input**
>
>```
>3
>5 1 4
>2 3 3
>```
>
>**Sample 1 Output**
>
>```
>78
>```
>
>**Sample 1 Explanation**
>$$
>\begin{aligned}
>h_1 &= f_1 ( g_1 + g_2 + g_3 ) + g_1 (f_2 + f_3) + f_2 g_3 + f_3 g_2 = 65 \\
>h_2 &= f_2 g_2 = 3 \\
>h_3 &= f_3 g_3 = 12
>\end{aligned}
>$$
>
>$$
>65 \oplus 3 \oplus 12 = 78
>$$
>
>**Sample 2**
>
>**Sample 2 Input**
>
>```
>4
>7 1 8 0
>6 2 9 1
>```
>
>**Sample 2 Output**
>
>```
>158
>```
>
>**Sample 2 Explanation**
>$$
>\begin{aligned}
>h_1 &= 213 \\
>h_2 &= 3 \\
>h_3 &= 72 \\
>h_4 &= 0
>\end{aligned}
>$$
>
>$$
>213 \oplus 3 \oplus 72 \oplus 0 = 158
>$$
>
>**Sample 3**
>
>见 `sample` 目录下 `gcdconv3.in/.ans` 。
>
>**Constraints**
>
>对20%的数据，$1 \le n \le 2000$ 。
>
>对100%的数据， $1 \le n \le 4 \times 10^5$ ， $0 \le f_i, g_i \le 998244352$ 。
>
>**Hints**
>
>时限在std的1.5倍左右。std没有卡常，数据有一定梯度，请放心食用。
>
>**Source**
>
>sun123zxy

## ???

+ 样例2比较暴力。

## 记号说明

默认诸如 $n,d,i,j,k$ 的下标变量的最大值为题目中的给出的序列长度，并把序列更换为数论函数来表示。

注意本题解中的 $n$ 通常是一个变量，和题目中定义的序列长度 $n$ 不同。

另外，用 $\circ$ 代表 $\gcd$ 卷积，即
$$
h(n) = (f \circ g)(n) = \sum_{\gcd(i,j) = n} f(i) g(j)
$$

## Solution

我们按照快速傅里叶变换（FFT）、快速莫比乌斯变换（FMT）解决卷积的思路来解决该问题——构造一种变换来满足卷积定理：

$$
\hat f(n) \hat g(n) = \widehat {(f \circ g)}(n)
$$
$\hat f$ 即对函数 $f$ 进行该变换后得到的函数。

通过一些敏锐的直觉，我们能感受到 $\gcd$ 和枚举约数或者倍数有一些关系。

容易想到构造出一种变换，称之为“倍数和变换”：
$$
\hat f(n) = \sum_{n|d} f(d)
$$
并根据莫比乌斯反演得到它的逆变换
$$
f(n) = \sum_{n|d} \mu(\frac n d) \hat f(d)
$$
（这实际上是标准莫比乌斯反演的另一种形式，详见后文 [Further Thoughts](#Further Thoughts) ）

这个变换对 $\gcd$ 卷积满足卷积定理，证明如下。

首先，写出 $\gcd$ 卷积
$$
h(k) = (f \circ g)(k) = \sum_{i,j} [\gcd(i,j)=k] f(i) g(j)
$$
 左右两边做倍数和变换：
$$
\begin{aligned}
\hat h(n) &= \sum_{n|k} \sum_{i,j} [\gcd(i,j)=k] f(i) g(j) \\
&= \sum_{i,j} \left( \sum_{n|k} [\gcd(i,j)=k] \right) f(i) g(j) \quad \\
&= \sum_{i,j} [n|i][n|j] f(i) g(j) \quad \\
&= \sum_{n|i} f(i) \sum_{n|j} g(j) \quad \\
&= \hat f(n) \hat g(n)
\end{aligned}
$$
得证。上述证明的核心在于 $\sum_{n|k} [\gcd(i,j)=k] = [n|i] [n|j]$ 。

于是，先对 $f,g$ 做倍数和变换，然后直接 $O(n)$ 点值相乘，再逆变换回来，就能得到 $f \circ g$ 。

那么剩下的问题在于如何快速做倍数和变换及其逆变换。这是非常simple的，直接暴力就好了。复杂度为 $O(n H(n))$ ，其中调和级数 $H(n)= \sum_{k=1}^{n} \frac 1 k$ ，有 $\lim_{n \to \infty} H(n) = \ln(n) + c$ 。欧拉常数 $c \approx 0.57721566490153286060651209$ 。

可以称之为“快速倍数和变换”。

总时间复杂度约为 $O(n \ln n)$ 。

## Code

```c++
/*
gcd卷积 (gcdconv) std
by sun123zxy 
*/
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include<queue>
#include<vector>
#include<map>
#include<set>
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
	if(x<0) return x+MOD;
	else if(x>=MOD) return x-MOD;
	else return x;
}

const ll MXN=5E5+5;
ll P[MXN],mu[MXN];ll pN;
bool notP[MXN];
void LinearSieve(ll n){
	notP[1]=1;for(ll i=2;i<=n;i++) notP[i]=0;
	P[1]=0;mu[1]=1;
	pN=0;
	for(ll i=2;i<=n;i++){
		if(!notP[i]){
			P[++pN]=i;
			mu[i]=-1;
		}
		for(ll j=1;i*P[j]<=n;j++){
			notP[i*P[j]]=1;
			if(i%P[j]==0){
				mu[i*P[j]]=0;
				break;
			}
			mu[i*P[j]]=mu[i]*mu[P[j]];
		}
	}
}

class Poly{public:
	ll& operator [] (ll idx){return cof[idx];}
	ll n;vector<ll> cof;
	Poly(){}
	Poly(ll n){Resize(n);}
	void Resize(ll n){
		this->n = n;
		cof.resize(n+1,0);
	}
};
namespace PC{//PolyCalc
	void FMT(Poly& A,ll typ){
		ll n=A.n;
		Poly B(n);
		for(ll i=1;i<=n;i++){
			for(ll j=1;i*j<=n;j++){
				ll t=A[i*j];if(typ==-1) t=t*mu[j]_;
				B[i]=PMod(B[i]+t);
			}
		}
		A=B;
	}
	Poly GcdConv(Poly A,Poly B){
		ll n=min(A.n,B.n); 
		Poly C(n);
		FMT(A,1);FMT(B,1);
		for(ll i=1;i<=n;i++) C[i]=A[i]*B[i]_;
		FMT(C,-1);
		return C;
	}
}

ll N;

int main(){
	freopen("gcdconv.in","r",stdin);
	freopen("gcdconv_std.out","w",stdout);
	N=Rd();
	LinearSieve(N);
	Poly A(N),B(N);
	for(ll i=1;i<=N;i++) A[i]=Rd();
	for(ll i=1;i<=N;i++) B[i]=Rd();
	Poly C=PC::GcdConv(A,B);
	ll ans=0;
	for(ll i=1;i<=N;i++) ans^=C[i];
	printf("%lld",ans);
	return 0;
}
```

## Further Thoughts

本题用到的是莫比乌斯反演的一种变形
$$
g(n) = \sum_{n|d} f(n) \iff f(d) = \sum_{n|d} \mu(\frac n d) g(d)
$$
原版莫比乌斯反演长这样
$$
g(n) = \sum_{d|n} f(n) \iff f(d) = \sum_{d|n} \mu(\frac n d) g(d)
$$
若把题目改成 $\mathrm{lcm}$ 卷积，即
$$
h_k = \sum_{\mathrm{lcm}(i,j) = k} f_i g_j
$$
用到的就是原版莫比乌斯反演。相应的构造一个“约数和变换”即可解决。而约数和变换可以用“快速约数和变换”（实际上就是个埃筛）实现，有兴趣的同学可以试试。

## Further Further Thoughts (update 2020/10/28)

我们来探讨一些更加接近本质的东西。

普通卷积（多项式乘法）、集合并/交卷积、还有 $\mathrm{lcm}$ / $\gcd$ 卷积，这三个问题我们是如何解决的？

关键在于——我们分别构造出了离散傅里叶变换、子集和/超集和变换（莫比乌斯变换）和约数和/倍数和变换来满足卷积定理。

而这三个变换的共性在哪里呢？

他们都是偏序集上的具有优美性质的广义反演。

+ 离散傅里叶变换用的是单位根反演——其偏序集是定义在自然数集上的小于等于关系；
+ 子集和/超集和变换用的是容斥原理——其偏序集是定义在集合上的包含关系；
+ 而约数和/倍数和变换则运用了莫比乌斯反演——其偏序集是定义在自然数集上的整除关系。

反演的意义就在于——它为将原来的“系数”转化为“点值”， $O(n)$ 乘起来后又转回“系数”提供了可能。而这三个特殊的反演之所以优美，就是因为我们发现了他们的“快速 (Fast) ”变换方式，让我们能通过这个Trick，免去了 $O(n^2)$ 的暴力运算。

还有更重要的一点——子集和/超集和变换与约数和/倍数和变换有着更加紧密的联系——这也是笔者由集合的FMT想到约数和变换的关键所在——以全体素数为基底可以张成一个包含全体正整数的“素数空间”（其良定义依赖于整数惟一分解定理）。

约数和变换就是“素数空间”中的高维前缀和，对应着“集合空间”中的子集和变换。换句话说，子集和变换、约数和变换分别是莫比乌斯变换在“素数空间”、“集合空间”的具体体现，它们的本质都是高维前缀和。

如果理解到上面这一点，就不难发现集合并卷积和 $\mathrm{lcm}$ 卷积的共性了——他们都把数放到了对两个输入元素各维度坐标取 $\max$ 后的位置；而集合交卷积和 $\gcd$ 卷积则是取 $\min$ 。

综上所述，有了上面的认识，可以说 $\gcd$ 卷积的发现是非常自然的。

## 后记 & 致谢

~~Idea是在语文课上摸出来的，Solution是在随后的数学课上想出来的~~

很早以前就觉得FMT和莫比乌斯反演有些说不清道不明的关系，出这道题也让笔者对其理解更加深入了。

其实早有大佬对 $\mathrm{lcm}$ / $\gcd$ 卷积展开过研究。Google一波可以发现国外数学社区有这方面的讨论，最晚2013年arXiv上就有讨论其性质的论文了（笔者驽钝，确实无力理解，感兴趣的大佬可以直接Google "GCD Convolution"了解）。只可惜如此自然而美妙的 $\gcd$ 卷积，竟然没有随着那篇集合幂级数的论文引入国内，让笔者感到有些遗憾。

总之，这道题深入的考察对FFT、FMT原理的理解以及对 $\gcd$ 、莫比乌斯反演的敏锐直觉，~~是不可多得的数论好题（~~

下面是战术感谢环节。

感谢keke学长为我们教授集合幂级数。

感谢TbYangZ、diong神、changruinian2020几位神仙的点拨。

And you...



<p style="text-align: right">——sun123zxy</p>

<p style="text-align: right">Sep. 2020 初稿完成</p>

<p style="text-align: right">Nov. 2020 最后更新</p>



**Next Phantasm...**