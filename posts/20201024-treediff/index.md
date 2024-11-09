---
title: "树上差分的两种形式（相遇 or 行程的交集 题解）"
date: 2020/10/24
date-modified: 2020/10/24
categories:
  - algorithm
  - solution
  
format:
  html: default
---

emm，这很NOIP...

写这个的原因是今天考试一个sb差分树题居然杠了个树剖上去，杀鸡用了牛刀。

而且不止一次了...总是想不到子树和这种差分，淦

Anyway，简单写写吧。

## 树上差分的两种形式

+ **单点修改&链查询**用差分转化为**单点修改&树上前缀和**

  ```c++
  class MPQU{public: //单点修改 & 树上前缀和 (Modify Point Query Up)
  	BIT bit;
  	MPQU(bool typ=0){if(typ==1) bit=BIT(dN);}
  	void Add(ll u,ll w){bit.Add(st[u],w); bit.Add(ed[u],-w);}
  	ll Query(ll u){return bit.Query(st[u]);}
  };
  class MPQL{public: //单点修改 & 链查询 (Modify Point Query Link)
  	MPQU mpqu;
  	MPQL(bool typ=0){if(typ==1) mpqu=MPQU(1);}
  	void Add(ll u,ll w){mpqu.Add(u,w);}
  	ll Query(ll u,ll v){ll lca=LCA(u,v); return mpqu.Query(u)+mpqu.Query(v)-mpqu.Query(lca)-mpqu.Query(fa[lca]);}
  }mpql;
  ```

+ **链修改&单点查询**用差分转化为**单点修改&子树和**

  ```c++
  class MPQD{public: //单点修改 & 子树和 (Modify Point Query Down)
  	BIT bit;
  	MPQD(bool typ=0){if(typ==1) bit=BIT(dN);} 
  	void Add(ll u,ll w){bit.Add(st[u],w);}
  	ll Query(ll u){return bit.Query(ed[u])-bit.Query(st[u]-1);}
  };
  class MLQP{public: //链修改 & 单点查询 (Modify Link Query Point)
  	MPQD mpqd;
  	MLQP(bool typ=0){if(typ==1) mpqd=MPQD(1);}
  	void Add(ll u,ll v){ll lca=LCA(u,v); mpqd.Add(u,1); mpqd.Add(v,1); mpqd.Add(lca,-1); mpqd.Add(fa[lca],-1);}
  	ll Query(ll u){return mpqd.Query(u);}
  }mlqp;
  ```

~~三重封装~~

转化后的问题都能用**DFS序+树状数组**解决。（在上文代码中，树状数组用`BIT`封装，`st, ed`是DFS序）

本质：

+ **树上前缀和**对应**序列前缀和**

+ **子树和**对应**序列后缀和**

在序列上，前缀和、后缀和均能解决这两个问题。

而之所以在树上这两种形式求解的问题出现不同，是因为与序列相比，树是上小下大的。

## 相遇 or 行程的交集 题解

这道题综合的考察了两种差分，成功把我干翻（

[[出处不明] 相遇 or 行程的交集](https://oj.bashu.com.cn/code/problempage.php?problem_id=5960)

>Description
>
>　　豪哥生活在一个n个点的树形城市里面，每一天都要走来走去。虽然走的是比较的多，但是豪哥在这个城市里面的朋友并不是很多。
>
>　　当某一天，猴哥给他展现了一下大佬风范之后，豪哥决定要获得一些交往机会来提升交往能力。豪哥现在已经物色上了一条友，打算和它（豪哥并不让吃瓜群众知道性别）交往。豪哥现在spy了一下这个人的所有行程起点和终点，豪哥打算从终点开始走到起点与其相遇。但是豪哥是想找话题的，他想知道以前有多少次行程和此次行程是有交集的，这样豪哥就可以搭上话了。这个路径与之前路径的有交集数量作为豪哥此次的交往机会。
>
>　　但是豪哥急着要做交往准备，所以算什么交往机会的小事情就交给你了。
>
>Input
>
>　　第一行一个正整数n表示节点个数。
>
>　　接下来n-1行，每行两个正整数分别是u，v表示节点u和v之间有连边。
>
>　　接下来一行一个 正整数m表示路径个数。
>
>　　然后有m行，每行两个正整数分别是u，v分别表示u到v之间有一条路径。
>
>Output
>
>　　输出共m行，每行一个整数，第i行表示豪哥在这条路径上获得的交往机会。
>
>Sample Input
>
>```
>5
>1 2
>1 3
>3 4
>3 5
>4
>4 5
>4 2
>1 3
>1 2
>```
>
>Sample Output
>
>```
>0
>1
>2
>2
>```
>
>Hint
>
>【数据范围与约定】
>
>　　对于20%的数据n，m≤2000
>
>　　对于另外20%的数据n，m≤50000
>
>　　对于另外10%的数据n，m≤200000，保证树形结构是一条链
>
>　　对于另外50%的数据n，m≤200000

简要题意：给你一棵树，然后按顺序输入若干条路径。问每次输入路径时该路径与前面已经输入的多少条路径有交集。

主要要想到把**两条树上路径有交集**转换为**其中某条路径的LCA被另一条路径所覆盖**。

于是对于当前输入的路径，分**当前路径覆盖了多少个之前路径的LCA**和**当前路径的LCA被之前多少个路径覆盖**讨论即可。前者是**单点修改&链查询**，后者是**链修改&单点查询**，按前文的方法做就可以了。

注意特判LCA重叠的情况，见代码。

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
typedef vector<ll> Vector;
typedef vector<ll>::iterator VecIt;
ll Rd(){
	ll ans=0;bool fh=0;char c=getchar();
	while(c<'0'||c>'9'){if(c=='-') fh=1; c=getchar();}
	while(c>='0'&&c<='9') ans=ans*10+c-'0',c=getchar();
	if(fh) ans=-ans;
	return ans;
}

ll Lowbit(ll x){return x&(-x);}

const ll PTN=2E5+5; 
class BIT{public: 
	vector<ll> tr;ll n;
	BIT(){}
	BIT(ll n){this->n = n; tr=vector<ll>(n+1,0);}
	void Add(ll p,ll w){
		if(!p) return;
		for(ll i=p;i<=n;i+=Lowbit(i)) tr[i]+=w;
	}
	ll Query(ll p){
		ll ans=0;
		for(ll i=p;i>=1;i-=Lowbit(i)) ans+=tr[i];
		return ans;
	}
};

ll N;
Vector edge[PTN];
ll fa[PTN],dep[PTN],sz[PTN],son[PTN];
ll st[PTN],ed[PTN];ll dN;
void DFS1(ll u,ll father,ll depth){
	fa[u]=father;dep[u]=depth;sz[u]=1;son[u]=0;st[u]=++dN;
	for(VecIt it = edge[u].begin(); it!=edge[u].end(); it++){
		ll v=(*it);if(v==father) continue;
		DFS1(v,u,depth+1);
		sz[u]+=sz[v];
		if(sz[v]>sz[son[u]]) son[u]=v;
	}
	ed[u]=++dN;
}
ll tp[PTN];
void DFS2(ll u,ll toop){
	tp[u]=toop;
	if(son[u]) DFS2(son[u],toop);
	for(VecIt it = edge[u].begin(); it!=edge[u].end(); it++){
		ll v=(*it);if(v==fa[u]) continue;
		if(v!=son[u]) DFS2(v,v);
	}
}
ll LCA(ll u,ll v){
	while(tp[u]!=tp[v]){
		if(dep[tp[u]]<dep[tp[v]]) swap(u,v);
		u=fa[tp[u]];
	}
	if(dep[u]<dep[v]) return u;
	else return v;
}

class MPQU{public: //单点修改 & 树上前缀和 (Modify Point Query Up)
	BIT bit;
	MPQU(bool typ=0){if(typ==1) bit=BIT(dN);}
	void Add(ll u,ll w){bit.Add(st[u],w); bit.Add(ed[u],-w);}
	ll Query(ll u){return bit.Query(st[u]);}
};
class MPQD{public: //单点修改 & 子树和 (Modify Point Query Down)
	BIT bit;
	MPQD(bool typ=0){if(typ==1) bit=BIT(dN);} 
	void Add(ll u,ll w){bit.Add(st[u],w);}
	ll Query(ll u){return bit.Query(ed[u])-bit.Query(st[u]-1);}
};
class MPQL{public: //单点修改 & 链查询 (Modify Point Query Link)
	MPQU mpqu;
	MPQL(bool typ=0){if(typ==1) mpqu=MPQU(1);}
	void Add(ll u,ll w){mpqu.Add(u,w);}
	ll Query(ll u,ll v){ll lca=LCA(u,v); return mpqu.Query(u)+mpqu.Query(v)-mpqu.Query(lca)-mpqu.Query(fa[lca]);}
}mpql;
class MLQP{public: //链修改 & 单点查询 (Modify Link Query Point)
	MPQD mpqd;
	MLQP(bool typ=0){if(typ==1) mpqd=MPQD(1);}
	void Add(ll u,ll v){ll lca=LCA(u,v); mpqd.Add(u,1); mpqd.Add(v,1); mpqd.Add(lca,-1); mpqd.Add(lca,-1);/*mpqd.Add(fa[lca],-1);*/} //特殊处理重叠LCA 
	ll Query(ll u){return mpqd.Query(u);}
}mlqp;
int main(){
	//freopen("meet.in","r",stdin);
	//freopen("meet.out","w",stdout);
	N=Rd();
	for(ll i=1;i<N;i++){
		ll u=Rd(),v=Rd();
		edge[u].push_back(v);
		edge[v].push_back(u);
	}
	dN=0;sz[0]=0;DFS1(1,0,0);
	DFS2(1,1);
	st[0]=0;ed[0]=0;
	mpql=MPQL(1);mlqp=MLQP(1); 
	ll qN=Rd();
	for(ll q=1;q<=qN;q++){
		ll u=Rd(),v=Rd();
		ll lca=LCA(u,v);
		
		printf("%lld\n",mpql.Query(u,v)+mlqp.Query(lca));
		mpql.Add(lca,1);
		mlqp.Add(u,v);
	}
	return 0;
}
```

