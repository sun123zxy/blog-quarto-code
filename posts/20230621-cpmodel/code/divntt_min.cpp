#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD=998244353,PR=3;
#define pmod_m(x,mod) ((x)<(mod)?(x):(x)-(mod))
#define nmod_m(x,mod) ((x)<0?(x)+(mod):(x))
#define hmod_m(x,mod) nmod_m((x)%(mod),(mod))
#define pmod(x) pmod_m(x,MOD)
#define nmod(x) nmod_m(x,MOD)
#define hmod(x) hmod_m(x,MOD)

template<typename T,typename U>
class ModInt{public:
    static inline ModInt uroot(T n){return qpow(ModInt(PR),(MOD-1)/n);}
    T dat; ModInt():dat{0}{}
    ModInt(initializer_list<T> lst):dat{*lst.begin()}{}
    ModInt(U dat):dat{T(hmod(dat))}{}
    explicit operator T const(){return dat;}
    friend ostream& operator << (ostream &out,const ModInt& mi){out<<mi.dat;return out;}

    friend ModInt operator + (const ModInt a,const ModInt b){return {pmod(a.dat+b.dat)};}
    friend ModInt operator - (const ModInt a){return {nmod(-a.dat)};}
    friend ModInt operator - (const ModInt a,const ModInt b){return {nmod(a.dat-b.dat)};}
    friend ModInt operator * (const ModInt a,const ModInt b){return {T(U{a.dat}*b.dat%MOD)};}
    friend ModInt qpow(ModInt x,ll up){
        ModInt ans=1; for(;up;up>>=1,x=x*x) if(up&1) ans=ans*x; return ans;
    }
    friend inline ModInt inv(ModInt a){return {qpow(a,MOD-2)};}
};

typedef ModInt<ll,ll> MI;

ll log2ceil(ll n){ll cnt=0; for(ll t=1;t<n;t<<=1) cnt++; return cnt;}
vector<ll> rev;
void spawn_rev(ll n){
    rev.resize(1<<n); rev[0]=0;
    for(ll i=1;i<(1<<n);i++) rev[i]=(rev[i>>1]>>1)+((i&1)<<(n-1));
}
class Poly:public vector<MI>{public:
    using vector<MI>::vector;
    inline int len()const{return size();}
    Poly subpoly(int pos,int count)const{
        Poly B(begin()+min(pos,len()),begin()+min(pos+count,len()));
        B.resize(count); return B;
    }
    friend ostream& operator << (ostream &out,const Poly& A){
        for(ll i=0;i<A.len();i++) out<<A[i]<<" "; return out;
    }
    friend Poly operator + (Poly A,const Poly& B){
        ll n=max(A.len(),B.len()); A.resize(n);
        for(ll i=0;i<n;i++) A[i]=A[i]+MI(i<B.len()?B[i]:0); return A;
    }
    void DFT(ll typ){
        ll n=len();
        for(ll i=0;i<n;i++) if(i<rev[i]) std::swap(at(i),at(rev[i]));
        for(ll hf=1;hf<n;hf<<=1){
            MI w=MI::uroot(hf<<1); if(typ==-1) w=inv(w);
            for(ll i=0;i<n;i+=hf<<1){
                MI wk=1;
                for(ll k=0;k<hf;k++){
                    MI x=at(i+k),y=wk*at(i+hf+k);
                    at(i+k)=x+y; at(i+hf+k)=x-y;
                    wk=wk*w;
                }
            }
        }
        if(typ==-1){MI inv_n=inv(MI{n}); for(ll i=0;i<n;i++) at(i)=at(i)*inv_n;}
    }
    friend Poly opt(Poly A,Poly B,int len,function<MI(MI a,MI b)> func){
        ll n=log2ceil(len); spawn_rev(n); n=1<<n;
        A.resize(n); A.DFT(n); B.resize(n); B.DFT(n);
        Poly C(n); for(ll i=0;i<n;i++) C[i]=func(A[i],B[i]);
        C.DFT(-1); C.resize(len); return C;
    }
    friend Poly operator * (const Poly& A,const Poly& B){
        return opt(A,B,A.len()+B.len()-1,[](MI a,MI b){return a*b;});
    }
};

Poly A,B,C;
void work(ll l,ll r){ // capable of non-negative [l,r)
    ll mid=(l+r)/2;
    if(l+1==r) return;
    Poly T=A.subpoly(l,mid-l)*B.subpoly(mid,r-mid);
    for(ll k=0;k<T.len()&&l+mid+k<C.len();k++)
        C[l+mid+k]=C[l+mid+k]+T[k];
    work(l,mid); work(mid,r);
}