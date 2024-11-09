---
title: "关于泰勒展开拉格朗日余项中值点的渐进性"
abstract: "中科大《数学分析教程》第三版问题 4.3.1"
date: 2022/11/02
date-modified: 2022/11/02
categories:
  - math
  - analysis
  - solution
  
format:
  html: default
---

之前学拉格朗日中值定理的时候做到一道涉及到特定函数中值渐进性的题，感觉似乎有一般的结论，推广了一下就是这样了。

感谢刘导拯救 $n=1$ 都不会证的我，感谢王佬指出这是中科大《数学分析教程》第三版问题 4.3.1。

> 设函数 $f(x)$ 在区间 $I$ 上有 $n+1$ 阶导数，$x_0 \in I$，$f^{(n+1)}(x_0) \not = 0$；定义函数 $T_n(x)$ 为 $f(x)$ 在 $x=x_0$ 处的 $n$ 阶泰勒多项式：
> 
> $$
> T(x) = T_n(x_0+h) = \sum_{k=0}^n \frac{f^{(k)}(x_0)}{k!} h^k
> $$
> 
> 求证： $f(x)$ 在 $x=x_0$ 处带拉格朗日余项的 $n-1$ 阶泰勒展开
> 
> $$
> f(x) = f(x_0+h) = T_{n-1}(x_0+h) + \frac{f^{(n)}(x_0+\theta h)}{n!} h^n \qquad \theta \in (0,1)
> $$
> 
> 中的 $\theta$ 满足：
> 
> $$
> \lim_{h \to 0} \theta = \frac 1 {n+1}
> $$

非常漂亮的结论啊。

理一下思路。关键点在于怎么构造出 $\theta$，得想办法把它从 $f^{(n)}(x_0+\theta h)$ 这样的形式中拿出来。或许我们能想到构造

$$
\frac{f^{(n)}(x_0+\theta h) - f^{(n)}(x_0)}{\theta h} \to f^{(n+1)}(x_0) \qquad (h \to 0)
$$

这样 $\theta$ 就可以拿出来单独求极限了。$f^{(n)}(x_0+\theta h)$ 可以从 $f(x)$ 的 $n-1$ 阶泰勒展开的拉格朗日余项中得到，而 $f^{(n)}(x_0)$ 和 $f^{(n+1)}(x_0)$ 也可以从 $f(x)$ 的 $n+1$ 阶带皮亚诺余项的泰勒展开中拿到。于是，下面的证明就十分自然了。

**证明：** 考虑 $f(x)$ 的 $n-1$ 阶带拉格朗日余项的泰勒展开

$$
f(x_0 + h) = T_{n-1}(x_0 + h) + \frac{f^{(n)}(x_0+\theta h)}{n!} h^n
$$

和 $f(x)$ 的 $n+1$ 阶带皮亚诺余项的泰勒展开

$$
f(x_0 + h) = T_{n+1}(x_0 + h) + o(h^{n+1}) = T_{n-1}(x_0 + h) + \frac{f^{(n)}(x_0)}{n!} h^n + \frac{f^{(n+1)}(x_0)}{(n+1)!} h^{n+1} + o(h^{n+1})
$$

两式相减得

$$
\frac{f^{(n)}(x_0+\theta h)}{n!} h^n = \frac{f^{(n)}(x_0)}{n!} h^n + \frac{f^{(n+1)}(x_0)}{(n+1)!} h^{n+1} + o(h^{n+1})
$$

移项整理得

$$
\theta \cdot \frac{f^{(n)}(x_0+\theta h)-f^{(n)}(x_0)}{\theta h} = \frac{f^{(n+1)}(x_0)}{n+1} + n! \cdot \frac {o(h^{n+1})}{h^{n+1}}
$$

两侧同时取 $h \to 0$ 即得

$$
\lim_{h \to 0} \theta \cdot f^{(n+1)}(x_0) = \frac{f^{(n+1)}(x_0)}{n+1}
$$

于是由 $f^{(n+1)}(x_0) \not = 0$，我们有

$$
\lim_{h \to 0} \theta = \frac{1}{n+1}
$$

得证。

---

备考：

```
Azpeitia A G. On the Lagrange remainder of the Taylor formula[J]. The American Mathematical Monthly, 1982, 89(5): 311-312.
Abel U. On the Lagrange remainder of the Taylor formula[J]. The American mathematical monthly, 2003, 110(7): 627-633.
```

这两篇论文似乎对这种渐进性有更深入的讨论。