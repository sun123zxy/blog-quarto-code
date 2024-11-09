---
title: "关于矩阵乘法结合律的证明"
date: 2020/06/06
date-modified: 2021/08/13
categories:
  - algorithm
  - math
  
format:
  html: default
---

upd 2021/08/13:

搬博客时随便一看发现当时完全是在扯淡——矩阵乘法哪来交换律啊我的天...

已经修改了，误人子弟了真是抱歉...

还有，为了简便证明过程只证明了方阵的结合律，一般矩阵的结合律证明与此相似。

---

其实很naive...

证明的主要意义在于说明两种矩阵运算如有分配律，则有矩阵乘法的结合律的性质。

>若有面向矩阵的二元运算 $\oplus , \otimes$​​，其中 $\oplus$​ 满足交换律，并且有 $\otimes$​ 对 $\oplus$​​ 的左、右分配律，即 
>$$
>\begin{aligned}
>a \otimes ( b \oplus c ) = a \otimes b \oplus a \otimes c \\
>( b \oplus c ) \otimes a = b \otimes a \oplus c \otimes a
>\end{aligned}
>$$
>据此定义矩阵乘法 $A * B = C$ ，即
>$$
>C_{i,j} = \bigoplus  _{k=1}^n A_{i,k} \otimes B_{k,j}
>$$
>（ $A,B,C$ 为矩阵，用 $A_{i,j}$ 表示矩阵 $A$ 中第 $i$ 行第 $j$ 列的元素）
>
>则矩阵乘法具有结合律：
>$$
>(A*B)*C = A*(B*C)
>$$

证明：


$$
\begin{aligned}
( ( A*B ) *C ) _{i,j} 
&= \bigoplus_{k=1}^{n} (A*B)_{i,k} \otimes C_{k,j} \\
&= \bigoplus_{k=1}^{n} (\bigoplus_{l=1}^n A_{i,l} \otimes B_{l,k}) \otimes C_{k,j} \\
&= \bigoplus_{k=1}^{n} \bigoplus_{l=1}^n A_{i,l} \otimes B_{l,k} \otimes C_{k,j}  \quad &\text{...分配律} \\
&= \bigoplus_{l=1}^{n} \bigoplus_{k=1}^n A_{i,l} \otimes B_{l,k} \otimes C_{k,j}  \quad &\text{...交换律更换枚举} \\
&= \bigoplus_{l=1}^{n} A_{i,l} \otimes ( \bigoplus_{k=1}^n B_{l,k} \otimes C_{k,j} )  \quad &\text{...分配律} \\
&= \bigoplus_{l=1}^{n} A_{i,l} \otimes ({B*C})_{l,j} \\
&= (A*(B*C))_{i,j}
\end{aligned}
$$

