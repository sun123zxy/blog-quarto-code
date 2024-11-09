---
title: "CodeChef-LECOINS Little Elephant and Colored Coins 题解"
date: 2020/11/18
date-modified: 2020/11/18
categories:
  - algorithm
  - solution
  
format:
  html: default
---

[CodeChef-LECOINS Little Elephant and Colored Coins](https://www.codechef.com/problems/LECOINS)

>**Little Elephant and Colored Coins**
>
>The Little Elephant from the Zoo of Lviv very likes coins. But most of all he likes colored coins.
>
>He has **N** types of coins, numbered from **1** to **N**, inclusive. The coin of the **i**-th type has the value **Vi** dollars and the color **Ci**. Note that he has infinite supply of each type of coins.
>
>The Little Elephant wants to make exactly **S** dollars using the coins. What is the maximal number of different colors he can use to make exactly **S** dollars using some of the coins he has? If it's impossible, output **-1**. Also note that the Little Elephant wants to know this for many values of **S**.
>
>**Input**
>
>The first line of the input contains a single integer **N**, denoting the number of types of coins. Each of the following **N** lines contains two space-separated integers **Vi** and **Ci**, denoting the value and the color of the coin of the **i**-th type. The next line contains a single integer **Q**, denoting the number of values of **S** to process. Each of the following **Q** lines contains a single integer **S**, denoting the coinage you should represent via given coins using maximum number of colors.
>
>**Output**
>
>For each value of **S** in the input, output the maximum number of different colors in the representation of **S** or **-1** if it is impossible to represent **S** via given coins.
>
>**Constraints**
>
>- **1** ≤ **N** ≤ **30**
>- **1** ≤ **Vi** ≤ **200000** (**2 \* 10^5**)
>- **1** ≤ **Ci** ≤ **10^9**
>- **1** ≤ **Q** ≤ **200000** (**2 \* 10^5**)
>- **1** ≤ **S** ≤ **10^18**
>
>**Example**
>
>```
>Input:
>3
>2 1
>3 4
>4 4
>4
>1
>3
>5
>7
>Output:
>-1
>1
>2
>2
>```
>
>**Explanation**
>
>- It is not possible to represent **S = 1** since every coin has value more than 1.
>- **S = 3** can only be represented using one coin of the second type, hence only one color is used in the representation.
>- **S = 5** can only be represented as **2 + 3**, which leads to two colors used.
>- For **S = 7** we have two representations as **2 + 2 + 3** (with two colors used) and **3 + 4** (with one color used). Hence, the answer is 2.

妙题啊。

## 可达性

如果有个 $C_i \le 1$ 的SubTask的话会更容易想到正解。

我们先不考虑颜色，只考虑是否能找到一种方案把硬币总面值凑成 $S$ 。

有一种非常朴素的想法是从 $0$ 到 $S$ 做可达性dp： $f_{x+V_i} = f_{x+V_i} | f_x$ ，不过因为 $S \le 10^{18}$ ，显然这是天方夜谭。

可达性dp只有 $0/1$ 两种状态，非常浪费；发现 $V_i$ 的值域很小，这启示我们将 $V_i$ 作为状态减少有效状态数。但究竟如何设置呢？

这里是最巧妙的地方。不失一般性，设 $V_1 = \min V_i$ 。我们尝试把 $V_1$ 孤立出来考虑。 **假设**保证过程中最终选择的总面额永远不会比 $S$ 大（换句话说，允许选择负数个硬币），那么只需知道硬币集合 $V - \{ V_1 \}$ 中是否存在一种方案其总面值模 $V_i$ 与 $S$ 同余即可。这样就只需对去掉 $V_1$ 的硬币集合做 $1$ 到 $V_1- 1$ 的可达性dp就好了。

考虑把前面那个假设干掉。之前提到 $f$ 只有 $0/1$ 两种状态，明显可以再塞点东西进去。于是令 $f_x$ 为硬币集合 $V - \{ V_1 \}$ 中所有总面值模 $V_1$ 等于 $x$ 的选择方案中最小的总面值。容易写出状态转移

$$
\mathrm{relax} \ f_{(x+V_i)\% V_1} \ \mathrm{by} \ f_x + V_i
$$

（ $\%$ 代表取模， $\mathrm{relax} \ A \ \mathrm{by} \ B$ 即 $A = \min(A,B)$ ）

这样一来，若 $f_{S \% V_1} \le S$ ，那么 $S$ 就是可以被组成的。 

发现这个dp就是个最短路，形成了一个 $|V| = V_1, |E| = (n-1) V_1$ 的图。于是 Dijkstra 一下就可以了。不过这个图还有更好的性质。

首先，路径中边的顺序可以任意调换而不影响可达性和最短路，所以我们可以分开考虑每一种边的松弛。而一分开看就非常明朗了，根据数论常识，所有 $x$ 到 $(x + V_i) \% m$ 的边会在图上形成 $\gcd(V_i, m)$ 个大小为 $\frac{V_1}{\gcd({V_i, m})}$ 的环，于是现在我们又可以分别考虑每一个环。只需从环上当前距离值最小的点绕环一圈就可以做到松弛了。于是现在预处理的时间复杂度是 $O(|E|) = O(nV_1)$ ，比直接 Dijkstra 少了个 $\log$ 。询问当然是 $O(1)$ 的。

## 考虑颜色

把颜色放到dp状态里面去就好了。设 $f_{x, c}$ ，$c$ 记录了选取的颜色种类数，其余与前述相同。

比较懒，不写代码了。实在需要可以看官方题解。

## 参考

[官方题解](https://discuss.codechef.com/t/lecoins-editorial/1723)

~~这篇题解基本上是官方题解的简化翻译版本~~