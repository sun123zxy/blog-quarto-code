---
title: "原创OI题目 白银之春 Problem and Solution"
date: 2020/12/06
date-modified: 2020/12/06
categories:
  - algorithm
  - original problem
  - solution
  
format:
  html: default
---

比赛用题面、题解、标程和数据生成器都挂在 [git@github.com:sun123zxy/spring.git](https://github.com/sun123zxy/spring) 上。

## Problem

>**白银之春 (spring.cpp/.in/.out) (2s,512MB)**
>
>**Background**
>
>妖梦正在收集春度！
>
>**Description**
>
>幻想乡由 $n$ 个地点和 $m$ 条单向小路组成，第 $i$ 个地点蕴含着 $s_i$ 的春度。妖梦从位于 $1$ 号节点的白玉楼出发，沿图上路径收集沿路的春度，总春度为收集到的所有春度之和。
>
>半人半灵的妖梦具有一种名叫“人妖槽”的属性，该属性有两种状态——“人类逢魔”与“妖怪逢魔”，出发时状态为“人类逢魔”。某些小路上可能被放置了“森罗结界”。在经过被放置结界的小路时，妖梦的人妖槽状态将会发生变化——若经过这条小路前人妖槽状态为“人类逢魔”，则经过后将变为“妖怪逢魔”；反之，若经过前状态为“妖怪逢魔”，则经过后将变为“人类逢魔”。当且仅当人妖槽状态为“妖怪逢魔”时，妖梦才可以收集到当前所在地点所蕴含的春度。
>
>每个点的春度只能被收集一次。妖梦可以在图上任意游走，并可以选择在任意一个地点停止收集。
>
>妖梦希望收集到的总春度最大，但她并没有学过OI，请你帮忙算出她最多能收集到多少春度。
>
>因为并非所有人都具有结界内的常识，妖梦也提供了一份题意简述 ：
>
>> 给定一个带点权普通有向图和一只具有 $0/1$ 状态的妖梦，从 $1$ 号节点出发，初始状态为 $0$ 。边有 $0/1$ 边权，经过边时状态要异或上边权。当前状态为 $1$ 时可取得所在点权，点权只能被取得一次。问在图上随意游走可获得的最大点权和。
>
>**Input**
>
>第一行四个整数 $n$ ， $m$ ，表示图由 $n$ 个点， $m$ 条边构成。
>
>接下来一行有 $n$ 个整数 $s_i$ ，表示$i$号节点蕴含 $s_i$ 的春度。
>
>接下来 $m$ 行每行 $3$ 个整数 $u_i$ ， $v_i$ ， $w_i$ ，表示有一条从 $u_i$ 到 $v_i$ 的有向边，若 $w_i = 1$ ，则表示该小路上被放置了森罗结界，若 $w_i = 0$ ，则表示未被放置。
>
>**Output**
>
>输出一行一个整数，表示妖梦能收集到的最大总春度。
>
>**Sample 1**
>
>**Sample 1 Input**
>
>```
>5 6
>99 82 44 35 3
>1 2 1
>2 3 0
>3 4 1
>4 5 0
>2 4 1
>3 5 1
>```
>
>**Sample 1 Output**
>
>```
>126
>```
>
>**Sample 1 Explanation**
>
>路径为 $1$ -> $2$ -> $3$ ，可获得 $0 \times 99 + 1 \times 82 + 1 \times 44=126$ 点春度。
>
>**Sample 2**
>
>**Sample 2 Input**
>
>```
>9 10
>9 9 8 2 4 4 3 5 3
>1 2 0
>2 3 1
>3 2 0
>3 4 0
>4 5 1
>5 6 0
>6 4 1
>2 5 0
>7 8 1
>9 8 1
>```
>
>**Sample 2 Output**
>
>```
>25
>```
>
>**Sample 2 Explanation**
>
>路径为 $1$ -> $2$ -> $3$ -> $2$ -> $5$ -> $6$ ，可以获得 $0 \times 9 + 0 \times 9 + 1 \times 8 + 1 \times 9 + 1 \times 4 + 1 \times 4= 25$ 点春度。
>
>**Sample 3**
>
>见 `sample` 目录下 `spring3.in/.ans` 。
>
>该样例是一个无环图。
>
>**Sample 4**
>
>见 `sample` 目录下 `spring4.in/.ans` 。
>
>**Constraints**
>
>对于30%的数据，保证图中无环。
>
>对于另外20%的数据，保证图随机生成。
>
>对于100%的数据， $2 \le N \le 5 *  10^5$ ， $1 \le M \le 10^6$ ， $0 \le s_i \le 10^9$ ， $1 \le u_i,v_i \le N$ ， $w_i \in \{ 0,1 \}$ 。
>
>**Hints**
>
>由于幻想乡不受常识束缚，不保证不出现重边和自环，不保证图连通。
>
>输入量较大，请使用较为快速的读入方式。
>
>保证时限在std用时的2倍左右。std没有卡常，请放心食用。
>
>**Source**
>
>sun123zxy

## Fun Facts

- [森罗结界 - 东方妖妖梦](https://thwiki.cc/%E6%A3%AE%E7%BD%97%E7%BB%93%E7%95%8C)
- [人妖槽 - 东方永夜抄](https://thwiki.cc/%E6%B8%B8%E6%88%8F%E6%94%BB%E7%95%A5/%E4%B8%9C%E6%96%B9%E6%B0%B8%E5%A4%9C%E6%8A%84/%E7%B3%BB%E7%BB%9F#%E5%9F%BA%E6%9C%AC%E4%BA%8B%E9%A1%B9)
- 题目名neta的是妖妖梦一面的卷首语。

## Solution

### 无环图

DAG上dp就好了。设状态 $f[u][0/1]$ 为到达点 $u$ 时状态为 $0/1$ 可收集到的最大春度，若 $f[u][t]$ 可达，有
$$
f[u][t] = t \times \mathrm{val}[u] + \max_{(v,w) \in \mathrm{pre}_u} f[v][t \otimes w]
$$
其中 $\mathrm{val}[u]$ 是点 $u$ 的权值， $(v,w) \in \mathrm{pre}_u$ 表示 $u$ 在DAG上的前驱边， $\otimes$ 代表异或。

答案即 $\max_{u \in G} \max(f[u][0],f[u][1])$ 。

### 普通图

普通图有环，环上的状态转移方程相互依赖，无法dp。

根据部分分的提示，考虑缩点。

不妨先看所有强连通分量都只是简单环的情况。

#### 环套DAG

为了方便描述，我们定义如下两种描述：

- 奇环：环上所有边权异或和为 $1$ 的环。
- 偶环：环上所有边权异或和为 $0$ 的环。

容易发现奇环上可以通过绕一圈的方式回到原点，使状态发生改变。也就是说，不论从进出位置和初始状态如何，一个奇环总可以输出任意的 $0$ 或 $1$ 。而如果在奇环上绕两圈，就可以取得环上所有点的春度。所以直接缩点处理即可。

那么偶环如何处理呢？

首先，若进入偶环的的位置（入点）确定，无论怎样在偶环上绕圈，到达环上某点（出点）时的状态总是唯一确定的。

进一步的，偶环上的点可根据到达该点时的状态被分为两组。组与组之间在环上交错排列，所有边权为 $1$ 的边都是都是一个间隔。若入点和出点在同一组内，则状态不会发生变化；反之则状态改变。这启发我们将偶环缩成两个点来处理，每一个点代表一个组。

考虑春度的获取。如果进入时状态为 $0$ ，那么和入点在同一组内的点上的春度都无法取得（因为经过该点时状态始终为 $0$ ），而在不同组的点上的春度能够取得（因为经过该点时状态始终为 $1$ ）；反之，若进入时状态为 $1$ ，那么和入点在同一组的点上的春度可以取得，在不同组的不能取得。

缩点后做一些讨论就可以了。

#### 强连通分量

在环上我们已经发现——奇环可以特殊处理，而偶环内的点可以被分成两组。强连通分量是否有与其相似的性质呢？

##### 奇强连通分量

强连通分量无非是许多个环叠起来的连通块。如果一个强连通分量包含一个或多个奇环（称之为“奇强连通分量”），那么该强连通分量同样有奇环的性质——每个点都可以通过在奇环上绕圈获得 $0/1$ 两种状态，块上所有点的春度都能取得。

实测发现随机图中出现偶强连通分量的概率极小，因此只处理奇强连通分量的算法可以通过随机图数据。

##### 偶强连通分量

剩下的问题已经很明确了——处理所含环全都是偶环的强连通分量（称之为“偶强连通分量”）。

可以发现这一结论：无论如何在偶强连通分量中游走，只要入点和进入时的状态确定，那么每个点的状态就唯一确定。于是偶强连通分量中的点也可以被分成两组，好比环套DAG中的偶环。

易用反证法证明该性质：在一偶强连通分量中，假设点 $u$ 到点 $v$ 同时存在偶路径 $P$ 和奇路径 $Q$ 。那么奇路径 $Q$ 必然与某条从 $v$ 到 $u$ 的奇路径 $R$ 共同组成了一个偶环（偶强连通分量中只有偶环且各点强连通）。则偶路径 $P$ 和奇路径 $R$ 构成奇环，与假设矛盾，故性质成立。

春度的获取也与偶环相同。

判断一个强连通分量是奇是偶，只需二分图染色，取环上任意一个点作为起点DFS，如果能以不同的状态到达某点，那该分量就是奇的，反之则是偶的。正确性比较显然，证明在此略去。

### 实现

实现细节较多，建议缩点后重新建图。

可以用4个节点分别代理两个分组各自的入边和出边，算出到达该组状态为 $0/1$ 时连通块内两个组的点权对答案的贡献。为了方便，实现时可以以边数x2的代价把节点数压缩到2个。

## Code

```c++
/*
白银之春 (spring) std
by sun123zxy

PS: If you got a runtime error, "-Wl,--stack=123456789"
*/
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
ll Rd(){
	ll ans=0;bool fh=0;char c=getchar();
	while(c<'0'||c>'9'){if(c=='-') fh=1; c=getchar();}
	while(c>='0'&&c<='9') ans=ans*10+c-'0',c=getchar();
	if(fh) ans=-ans;
	return ans;
}

const ll INF=1E18;

const ll PTN=1E6+5,EDN=2E6+5;
ll N;
struct Edge{ll u,v;bool w;ll nxt;};
struct Graph{
	Edge edge[EDN];
	ll graM,last[PTN];
	void GraphInit(){graM=0;for(ll i=0;i<PTN;i++) last[i]=0;}
	void AddBscEdge(ll u,ll v,bool w){
		edge[++graM]=(Edge){u,v,w,last[u]};
		last[u]=graM;
	}
	void AddUnEdge(ll u,ll v,bool w){
		AddBscEdge(v,u,w); 
	}
	ll ptW[PTN][2]; //value Youmu can get when reaching the vertex with state 0/1
}G1,G2;
ll Id(ll cId,bool col){
	return 2*cId-col;
}

ll bel[PTN],cN,rps[PTN]; //belong, number of components, representative vertax of the component
ll dfn[PTN],low[PTN],dN;
ll stk[PTN],tp;bool isI[PTN];
void Tarjan(ll u){
	dfn[u]=low[u]=++dN;
	stk[++tp]=u;isI[u]=1;
	for(ll i=G1.last[u];i!=0;i=G1.edge[i].nxt){
		ll v=G1.edge[i].v;
		if(isI[v]){
			low[u]=min(low[u],dfn[v]);
		}else if(!dfn[v]){
			Tarjan(v);
			low[u]=min(low[u],low[v]);
		}
	}
	if(dfn[u]==low[u]){
		rps[++cN]=u;ll t;
		do{
			t=stk[tp--];
			isI[t]=0;bel[t]=cN;
		}while(t!=u);
	}
}
bool cTyp[PTN]; //component type (0: even; 1: odd)
ll col[PTN];
void ColDFS(ll u,bool color,ll curC){
	col[u]=color;
	G2.ptW[Id(curC,color)][1]+=G1.ptW[u][1]; //calculate values for each group (even component)
	for(ll i=G1.last[u];i!=0;i=G1.edge[i].nxt){
		ll v=G1.edge[i].v;bool w=G1.edge[i].w;
		if(bel[v]!=curC) continue;
		if(col[v]==-1) ColDFS(v,color^w,curC);
		else if((color^w)!=col[v]) cTyp[curC]=1; //odd component
	}
}
void BuildG2(){
	for(ll i=1;i<=G1.graM;i++){
		ll u=G1.edge[i].u,v=G1.edge[i].v;bool w=G1.edge[i].w;
		ll cU=bel[u],cV=bel[v];
		if(!cU||!cV) continue; //edges Youmu can never reach
		if(cU==cV) continue;   //edges inside the component
		ll myV=Id(cV,col[v]*(cTyp[cV]^1));
		if(cTyp[cU]==1){
			G2.AddUnEdge(Id(cU,0),myV,w);
			G2.AddUnEdge(Id(cU,0),myV,w^1);
		}else{
			G2.AddUnEdge(Id(cU,col[u]),myV,w);     //from this group
			G2.AddUnEdge(Id(cU,col[u]^1),myV,w^1); //from the other group
		}
	}
}
ll f[PTN][2];
ll F(ll u,bool typ){
	if(f[u][typ]!=-1) return f[u][typ];
	f[u][typ]=-INF; 
	for(ll i=G2.last[u];i!=0;i=G2.edge[i].nxt){
		ll v=G2.edge[i].v;bool w=G2.edge[i].w;
		f[u][typ]=max(f[u][typ],G2.ptW[u][typ]+F(v,typ^w));
	}
	return f[u][typ];
}
ll ST=1;
void Solve(){
	cN=0;dN=0;tp=0;for(ll i=1;i<=N;i++) dfn[i]=low[i]=0,bel[i]=0,isI[i]=0;
	Tarjan(ST); //Only need to get components Youmu can reach
	G2.GraphInit();
	for(ll i=1;i<=N;i++) col[i]=-1;
	for(ll i=1;i<=cN;i++) cTyp[i]=0,ColDFS(rps[i],0,i);
	for(ll i=1;i<=cN;i++){
		if(cTyp[i]==1){ //odd component
			G2.ptW[Id(i,0)][0]=G2.ptW[Id(i,0)][1]+=G2.ptW[Id(i,1)][1]; //an odd component enjoys all the values
			G2.ptW[Id(i,1)][0]=G2.ptW[Id(i,1)][1]=0; //abandon Id(i,1)
		}else{ //even component
			G2.ptW[Id(i,0)][0]=G2.ptW[Id(i,1)][1];
			G2.ptW[Id(i,1)][0]=G2.ptW[Id(i,0)][1];
		}
	}
	BuildG2();
	
	for(ll i=1;i<=2*N;i++) f[i][0]=f[i][1]=-1;
	ll myST=Id(bel[ST],col[ST]*(cTyp[bel[ST]]^1));
	f[myST][0]=G2.ptW[myST][0];
	ll ans=-INF;
	for(ll i=1;i<=2*N;i++)
		ans=max(ans,max(F(i,0),F(i,1)));
	printf("%lld",ans);
}
int main(){
	freopen("spring.in","r",stdin);
	freopen("spring_std.out","w",stdout);
	G1.GraphInit();
	N=Rd();ll m=Rd();
	for(ll u=1;u<=N;u++) G1.ptW[u][1]=Rd();
	while(m--){
		ll u=Rd(),v=Rd();bool w=Rd();
		G1.AddBscEdge(u,v,w); 
	}
	Solve();
	return 0;
}
```

## Omake

第一次出题，有纰漏请多多包涵。

快要交题时才发现一年前写的std出锅了，匆匆忙忙的重写了一个，不知道有没有新造出什么bug。数据也造得比较匆忙，如果爆炸了请随便辱骂出题人或者[去他博客上](https://www.cnblogs.com/sun123zxy/p/14019963.html)告诉他（

可以说这道题把二分图拓展到了强连通有向图上，不知道有没有什么更有趣的性质可以发掘。

后来做到几道性质相似的题目，这里列出来供参考： ~~垃圾撞题出题人~~

+ [CF1444C - Codeforces Round #680 - Team-Building](https://codeforces.com/contest/1444/problem/C) ([official solution](https://codeforces.com/blog/entry/84248))
+  [LOJ508 - LibreOJ NOI Round #1 - 失控的未来交通工具](https://loj.ac/problem/508) ([official solution](https://loj.ac/article/87))

思考背景怎样与题目契合也是个挺有趣的过程。

感谢听我乱扯idea的 TbYangZ 和 Waper ，以及尝试叉掉std的两位勇士 p9t6g 和 changruinian2020 。 ~~虽然都失败了~~ 

就这些吧。



<p style="text-align: right">——sun123zxy</p>

<p style="text-align: right">Oct. 2019 初稿完成 </p>

<p style="text-align: right">Nov. 2020 最后更新</p>



**Next Phantasm...**