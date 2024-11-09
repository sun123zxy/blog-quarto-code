---
title: "bsoj5988 [Achen模拟赛]期望 题解"
abstract: "“套着期望皮的容斥题。”"
date: 2019/12/18
date-modified: 2019/12/18
categories:
  - algorithm
  - math
  - number theory
  - combinatorics
  - solution
  
format:
  html: default
---

[bsoj5988](https://oj.bashu.com.cn/code/problempage.php?problem_id=5988)

>【题目背景】
>
>　　NOI2018 已经过去了许久，2019 届的 BSOIer 们退役的退役，颓废的颓废，计数能力大不如前。曾经的数数之王 xxyj 坦言：“我现在算期望都靠枚举”，嘴边还挂着什么“分布列”，什么“样本数据”，然后又继续投身于文化课学习中了。
>为了让 OI 的火炬传递下去，苣蒻 AChen 决定将 xxyj 退役前随口提到的期望问题交给你来解决。
>
>【题目描述】
>
>　　现有 m + 1 个白色的小球排成一列并从一开始编号。每次操作从前 m 个小球中随机选择一个涂黑。现在执行了 n 次操作，则编号最小的白球编号的期望是多少？
>
>【输入】
>
>　　从文件 mex.in 中读入数据。
>
>　　输入共一行两个整数 n,m。表示操作次数和白色小球的个数
>
>【输出】
>
>　　输出到文件 mex.out 中。
>
>　　若最小的白球编号的期望为 E，则输出一行表示：$((m^n)* E) \mod (10^9 + 7)$
>
>　　可以看出上式一定是个整数。
>
>【样例输入】
>
>1 1
>
>【样例输出】
>
>2
>
>【提示】
>
>$n \le 10^9, m \le 10^6$
>
>【来源】
>
>Achen

%%%AChen队爷%%%

考察对容斥的基础理解，挺不错的一题

易列答案式

$$
\sum_{i=1}^{m+1} P(mex = i) i
$$

对这种期望，常使用套路化法
$$
\sum_{i=0}^m P(mex>i)
$$

和式里面相当于要求已钦定$i$个确定的球，求随机选$n$次将这$i$个球全部染黑的概率。

考虑容斥。先随便选，然后减去一个球未染的，然后加上两个球未染的，...
$$
\sum_{i=0}^{m} \sum_{k=0}^{i} (-1)^k C_i^k (\frac{m-k}{m})^n
$$

是一个类似二项式反演但又不是的容斥

更换枚举
$$
\sum_{k=0}^{m} (-1)^k (\frac{m-k}{m})^n \sum_{i=k}^{m} C_i^k
$$
又由组合数的性质
$$
\sum_{i=k}^{n} C_i^k = C_{n+1}^{k+1}
$$
（容易通过杨辉三角和组合数的递推式证明）

得
$$
\sum_{k=0}^{m} (-1)^k (\frac{m-k}{m})^n C_{m+1}^{k+1}
$$
直接计算即可。

数据量如果更大的话，可以线筛出所有$n$次幂以省掉快速幂的$\log$。~~偷懒不写了（~~

```c++
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<algorithm>
#include<queue>
#include<vector>
#include<map>
#include<set>
using namespace std;
typedef long long ll;
const ll MOD=1E9+7;

ll QPow(ll x,ll up){
	x%=MOD;
	ll ans=1;
	while(up)
		if(up%2==0) x=x*x%MOD,up=up/2;
		else ans=ans*x%MOD,up--;
	return ans;
}
ll Inv(ll x){
	return QPow(x,MOD-2);
}

const ll MXN=1E6+5;
ll fac[MXN],facInv[MXN];
void FacInit(ll n){
	fac[0]=1;for(ll i=1;i<=n;i++) fac[i]=fac[i-1]*i%MOD;
	facInv[n]=Inv(fac[n]);for(ll i=n-1;i>=1;i--) facInv[i]=facInv[i+1]*(i+1)%MOD;
	facInv[0]=1;
}
ll C(ll n,ll m){
	if(n<m) return 0;
	return fac[n]*facInv[m]%MOD*facInv[n-m]%MOD;
}

ll N,M;
int main(){
	scanf("%lld%lld",&N,&M);
	FacInit(M+1);
	ll Ans=0;
	for(ll k=0;k<=M;k++){
		ll p=1;if(k%2==1) p=(-1+MOD)%MOD;
		Ans+=p*QPow(M-k,N)%MOD*C(M+1,k+1)%MOD;
		Ans%=MOD;
	}
	printf("%lld",Ans);
	return 0;
}
```