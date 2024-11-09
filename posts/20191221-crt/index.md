---
title: "中国剩余定理（CRT）及其扩展（ExCRT）"
date: 2019/12/21
date-modified: 2020/12/02
categories:
  - algorithm
  - math
  - number theory
  
format:
  html: default
---

## 中国剩余定理 CRT

### 推导

> 给定 $n$ 个同余方程
> 
> $$
> \left\{
\begin{aligned}
x &\equiv a_1 \pmod{m_1} \\
x &\equiv a_2 \pmod{m_2} \\
&... \\
x &\equiv a_n \pmod{m_n}
\end{aligned}
\right.
> $$
> 
> $m_1, m_2 , ... , m_n$ 两两互质
> 
> 令 $M = \prod_{i=1}^{n} m_i$ ，求 $x \mod M$ 

解决该问题的方法是构造。

我们假定最终答案的形式是一个 $n$ 个项的和式，对每个同余方程的构造反应在对应项的系数上。

如果要对每一个项分别构造，就要求为每一项乘上一个合适的数，使得每项构造的系数对其他方程的结果没有影响。

容易想到构造

$$
M_i = \frac{M}{m_i}
$$

显然该数仅在模 $m_i$ 时不为 $0$ ，于是改变该项的系数将不会对其他方程造成影响。

现在我们希望该项模 $m_i$ 意义下是 $a_i$ ，但上一次的构造残留下了一个 $M_i$ 。简单粗暴的乘上 $M_i$ 在模 $m_i$ 意义下的逆元 $\mathrm{inv}_{m_i}(M_i)$ ，让该项在模 $m_i$ 意义下变为 $1$ ，然后乘上 $a_i$ 就构造出来了。

综上，答案为

$$
\sum_{i=1}^{n} M_i \mathrm{inv}_{m_i}(M_i) a_i \mod{M}
$$

模数互质条件保证了 $M_i$ 在模 $m_i$ 意义下非 $0$ ，进而保证了 $\mathrm{inv}_{m_i}(M_i)$ 的存在。

### 实现

大部分题的 $m_i$ 都是质数，求逆元快速幂即可。

对于一般的情况，上ExGCD就行。

板题：[洛谷P1495 曹冲养猪](https://www.luogu.org/problemnew/show/P1495)

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

namespace ExGcd{
    ll x,y;
    ll ExGcd(ll a,ll b){
        ll ans;
        if(a%b==0){
            x=0;y=1;ans=b;
        }else{
            ans=ExGcd(b,a%b);
            ll x1=x,y1=y;
            x=y1;y=x1-a/b*y1;
        }
        return ans;
    }
    bool SolveEqu(ll a,ll b,ll c){
        ll d=ExGcd(a,b);
        if(c%d!=0) return 0;
        x*=c/d;y*=c/d;
        x=(x%b+b)%b;
        y=(c-a*x)/b;
        return 1;
    }
}
ll Inv(ll a,ll m){
    ExGcd::SolveEqu(a,m,1);
    return ExGcd::x;
}

const ll CRTN=20;
namespace CRT{
    ll N;
    ll m[CRTN],a[CRTN];
    ll Sol(){
        ll ans=0,M=1;
        for(ll i=1;i<=N;i++) M*=m[i];
        for(ll i=1;i<=N;i++){
            ll Mi=M/m[i];
            ans=(ans+Mi*Inv(Mi,m[i])*a[i])%M;
        }
        return ans;
    }
}
int main(){
    using namespace CRT;
    scanf("%lld",&N);
    for(ll i=1;i<=N;i++)
        scanf("%lld%lld",&m[i],&a[i]);
    printf("%lld",Sol());
    return 0;
}
```

## 扩展中国剩余定理 ExCRT

ExCRT和CRT并没有什么关系，~~正如ExLucas和Lucas也没什么关系~~

其实从纯推理的角度来看，ExCRT可能还要好想一点（

### 推导

问题同CRT，但是模数是任意的，并不要求互质。

这时，我们就不能保证存在逆元了。那么如何解决该问题呢？

考虑如何合并两个方程。如果我们找到了合并的方法，就能如法炮制将$n$个方程依次合并起来，得到答案。

$$
\left\{
\begin{aligned}
x &\equiv a_1 \pmod{m_1} \\
x &\equiv a_2 \pmod{m_2}
\end{aligned}
\right.
$$

去掉同余，化为不定方程

$$
\left\{
\begin{aligned}
x &= m_1 y_1 + a_1 \\
x &= m_2 y_2 + a_2
\end{aligned}
\right.
$$

于是得到

$$
m_1 y_1 + a_1 = m_2 y_2 + a_2
$$

只要找到一组满足该式的 $y_1$ 和 $y_2$ ，就能反算出 $x$ ，实现合并。

而我们得到的是一个二元一次不定方程，可以用ExGCD求解。

化为标准式

$$
m_1 y_1 - m_2 y_2 = a_2 - a_1
$$

解就是了。由裴蜀定理，若 $\gcd(m_1,m_2) |  a_2-a_1$ 不成立，说明同余方程组无解。

于是最后化得的合并式为

$$
x \equiv m_1 y_1 + a_1 \pmod{\mathrm{lcm}(m_1,m_2)}
$$

#### Update 2020/12/02 关于合并后的模数

之前没有讲清楚 $\pmod{\mathrm{lcm}(m_1,m_2)}$ 是怎么来的，这里补充一笔。

根据二元一次不定方程理论， $y_1$ 的通解形式应为 $y_1 = y + k \frac{m_2}{\gcd(m_1,m_2)}$ （ $y$ 是某一个特解），此时带回得到 $x =k \frac{m_1 m_2}{\gcd(m_1,m_2)} + m_1 y + a_1$ ，模上个 $\mathrm{lcm}(m_1,m_2) = \frac{m_1 m_2}{\gcd(m_1,m_2)}$ 就是最终的合并式了。

### 实现

唯一需要注意的地方是，本来解方程应该解 $(m_1,-m_2,a_2-a_1)$ ，但ExGCD不好处理负数，所以把 $- m_2$ 改成了 $m_2$ 。因为我们并不需要用到 $y_2$ ，所以不会影响求解。 

板题：[poj2891 Strange Way to Express Integers](http://poj.org/problem?id=2891) or [洛谷P4777 扩展中国剩余定理（EXCRT）](https://www.luogu.com.cn/problem/P4777)

会被卡乘法爆ll，~~懒得改~~

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

namespace ExGcd{
    ll x,y;
    ll ExGcd(ll a,ll b){
        ll ans;
        if(a%b==0){
            x=0;y=1;ans=b;
        }else{
            ans=ExGcd(b,a%b);
            ll x1=x,y1=y;
            x=y1;y=x1-a/b*y1;
        }
        return ans;
    }
    bool SolveEqu(ll a,ll b,ll c){
        ll d=ExGcd(a,b);
        if(c%d!=0) return 0;
        x*=c/d;y*=c/d;
        x=(x%b+b)%b;
        y=(c-a*x)/b;
        return 1;
    }
}
ll Gcd(ll a,ll b){
    if(a%b==0) return b;
    return Gcd(b,a%b);
}
namespace ExCRT{
    ll a1,m1;
    void Init(){
        a1=0;m1=1;
    }
    void Expand(ll a2,ll m2){
        ExGcd::SolveEqu(m1,m2,a2-a1);
        ll y1=ExGcd::x;
        ll mn=m1*m2/Gcd(m1,m2);
        a1=(m1*y1+a1)%mn;
        m1=mn;
    }
}
int main(){
    ll N;scanf("%lld",&N);
    ExCRT::Init();
    for(ll i=1;i<=N;i++){
        ll a,m;scanf("%lld%lld",&m,&a);
        ExCRT::Expand(a,m);
    }
    printf("%lld",ExCRT::a1);
    return 0;
}
```
