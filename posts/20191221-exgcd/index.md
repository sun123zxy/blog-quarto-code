---
title: "扩展欧几里得算法（ExGCD）"
date: 2019/12/21
date-modified: 2019/12/21
categories:
  - algorithm
  - math
  
format:
  html: default
---

## 瞎扯

ExGCD用于求解不定方程

$$
ax + by = c
$$

的一组特解。常用于求解同余方程，比如求模非质数意义下的逆元。

## 推导

### 主体

首先，不定方程有解的充分必要条件由裴蜀定理给出

$$
\gcd(a,b) | c
$$

于是，我们只需关注

$$
ax + by = \gcd(a,b)
$$

的解。（原方程的解只需分别对$x$，$y$乘上$\frac{c}{gcd(a,b)}$即可求出）

（下文中%代指取模操作）

考虑递归的求解。假设我们已经知道了不定方程

$$
b x + (a \% b) y = \gcd(b, a\% b)
$$

的解$x_1$，$y_1$，即

$$
b x_1 + (a \% b) y_1 = \gcd(b, a\% b)
$$

现在尝试利用此式构造出原方程的解$x$，$y$。

由 $\gcd(a,b) = \gcd(b,a\%b)$ 以及 $a \% b = a- \lfloor \frac{a}{b} \rfloor b$

原式化为

$$
b x_1 + (a- \lfloor \frac{a}{b} \rfloor b) y_1 = \gcd(a,b)
$$

拆开括号

$$
b x_1 + a y_1 - \lfloor \frac a b \rfloor b y_1 = \gcd(a,b)
$$

我们的目标是构造出系数为$a$，$b$的原方程，故整理得

$$
a y_1 + b(x_1 - \lfloor \frac a b \rfloor y_1) = \gcd(a,b)
$$

成功构造。故

$$
\begin{aligned}
x &= y_1 \\
y &= x_1 - \lfloor \frac a b \rfloor y_1
\end{aligned}
$$

该递归的边界条件同欧几里得算法，当$b=0$时，方程为

$$
a x = \gcd(a,0) = a
$$

$x=1$，$y = 0$即该方程的一组特解。

### 构造最小$x$的特解

Exgcd常用于逆元求解。这时需要找到一组解，使得$x$最小（正整数范围内）。可以这样构造。

我们知道，不定方程的通解形式为

$$
\left\{
\begin{aligned}
x &= x_0 + kt\\
y &= y_0 - ku
\end{aligned}
\right.
$$

其中，$t=\frac b {\gcd(a,b)}, u=\frac a {\gcd(a,b)}$

upd：友情提示一下大家通常了解到的通解形式里面的$t=b, u=a$，但是实际上该通解形式仅在$\gcd(a,b)=1$时正确（不过如果你没有意识到这一点也没啥问题，因为仍然能求出MOD内的逆元，，，）

所以，以$x$为例，最小的正整数$x= (x_0 \% t +t) \% t$，然后将其带入不定方程解出$y=\frac {c-a*x} {b}$即可。

## Code

```c++
namespace ExGcd{
    ll x,y;
    ll ExGcd(ll a,ll b){
        ll ans;
        if(b==0){
            x=1;y=0;ans=a;
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
        //Minimize x
        ll t=b/d;
        x=(x%t+t)%t;
        y=(c-a*x)/b;
        return 1;
    }
}
//以下为求逆元
ll Inv(ll a,ll m){
    ExGcd::SolveEqu(a,m,1);
    return ExGcd::x;
}
```

板题：[洛谷P1082 同余方程](https://www.luogu.org/problemnew/show/P1082)，随便改改上面的程序即可。
