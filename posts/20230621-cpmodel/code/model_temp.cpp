#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
// --- CAUTION uncensored ---
namespace MaxFlow{
    const int INF=999999999;
    const int PTN=20005,EDN=400005;
    struct Edge{
        int u,v,w;int nxt;
    }edge[EDN];
    int graN,graM,last[PTN];
    void GraphInit(){graM=0;for(int i=0;i<PTN;i++) last[i]=0;}
    void AddBscEdge(int u,int v,int w){
        edge[++graM]=(Edge){u,v,w,last[u]};
        last[u]=graM;
    }
    void AddNetEdge(int u,int v,int w){
        AddBscEdge(u,v,w);AddBscEdge(v,u,0);
    }
    int Un(int x){if(x%2==0) return x-1;else return x+1;}

    int ST,ED;int dis[PTN],gap[PTN],cur[PTN];
    bool bomb;
    int Send(int u,int ret){
        if(u==ED) return ret;
        int gone=0;
        for(int& i=cur[u];i!=0;i=edge[i].nxt){
            int v=edge[i].v,w=edge[i].w;
            if(w==0||dis[u]-1!=dis[v]) continue;
            int tmp=Send(v,min(ret,w));
            edge[i].w-=tmp;
            edge[Un(i)].w+=tmp;
            ret-=tmp;gone+=tmp;
            if(ret==0||bomb) return gone;
        }
        if(--gap[dis[u]]==0) bomb=1;
        gap[++dis[u]]++;
        return gone;
    }
    int ISAP(int st,int ed){
        ST=st;ED=ed;
        for(int i=1;i<=graN;i++) dis[i]=0,gap[i]=0;
        gap[0]=graN;
        bomb=0;int mxFlow=0;
        while(!bomb){
            for(int i=1;i<=graN;i++) cur[i]=last[i];
            mxFlow+=Send(ST,INF);
        }
        return mxFlow;
    }
}
namespace CostFlow{
    const int INF=999999999;
    const int PTN=10005,EDN=200005;
    struct Edge{
        int u,v,w,c;int nxt;
    }edge[EDN];
    int graN,graM,last[PTN];
    void GraphInit(){graM=0;for(int i=0;i<PTN;i++) last[i]=0;}
    void AddBscEdge(int u,int v,int w,int c){
        edge[++graM]=(Edge){u,v,w,c,last[u]};
        last[u]=graM;
    }
    void AddNetEdge(int u,int v,int w,int c){
        AddBscEdge(u,v,w,c);AddBscEdge(v,u,0,-c);
    }
    int Un(int x){if(x%2==0) return x-1;else return x+1;}

    int mxFlow,miCost,ST,ED;
    int dis[PTN],isQ[PTN],pre[PTN];
    int Q[10*PTN],hd,tl;
    bool SPFA(){
        for(int i=1;i<=graN;i++) dis[i]=INF,isQ[i]=0;
        hd=1;tl=0;
        dis[ST]=0;isQ[ST]=1;Q[++tl]=ST;
        while(hd<=tl){
            int u=Q[hd++];isQ[u]=0;
            for(int i=last[u];i!=0;i=edge[i].nxt){
                int v=edge[i].v,w=edge[i].w,c=edge[i].c;
                if(w==0) continue;
                if(dis[v]>dis[u]+c){
                    dis[v]=dis[u]+c;pre[v]=i;
                    if(!isQ[v]){
                        isQ[v]=1;
                        Q[++tl]=v;
                    }
                }
            }
        }
        if(dis[ED]>=INF) return 0;
        return 1;
    }
    void Adjust(){
        int dlt=INF;
        for(int v=ED;v!=ST;v=edge[pre[v]].u)
            dlt=min(dlt,edge[pre[v]].w);
        for(int v=ED;v!=ST;v=edge[pre[v]].u){
            edge[pre[v]].w-=dlt;
            edge[Un(pre[v])].w+=dlt;
        }
        mxFlow+=dlt;miCost+=dlt*dis[ED];
    }
    void EK(int st,int ed){
        ST=st,ED=ed;
        mxFlow=miCost=0;
        while(SPFA()) Adjust();
    }
}
namespace Tarjan{
    const ll PTN=1E6+5,EDN=2E6+5;
    ll N;
    struct Edge{ll u,v;bool w;ll nxt;};
    Edge edge[EDN];
	ll graM,last[PTN];
	void GraphInit(){graM=0;for(ll i=0;i<PTN;i++) last[i]=0;}
	void AddBscEdge(ll u,ll v,bool w){
	    edge[++graM]=(Edge){u,v,w,last[u]};
	    last[u]=graM;
	}
    ll bel[PTN],cN,rps[PTN]; //belong, number of components, representative vertax of the component
    ll dfn[PTN],low[PTN],dN;
    ll stk[PTN],tp;bool isI[PTN];
    void Tarjan(ll u){
	    dfn[u]=low[u]=++dN;
	    stk[++tp]=u;isI[u]=1;
	    for(ll i=last[u];i!=0;i=edge[i].nxt){
		    ll v=edge[i].v;
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
}