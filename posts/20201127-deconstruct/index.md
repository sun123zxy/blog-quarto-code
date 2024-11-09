---
title: "树的解构 题解"
abstract: "一道并不是特别难但没有切掉的期望题。"
date: 2020/11/27
date-modified: 2020/11/27
categories:
  - algorithm
  - probability
  - math
  - solution
  
format:
  html: default
---

[bsoj7076](https://oj.bashu.com.cn/code/problempage.php?problem_id=7076)，没找到出处...

>树的解构
>
>Mivik 喜欢 Eprom 的解构俱乐部，于是他想解构一棵树。
>
>Mivik 找到了一棵以 $1$ 为根的有 $n$ 个结点的有根外向树。Mivik 会进行 $(n−1)$  次操作，每次 Mivik 都会从未删掉的边中等概率选择一条边将其删去。记这条边为 $a \to b$ ，则删去这条边的代价是删边时 $b$ 的子树大小（包括 $b$ 自己）；删去这条边后 $b$ 为根的子树会形成一棵新的以 $b$ 为根的有根树。
>
>Mivik 想知道，他进行这 $(n−1)$ 次操作后期望的代价总和是多少。由于 Mivik 不喜欢太大的数，你只需要输出期望的值对 $10^9 + 7$ 取模的结果。
>
>对于所有测试点，满足 $1 \le n \le 2 \times 10^6$ 。保证给出的有根树合法。

一道并不是特别难但没有切掉的期望题。

考场写了链过后就一直在想怎么拆树，无果而终。然后想到解决排列问题的一个常见的思路是考虑所有排列方案的贡献和，最后乘个 $\frac{1}{(n-1)!}$ 就可以了。尝试在树形dp边的排列顺序，同样无果而终。

对整体dp不好做，那就转而思考每个点对答案的贡献。

考虑从树上某个深度（到根需经过的边数）为 $d_u$ 的点 $u$ ，其向上到根的路径上各边的相对删除顺序可以用一个阶为 $d_u$ 的排列 $P$ 表示。若只考虑该路径，设此时对答案的贡献为 $f_u(P)$ ，我们尝试化简 $\sum_{P \in \mathrm{perm(d_u)}} f_u(P)$ 。（其中 $\mathrm{perm(n)}$ 表示所有 $n$ 阶排列的集合）

对于其中某条位置为 $i$ 的边，若其被删除时点 $u$ 可对答案产生贡献，则必须保证删除该边时该边与 $u$ 仍然连通，也就是对于任意位置为 $j$ 且 $j < i$ 的的边，有 $P_j > P_i$ 。换句话说，若从后往前 （从上至下）决定边的删除时间，那么一条删除时间为 $x$ 的边必须在 $[1,x]$ 中最后被选中才能满足上述条件。显然其发生概率为 $\frac{1}{x}$ ，故在所有排列中 $x$ 产生贡献的次数为 $\frac{d_u !}{x}$ 。因此
$$
\sum_{P \in \mathrm{perm(d_u)}} f_u(P) = \sum_{x=1}^{d_u} \frac{d_u!}{x}
$$
$P$ 的相对顺序已经确定，而在整棵树中还有其他边的相对顺序未被决定，因此点 $u$ 的总贡献还要再乘上对 $P$ 消序后的全部排列方案数
$$
\frac{(n-1)!}{d_u!} \sum_{x=1}^{d_u} \frac{d_u!}{x} = (n-1)! \sum_{x=1}^{d_u} \frac{1}{x}
$$
整合所有点的贡献并乘上概率就是最终的期望
$$
\begin{aligned} \mathrm{ans} &= \frac{1}{(n-1)!} \sum_{u=1}^n (n-1)! \sum_{x=1}^{d_u} \frac{1}{x} \\ &= \sum_{u=1}^n \sum_{x=1}^{d_u} \frac{1}{x} \end{aligned}
$$
~~意外的约掉了好多玩意儿呢~~

线性求逆元并前缀和即可 $O(n)$ 解决。

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
ll Rd(){
	int ans=0;bool fh=0;char c=getchar();
	while(c<'0'||c>'9'){if(c=='-') fh=1; c=getchar();}
	while(c>='0'&&c<='9') ans=ans*10+c-'0',c=getchar();
	if(fh) ans=-ans;
	return ans;
}
typedef vector<ll> Vec;
typedef vector<ll>::iterator IVec;
#define foreach(it,v) for(IVec it=v.begin();it!=v.end();it++)
const ll MOD=1E9+7;
#define _ %MOD
ll PMod(ll x){
	if(x<0) return x+MOD;
	else if(x>=MOD) return x-=MOD;
	else return x;
}

const ll MXN=2E6+5;
ll N;Vec chd[MXN];

ll dep[MXN];
void InfoDFS(ll u,ll depth){
	dep[u]=depth;
	foreach(it,chd[u]) InfoDFS((*it),depth+1); 
}

ll inv[MXN],sInv[MXN];
void SpawnInv(){
	inv[1]=1;for(ll i=2;i<=N;i++) inv[i]=PMod(-(MOD/i)*inv[MOD%i]_); 
	sInv[1]=inv[1];for(ll i=2;i<=N;i++) sInv[i]=PMod(sInv[i-1]+inv[i]);
}
void Solve(){
	SpawnInv(); 
	InfoDFS(1,0);
	ll ans=0;
	for(ll u=1;u<=N;u++) ans=PMod(ans+sInv[dep[u]]);
	printf("%lld",ans); 
}
int main(){
	//freopen("deconstruct.in","r",stdin);
	//freopen("deconstruct.out","w",stdout);
	N=Rd();
	for(ll i=2;i<=N;i++) chd[Rd()].push_back(i);
	Solve();
	return 0;
}
```

