# 有限覆盖定理与实数理论
sun123zxy
2022-12-13

## Example 1

为更好的证明本题，先引入 Bolzano-Weierstrass 定理的一种等价表述.

<div id="def-accu" class="theorem definition">

<span class="theorem-title">**Definition 1 (数列聚点)**</span> 对任意实数列 $\{ x_n \}$，若实数 $a$ 满足：对 $a$ 的任意小邻域 $U(a, \varepsilon) = (a - \varepsilon, a + \varepsilon)$，都有无穷个 $x_n$ 满足 $x_n \in U(a, \varepsilon)$，则称 $a$ 是数列 $\{ x_n \}$ 的一个聚点.

</div>

请注意，这里我们没有使用建立在集合之上的标准的聚点定义. 对数列单独定义聚点，是考虑到数列允许重复的元素出现，如此定义更容易展开后面的讨论.

<div id="thm-accuequiv" class="theorem">

<span class="theorem-title">**Theorem 1**</span> 实数 $a$ 是数列 $\{ x_n \} \subset \mathbb R$ 的聚点的充要条件是：存在 $\{ x_n \}$ 的一个收敛子列 $\{ x_{n_k} \}$，其极限为 $a$.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span>先证充分性. 用定义写开 $\lim_{k \to \infty} \{ x_{n_k} \} = a$，就有
$$
(\forall \varepsilon > 0)(\exists K \in \mathbb N_+)(\forall k > K)(|x_{n_k} - a| < \varepsilon)
$$
故确有无穷项 $x_n$ 落在任意小的 $U(a, \varepsilon)$ 中，即 $a$ 是 $\{ x_n \}$ 的一个聚点，该方向得证.

下证必要性. 已知 $\{ x_n \}$ 有一聚点 $a$. 我们按如下方法构造子列 $\{ x_{n_k} \}$：

1.  当 $k=1$，取 $\varepsilon_1 = 1$，因为 $a$ 是 $E$ 的一个聚点，$\exists n_1 \in \mathbb N_+$，$|x_{n_1} - a| < \varepsilon$.
2.  当 $k \geqslant 2$，取 $\varepsilon_k = \frac 1 k$，因为 $a$ 是 $E$ 的一个聚点， $\exists n_k > n_{k-1}$，$|x_{n_k} - a| < \varepsilon$.

这样，我们构造出 $\{ x_n \}$ 的一个子列 $\{ x_{n_k} \}$ 满足 $|x_{n_k} - a| < \varepsilon_k = \frac 1 k$. 因此其收敛于 $a$，该方向得证.

</div>

上述定理立刻证明了下定理与 Bolzano-Weierstrass 定理的等价性.

<div id="thm-bwequiv" class="theorem">

<span class="theorem-title">**Theorem 2 (数列聚点定理)**</span> 任意有界实数列 $\{ x_n \}$ 至少有一个聚点.

</div>

<div id="exm-finite2bw" class="theorem example">

<span class="theorem-title">**Example 1**</span> 利用有限覆盖定理证明 Bolzano-Weierstrass 定理.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span>命题等价于用有限覆盖定理证明数列聚点定理. 用反证法. 假设一有界实数列 $\{ x_n \}$ 不存在聚点，设其有上界 $L$ 和下界 $l$. 对任意 $a \in [l,L]$，它都不是 $\{ x_n \}$ 的聚点，因此总存在一个 $\varepsilon(a) > 0$，使得只有有限个 $x_n$ 落入 $U(a, \varepsilon(x_0))$. 这样，构造开区间族
$$
\mathscr{F} = \{ U(a, \varepsilon(a)) \mid a \in [l,L] \}
$$
它显然是闭区间 $[l,L]$ 的一个开覆盖. 由有限覆盖定理，只需取其中有限个开区间就可以覆盖住 $[l,L]$，因此覆盖 $\{ x_n \}$ 也只需要有限个开区间. 然而由前述构造，每一个开区间中也只包含有限个 $x_n$，因此数列 $\{ x_n \}$ 只有有限项——这显然是荒谬的. 故 $\{ x_n \}$ 必有聚点，原命题得证.

</div>

## Example 2

<div id="thm-lebesgue" class="theorem">

<span class="theorem-title">**Theorem 3 (Lebesgue 覆盖定理)**</span> 设开区间族 $\mathscr F$ 是闭区间 $[a, b]$ 的一个开覆盖，则必存在 $\sigma > 0$，使得只要区间 $\Omega \subset [a, b]$ 且 $\Omega$ 的长度 $|\Omega| < \sigma$，就必有 $\mathscr F$ 中的一个开区间包含 $\Omega$. 其中 $\sigma$ 称为 Lebesgue 数.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span>不妨只证 $\Omega$ 是闭区间这种最强的情况.

用反证法. 假设命题不成立，则对任意 $\sigma > 0$，都存在一个长度小于 $\sigma$ 的闭区间 $\Omega \subset [a,b]$，它不被任何 $\mathscr F$ 中的开区间包含. 因此，对所有自然数 $n$，可取 $\sigma_n = \frac 1 n$，按上述方法就可构造出一列闭区间
$$\{ \Omega_n \} = \{ [a_n, b_n] \} \subset [a,b]$$
其中每一个闭区间都不被任何 $\mathscr F$ 中的开区间包含，且区间长度 $|\Omega_n| < \sigma_n = \frac 1 n$，即 $\lim_{n \to \infty} |\Omega_n| = 0$.

因为 $\Omega_n \subset [a,b]$，$\{ a_n \}$ 有界，由 Bolzano-Weierstrass 定理，其存在一收敛子列 $\{ a_{n_k} \}$，设其极限为 $x_0$，极限保序性表明 $x_0 \in [a,b]$. 注意到 $b_{n_k} = a_{n_k} + |\Omega_{n_k}|$，两端取 $k \to \infty$ 即得 $\lim_{k \to \infty} b_{n_k} = x_0$. 综上，我们说明了 $\{ \Omega_{n_k} \}$ 收缩于 $x_0$.

但，因为 $\mathscr F$ 是闭区间 $[a,b]$ 的一个开覆盖，故总存在一个开区间 $I_{x_0} = (a_0, b_0) \in \mathscr F$ 使得 $x_0 \in I_{x_0}$，而 $\{ \Omega_{n_k} \}$ 又收缩于 $x_0$，故存在 $k \in \mathbb N$，$\Omega_{n_k} \subset I_{x_0} \in \mathscr F$，这与我们构造 $\{ \Omega_{n_k} \}$ 的方法矛盾. 故原命题成立.

</div>

<div class="proof remark">

<span class="proof-title">*Remark*. </span>证明过程与用 Bolzano-Weierstrass 定理证明闭区间一致连续性定理类似.

</div>

<div id="exm-lebesgue2bw" class="theorem example">

<span class="theorem-title">**Example 2**</span> 用 Lebesgue 覆盖定理证明有限覆盖定理.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span>若 $\sigma$ 是覆盖 $[a,b]$ 的开覆盖 $\mathscr F$ 的勒贝格数，令 $N = \lceil \frac{2(b-a)}{\sigma} \rceil$，$L = \frac {b-a}{N} \leqslant \frac \sigma 2 < \sigma$. 由 Lebesgue 覆盖定理，任意长度为 $L$ 的 $[a,b]$ 内闭区间都包含于某个 $\mathscr F$ 中的开区间. 因此对 $n = 1,2,\dots,N$，令 $\Omega_n = \left[ a + (n-1)L, a + nL \right]$，总存在一个 $\mathscr F$ 中开区间 $I_n$ 满足 $\Omega_n \subset I_n$. 因为显然 $\{ \Omega_n \}_{n=1}^N$ 是 $[a,b]$ 的一个覆盖，故 $\{ I_n \}_{n=1}^N \subset \mathscr F$ 也是 $[a,b]$ 的覆盖. 这样，我们就成功构造出了一个 $\mathscr F$ 的有限子覆盖 $\{ I_n \}_{n=1}^N$，命题得证.

</div>

<div class="proof remark">

<span class="proof-title">*Remark*. </span>在处理区间问题时，Lebsegue 覆盖定理很多时候比有限覆盖定理更好用. 例如处理闭区间一致连续性定理时，“落入两个有交点的相邻的这样的区间”(李忠 and 方丽萍 2008)这种神乎其技的操作就可以省去了. （这种操作的严格叙述是：把单点连续性要求的区间半径改成 $2 \delta_x$，但构造开覆盖还是用半径为 $\delta_x$ 的区间，这样可以有 $|x_2 - x| \leqslant |x_2 - x_1| + |x_1 - x| \leqslant \delta_{m} + \delta_{x} \leqslant 2 \delta_{x}$，能把两个点放在同一个区间内了. 详细可参考 <https://www.zhihu.com/question/56393706/answer/298562084>. 事实上，Lebesgue 覆盖定理也可以用类似的方法证明.）

</div>

作为小结，下图展现了刚刚介绍的几个定理在整个实数完备性等价定理体系中的地位.

<div>

<img src="index_files\figure-commonmark\dot-figure-1.png" style="width:6.25in;height:2.5in" />

</div>

## Example 3

<div id="thm-ulaccu" class="theorem">

<span class="theorem-title">**Theorem 4**</span> 设数列 $\{ x_n \}$ 有界，其上极限 $\varlimsup_{n \to \infty} x_n = L$，下极限 $\varliminf_{n \to \infty} x_n = l$，则 $L$ 是 $\{ x_n \}$ 的最大聚点，$l$ 是 $\{ x_n \}$ 的最小聚点.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span><a href="#thm-accuequiv" class="quarto-xref">Theorem 1</a> 中，我们已经知道，一个数列的收敛子列的极限也是该数列的一个聚点. 结合上下极限的子列式定义即可证明上述定理.

</div>

<div id="exm-ullim" class="theorem example">

<span class="theorem-title">**Example 3**</span> 设数列 $\{ x_n \}$ 有界且 $\lim_{n \to \infty}(x_{n+1} − x_n) = 0$，分别记 $\{ x_n \}$ 的上下极限为 $L$ 和 $l$. 证明 $[l, L]$ 上的任意点可作为 $\{ x_n \}$ 某个子列的极限.

</div>

<div class="proof">

<span class="proof-title">*Proof*. </span>反证. 假设 $[l, L]$ 上有一点 $a$ 不是任何 $\{ x_n \}$ 的收敛子列的极限，则根据 <a href="#thm-accuequiv" class="quarto-xref">Theorem 1</a>，$a$ 不是 $\{ x_n \}$ 的聚点，即存在 $a$ 的一个邻域 $U(a,\varepsilon)$，使得只有有限个 $x_n$ 落入该邻域，换句话说，存在某个 $N \in \mathbb N_+$，当 $n>N$ 时，就有 $x_n \notin U(a,\varepsilon)$.

又，考虑到 <a href="#thm-ulaccu" class="quarto-xref">Theorem 4</a> 表明上下极限 $L$ 和 $l$ 都是 $\{ x_n \}$ 的聚点，$L,l \notin U(a,\varepsilon)$ 显然成立，且第 $N$ 项后的 $\{ x_n \}$ 完全由满足 $x_n > a+\varepsilon$ 和 $x_n < a-\varepsilon$ 的两种 $x_n$ 构成，且它们均有无穷多项. 这样，对于任意的 $M>N$，总可以找到一个 $m > M$ 使得 $x_m$ 和 $x_{m+1}$ 分属 $U(a, \varepsilon)$ 的两侧，故 $|x_m - x_{m+1}| \geqslant 2 \varepsilon$，这就与条件 $\lim_{n \to \infty}(x_{n+1} − x_n) = 0$ 产生矛盾. 故不存在这样的 $a$，定理得证.

</div>

## Acknowledgments

感谢史老师主持研讨课并指出讲稿的多处错误，特别是原来聚点定理的证明中数列元素可重的 bug. 史老师还提供了标准聚点定义的另一种叙述.

<div id="refs" class="references csl-bib-body hanging-indent" entry-spacing="0">

<div id="ref-lizhong" class="csl-entry">

李忠, and 方丽萍. 2008. *数学分析教程*. 北京: 高等教育出版社.

</div>

</div>
