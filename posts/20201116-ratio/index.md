---
title: "CodeChef-RNDRATIO Mysterious Ratio 题解"
abstract: "积性函数推式子纪念题。"
date: 2020/11/16
date-modified: 2020/11/16
categories:
  - algorithm
  - math
  - number theory
  - solution
  
format:
  html: default
---

[CodeChef-RNDRATIO Mysterious Ratio](https://www.codechef.com/problems/RNDRATIO)

>题意简述：
>
>对每个 $1 \le i \le n$ ，随机选择一个数 $A_i$ ，满足 $L_i \le A_i \le R_i$ ，求 $\mathrm{lcm}_{i=1}^n A_i$ 的期望。
>
>$1 \le n \le 10^5$ ， $1 \le L_i \le R_i \le 10^5$ 。
>
>Example Input:
>
>```
>2
>1
>1 3
>2
>2 4
>5 5
>```
>
>Example Output:
>
>```
>1
>15
>```

好久没做毒瘤式子题，推错好几次...

不过后面用到的乘法差分还是比较新颖，记录一下吧。

答案显然是这个式子——
$$
\prod_{i=1}^n (R_i-L_i+1)^{-1} \sum_{\begin{aligned}L_i \le &A_i \le R_i \\ 1 \le &i \le n\end{aligned}} \frac{\prod_{i=1}^n A_i}{\gcd_{i=1}^{n} A_i}
$$
前面的 $\prod$ 是常数可以先抛开，后面就暴算吧。（等式右边 $\cdots$ 后有少许说明）
$$
\begin{aligned}
\sum_{L_i \le A_i \le R_i} \frac{\prod A_i}{\gcd A_i}
&= \sum_{k=1}^{\min R_i} \sum_{L_i \le A_i \le R_i} [\gcd A_i = k] \frac{\prod A_i}{k} \\
&= \sum_k k^{n-1} \sum_{\lceil \frac{L_i}{k} \rceil \le B_i \le \lfloor \frac{R_i}{k} \rfloor} [\gcd B_i = 1] \prod B_i \qquad \qquad \cdots ( \ k B_i = A_i \ ) \\
&= \sum_k k^{n-1} \sum_{\lceil \frac{L_i}{k} \rceil \le B_i \le \lfloor \frac{R_i}{k} \rfloor} \sum_{d|\gcd B_i} \mu(d) \prod B_i \qquad \qquad \cdots ( \ \sum_{d|n} \mu(d) = [n=1] \ ) \\
&= \sum_k k^{n-1} \sum_{d=1}^{\lfloor \frac{\min R_i}{k} \rfloor} \mu(d) d^n \sum_{\lceil \frac{L_i}{kd} \rceil \le C_i \le \lfloor \frac{R_i}{kd} \rfloor} \prod C_i \qquad \quad \ \cdots ( \ d C_i = B_i , \ \frac{\lfloor \frac{a}{b} \rfloor}{c} = \lfloor \frac{a}{bc} \rfloor \ ) \\
&= \sum_k k^{n-1} \sum_{d} \mu(d) d^n \prod_{i=1}^n \sum_{\lceil \frac{L_i}{kd} \rceil \le x \le \lfloor \frac{R_i}{kd} \rfloor} x \\
&= \sum_k k^{n-1} \sum_{d} \mu(d) d^n \prod_{i=1}^n \left( S(\lfloor \frac{R_i}{kd} \rfloor) - S(\lceil \frac{L_i}{kd} \rceil -1) \right) \qquad \qquad \qquad \cdots ( \ S(n) = \sum_{x=1}^n x \ ) \\
&= \sum_{T=1}^{\min R_i} \left( \sum_{d|T} \mu(d) (\frac{T}{d})^{n-1} d^n \right) \prod_{i=1}^n \left( S(\lfloor \frac{R_i}{T} \rfloor) - S(\lceil \frac{L_i}{T} \rceil -1) \right) \qquad \ \cdots ( \ T = kd \ ) \\
&= \sum_T T^{n-1} \left( \sum_{d|T} \mu(d) d \right) \prod_{i=1}^n \left( S(\lfloor \frac{R_i}{T} \rfloor) - S(\lceil \frac{L_i}{T} \rceil -1) \right)
 \end{aligned}
$$

所以最终的式子便是

$$
\prod_{i=1}^n (R_i-L_i+1)^{-1} \sum_{T=1}^{\min R_i} T^{n-1} f(T) \prod_{i=1}^n \left( S(\lfloor \frac{R_i}{T} \rfloor) - S (\lceil \frac{L_i}{T} \rceil -1) \right)
$$
 其中
$$
S(n) = \frac{n(n+1)}{2}
$$

$$
f(n) = \sum_{d|T} \mu(d) d
$$

显然 $f(n)$ 是个积性函数，故可以用线性筛预处理出其前 $n$ 项，只需用到如下的性质

$$
f(p^c) = \sum_{i=0}^c \mu(p^i) p^i = 1 - p \quad (c \ge 2)
$$

其实如果所有 $Li,R_i$ 都相等的话就可以直接上杜教筛 / min25筛之类的黑科技把复杂度优化到 $O(n^{1-\omega})$ 了，但这道题很不套路的不相等，所以我们需要找到方法对所有 $T$ 快速计算后面的 $\prod$ 。

正解巧妙的使用了乘法差分。首先 $O(n \sqrt n)$ 对所有 $i$ 都整除分块一次得到区间，然后在 $T$ 上做乘法差分，每个分块区间乘上贡献 $S(\lfloor \frac{R_i}{T} \rfloor) - S (\lceil \frac{L_i}{T} \rceil -1)$ 。最后计算的时候扫一遍就能拿到每个 $T$ 对应的 $\prod$ 的值了。

注意对 $0$ 的特殊处理。

CodeChef上跑不过，懒得卡常了。

```c++
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include<queue>
#include<algorithm>
#include<map>
#include<set>
#include<vector>
using namespace std;
typedef long long ll;
typedef double db;
ll Rd(){
	ll ans=0;bool fh=0;char c=getchar();
	while(c<'0'||c>'9'){if(c=='-') fh=1; c=getchar();}
	while(c>='0'&&c<='9') ans=ans*10+c-'0',c=getchar();
	if(fh) ans=-ans;
	return ans;
}

ll CD(ll a,ll b){return (a-1)/b+1;} //CeilDiv
const ll MOD=998244353,INF=1E18;
#define _ %MOD
ll PMod(ll x){
	if(x>=MOD) return x-MOD;
	else if(x<0) return x+MOD;
	else return x;
}
ll QPow(ll x,ll up){
	PMod(x _);ll ans=1;
	while(up){
		if(up%2==0) x=x*x _,up/=2;
		else ans=ans*x _,up--;
	}
	return ans;
}
ll Inv(ll x){return QPow(x,MOD-2);}
const ll INV2=Inv(2);

const ll MXN=1E5+5;
ll P[MXN],pN;
bool notP[MXN];
ll f[MXN];//\sum_{d|n} \mu(d) d
void LinearSieve(ll n){
	notP[1]=1;for(ll i=2;i<=n;i++) notP[i]=0;
	pN=0;f[1]=1;
	for(ll i=2;i<=n;i++){
		if(!notP[i]){
			P[++pN]=i;
			f[i]=PMod(1-i);
		}
		for(ll j=1;j<=pN&&i*P[j]<=n;j++){
			notP[i*P[j]]=1;
			if(i%P[j]==0){
				f[i*P[j]]=f[i];
				break;
			}
			f[i*P[j]]=f[i]*f[P[j]]_;
		}
	}
}
ll S(ll n){return n*(n+1)_*INV2 _;}
ll N,MI;
ll L[MXN],R[MXN];
/*namespace Normal{
	void Solve(){
		ll Ans=0;
		for(ll t=1;t<=MI;t++){
			ll pdt=1;for(ll i=1;i<=N;i++) pdt=pdt*PMod(S(R[i]/t)-S(CD(L[i],t)-1))_;
			Ans=(Ans+QPow(t,N-1)*f[t]_*pdt)_;
		}
		ll pdt=1;for(ll i=1;i<=N;i++) pdt=pdt*Inv(R[i]-L[i]+1)_;
		Ans=Ans*pdt _;
		printf("%lld\n",Ans);
	}
}*/
namespace Lunatic{
	ll diff[MXN],zero[MXN];
	ll bl[MXN],br[MXN],blN,brN;
	void PutDiff(){
		for(ll k=1;k<=MI;k++) diff[k]=1,zero[k]=0;
		for(ll i=1;i<=N;i++){
			brN=0;for(ll t=1;t<=R[i];t=R[i]/(R[i]/t)+1) br[++brN]=R[i]/(R[i]/t);
			br[++brN]=MI;
			blN=0;for(ll t=MI;t>=1;t=CD(L[i],CD(L[i],t))-1) bl[++blN]=t;
			for(ll j=blN,k=1,st=1,ed;st<=MI;st=ed+1){
				if(bl[j]<br[k]||k>brN) ed=bl[j--];
				else ed=br[k++];
				ll t=ed;
				ll val=PMod(S(R[i]/t)-S(CD(L[i],t)-1));
				if(val){
					diff[st]=diff[st]*val _;
					diff[ed+1]=diff[ed+1]*Inv(val)_;
				}else{
					zero[st]++;
					zero[ed+1]--;
				}
			}
		}
	}
	void Solve(){
		PutDiff();
		ll Ans=0;
		ll pdt=1,zn=0;
		for(ll t=1;t<=MI;t++){
			zn+=zero[t];pdt=pdt*diff[t]_;
			if(!zn) Ans=(Ans+QPow(t,N-1)*f[t]_*pdt)_;
		}
		pdt=1;for(ll i=1;i<=N;i++) pdt=pdt*Inv(R[i]-L[i]+1)_;
		Ans=Ans*pdt _;
		printf("%lld\n",Ans);
	}
}
int main(){
	LinearSieve(MXN-1);
	ll T=Rd();while(T--){
		N=Rd();
		MI=INF;
		for(ll i=1;i<=N;i++){
			L[i]=Rd();R[i]=Rd();
			MI=min(MI,R[i]);
		}
		//Normal::Solve();
		Lunatic::Solve();
	}
	return 0;
}
```
