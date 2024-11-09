---
title: "由 [SDOI2012]Longge的问题 探讨欧拉函数和莫比乌斯函数的一些性质和关联"
date: 2019/09/22
date-modified: 2019/11/04
categories:
  - algorithm
  - math
  - number theory
  - solution
  
format:
  html: default
---

## 本题题解

题目传送门：[https://www.luogu.org/problem/P2303](https://www.luogu.org/problem/P2303)

> 给定一个整数$n$，求
> 
> $$
> \sum_{i=1}^n \gcd(n,i)
> $$

蒟蒻随便yy了一下搞出来个$O(\sqrt{n})$的算法 ~~这题数据怎么这么水~~

首先看到gcd我们就下意识的对它反演一波对吧

### 第一步

$$
\sum_{i=1}^n \gcd(n,i) = \sum_{d|n} \varphi(d) \frac{n}{d}
$$

这里提供两种化法，得到的结果都是这个。

#### 法一

根据欧拉函数和式

$$
n = \sum_{d|n} \varphi(d)
$$

暴力推导即可

$$
\begin{aligned}
\sum_{i=1}^n \gcd(n,i) &= \sum_{i=1}^n \sum_{d|\gcd(n,i)} \varphi(d) \\
&= \sum_{d|n} \sum_{i=1}^{\frac n d} \varphi(d) \\
&= \sum_{d|n} \varphi(d) \frac n d
\end{aligned}
$$

#### 法二

根据欧拉函数的定义式

$$
\varphi(n) = \sum_{i=1}^n [\gcd(n,i) = 1]
$$

PS：$\varphi(n)$表示$1$~$n-1$内与$n$互质的数，将和式上界提升到$n$不但不会影响正确性（$\gcd(n,n) = n \neq 1$），而且让$\varphi(1)$不用特判。

易得

$$
\begin{aligned}
\sum_{i=1}^n \gcd(n,i) &= \sum_{d|n} d \sum_{i=1}^n [\gcd(n,i) = d] \\
&= \sum_{d|n} d \sum_{i=1}^{\frac n d} [\gcd(\frac n d,i) = 1] \\
&= \sum_{d|n} d \varphi(\frac n d) \\
&= \sum_{d|n} \varphi(d) \frac n d \\
\end{aligned}
$$

这一步还是比较简单的。~~稍有基础的同学大概都会吧~~

### 第二步

令

$$
g(n) = \sum_{i=1}^n \gcd(n,i) = \sum_{d|n} \varphi(d) \frac{n}{d}
$$

我们希望求$g$的在$n$的函数值。容易发现右式是狄利克雷卷积$\varphi * Id$，也就是说$g$也是积性函数。所以考虑质因数分解$n$，最后用积性累乘出来

即

$$
g(n) = g({p_1}^{c_1}) g({p_2}^{c_2}) ... g({p_n}^{c_n})
$$

则只需求$g(p^c)$（这里省略下标）

$p^c$的因数分别为$1$，$p$，$p^2$，...，$p^c$

所以有

$$
\begin{aligned}
g(p^c) &= \sum_{i=0}^{c} \varphi(p^i) \frac{p^c}{p^i} \\
&= \sum_{i=0}^{c} \varphi(p^i) p^{c-i}
\end{aligned}
$$

#### 求$\varphi(p^c)$

考虑先弄出上式中$\varphi(p^i)$的封闭形式，再带回原式看看

根据欧拉函数通式

$$
\varphi(n) = n \prod_{i=1}^k (1 - \frac 1 {p_i})
$$

（这个$\pi$指的是分解质因数）

易得

$$
\begin{aligned}
\varphi(p^c) &= p^c (1 - \frac 1 p) \\
&= p^c - p^{c-1}
\end{aligned}
$$

注意这个式子需要在$c=0$时特判，因为$\varphi(1) = 1$（$1$可以视作分解不出任何质因数）

#### 求$g(p^c)$

得到了$\varphi(p^c)$，带回之前未推完的$g(p^c)$的式子，得

$$
\begin{aligned}
g(p^c) &= \sum_{i=0}^{c} \varphi(p^i) p^{c-i} \\
&= p^c + \sum_{i=1}^{c} (p^i - p^{i-1}) p^{c-i} \\
&= p^c + \sum_{i=1}^{c} (p^c - p^{c-1}) \\
&= p^c + c (p^c - p^{c-1}) \\
&= (c+1)p^c - c \ p^{c-1}
\end{aligned}
$$

（中途对$i=0$进行了特殊讨论）（该式同样不适用于$c=0$的情况）

然后积性合并起来就完了

冷静分析一波时间复杂度。质因数分解消耗$O(\sqrt n)$的时间复杂度，分解出不超过$O(log_2 n)$个$p^c$，每个$g(p^c)$的计算是$O(1)$的。所以总时间复杂度为$O(\sqrt n)$

### 代码

非常简单的代码

```c++
#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<ctime>
#include<cstdlib>
#include<algorithm>
using namespace std;
typedef long long ll;

ll p[1005],c[1005],g[1005];ll kN;
void Div(ll n){
    kN=0;
    for(ll i=2;i*i<=n;i++){
        if(n%i==0){
            kN++;p[kN]=i;
            g[kN]=1;
            ll e=0;while(n%i==0) e++,n/=i,g[kN]*=i;
            c[kN]=e;
        }
    }
    if(n!=1) kN++,p[kN]=n,c[kN]=1,g[kN]=n;
}
ll N;
int main(){
    cin>>N;
    Div(N);
    ll pdt=1;
    for(int i=1;i<=kN;i++) pdt=pdt*((c[i]+1)*g[i]-c[i]*g[i]/p[i]);
    cout<<pdt;
    return 0;
}
```

~~这式子长得跟[小粉兔菊苣的题解](https://www.luogu.org/blog/PinkRabbit/solution-p2303)很像？~~

## 更多思考

~~坐车时无聊在草稿纸上瞎搞出来的~~

### 拓展到莫比乌斯函数

第一步化完后，我们得到这样一个函数

$$
g(n) = \sum_{d|n} \varphi(d) \frac{n}{d}
$$

然后我们用质因数分解弄出了一个求它单点函数值的方法

可不可以把它拓展到莫比乌斯函数上呢？

$$
g(n) = \sum_{d|n} \mu(d) \frac{n}{d}
$$

直接仿照上面化$\varphi$的方法来

根据莫比乌斯函数定义，易得

$$
\mu(p^c) = -[c=1]
$$

同样需要特判$c=0$的情况

带回得

$$
\begin{aligned}
g(p^c) &= \sum_{i=0}^{c} \mu(p^i) p^{c-i} \\
&= p^c + \sum_{i=1}^{c} -[i=1] p^{c-i} \\
&= p^c - p^{c-1}
\end{aligned}
$$

（该式同样不适用于$c=0$的情况）

挺简洁的对吧（

### 小小的总结

总结一下，首先我们发现要求的$g(n) = \sum_{d|n} f(d) \frac{n}{d}$是积性函数，所以考虑分解质因数，简化枚举因数的过程为$g(p^c) = \sum_{i=0}^{c} f(p^i) p^{c-i}$。我们分别根据$\varphi$和$\mu$的特殊性质，化出了它们在$p^c$的函数值，然后代回化简得出$g(p^c)$的封闭形式，最后用积性合并起来，就得到了$g(n)$

仔细思考一下$\varphi$和$\mu$的特殊性质。

$\varphi(p^i) = p^i - p^{i-1}$，而带回后与$p^{c-i}$刚好抵消掉了枚举的变量$i$，从而得出封闭形式。也就是说，$\varphi$可以这么化是因为待求函数$g$比较特殊，它卷了个$Id$，$\frac n d$发挥了抵消作用。

$\mu(p^c) = [c=1]$，只有在$c=0$或$c=1$时函数非$0$，而这也就把和式简化为仅将$i=0$和$i=1$两项相加。可见$\mu$并没有用到$\frac n d$的特殊性质，对于狄利克雷卷积是通用的，常用于分解质因数后的处理。比如这道题：[洛谷P4464 [国家集训队]JZPKIL](https://www.luogu.org/problem/P4464)

### 莫比乌斯函数与欧拉函数的相互关系

第一步我们在做什么？

$$
\sum_{i=1}^n \gcd(n,i) = \sum_{d|n} \varphi(d) \frac{n}{d}
$$

那我同样考虑把它变到莫比乌斯函数上。

思考化该式时用到过的欧拉函数和式，联系到莫比乌斯函数的和式

$$
[n=1] = \sum_{d|n} \mu(d)
$$

猜想

$$
\sum_{i=1}^n [\gcd(n,i)=1] = \sum_{d|n} \mu(d) \frac{n}{d}
$$

证明很容易。

$$
\begin{aligned}
\sum_{i=1}^n [\gcd(n,i)=1] &= \sum_{i=1}^n \sum_{d|\gcd(n,i)} \mu(d) \\
&= \sum_{d|n} \mu(d) \frac n d \\
\end{aligned}
$$

然后你仔细看看左式，这不就是欧拉函数的定义式吗

于是我们找到了一个极其简洁地描述了$\mu$和$\varphi$关联的公式

$$
\varphi(n) = \sum_{d|n} \mu(d) \frac{n}{d}
$$

将本式简单变形就得到了一个更常见的表现形式

$$
\frac {\varphi(n)} n  = \sum_{d|n} \frac{\mu(d)}{d}
$$

额，不过这式子好像也没啥用，~~至少我没见过要用这个的题~~

### upd 2019/11/04 用狄利克雷卷积证明

突然发现上式可以用狄利克雷卷积非常容易的证明

$$
\begin{aligned}
Id &= \varphi * I \\
Id * \mu &= \varphi * I * \mu \\
&= \varphi * \varepsilon \\
&= \varphi
\end{aligned}
$$