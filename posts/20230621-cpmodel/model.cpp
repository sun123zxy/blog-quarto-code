#include<bits/stdc++.h>
using namespace std;
typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef double db;
typedef long double ldb;

// --- read ---
template<typename T=ll>
T rd(){ // for bigint read
    T ans=0; bool sgn=0; char c=getchar();
    while(c<'0'||c>'9'){if(c=='-') sgn=1; c=getchar();}
    while(c>='0'&&c<='9'){ans=ans*10+T{c-'0'};c=getchar();}
    if(sgn) ans=-ans;
    return ans;
}

// --- dynamic mod ---
template<typename T=int>
inline T lmod(T x,T mod){if(x<0) x+=mod; else if(x>=mod) x-=mod; return x;}
template<typename T=int,typename U=ll>
inline T hmod(U x,T mod){return lmod<T>(x%mod,mod);}
template<typename T=int,typename U=ll>
T qpow(U x,ll up,T mod){
    x=hmod(x,mod); T ans=1;
    for(;up;up>>=1,x=U(x)*x%mod) if(up&1) ans=U(ans)*x%mod;
    return ans;
}
template<typename T=int, typename U=ll>
inline T inv(T x,T mod){return qpow<T,U>(x,mod-2,mod);}

// --- constant mod ---
template<typename T,typename U,T MOD,T PR=0>
class ModInt{public:
    static inline T lmod(T dat){
        while(dat<0) dat+=MOD;
        while(dat>=MOD) dat-=MOD;
        return dat;
    } static inline T hmod(U dat){return lmod(dat%MOD);}
    static inline ModInt uroot(T n){return qpow(ModInt(PR),(MOD-1)/n);}

    T dat; ModInt():dat{0}{}
    ModInt(initializer_list<T> lst):dat{*lst.begin()}{}
    ModInt(U dat):dat{hmod(dat)}{} // implicit conversion, strangely slow
    explicit operator T const(){return dat;}
    friend ModInt operator + (const ModInt a,const ModInt b){T dat=a.dat+b.dat; return {dat<=MOD?dat:dat-MOD};}
    friend ModInt operator - (const ModInt a){return {a.dat==0?0:MOD-a.dat};}
    friend ModInt operator - (const ModInt a,const ModInt b){return a+(-b);}
    friend ModInt operator * (const ModInt a,const ModInt b){return {T(U{a.dat}*b.dat%MOD)};}
    friend ModInt qpow(ModInt x,ll up){
        ModInt ans=1; for(;up;up>>=1,x=x*x) if(up&1) ans=ans*x;
        return ans;
    } friend ModInt inv(ModInt a){return {qpow(a,MOD-2)};} // assume MOD is prime
    friend ModInt operator / (const ModInt a,const ModInt b){return a*inv(b);}
    
    void operator += (const ModInt other){*this=*this+other;}
    void operator -= (const ModInt other){*this=*this-other;}
    void operator *= (const ModInt other){*this=*this*other;}
    void operator /= (const ModInt other){*this=*this/other;}

    friend ostream& operator << (ostream &out,const ModInt& mi){out<<mi.dat; return out;}
};

// --- polynomial ---
int log2ceil(int n){int cnt=0; for(int t=1;t<n;t<<=1) cnt++; return cnt;}
vector<int> rev;
void spawn_rev(int n){ // n=log2ceil(N)
    rev.resize(1<<n); rev[0]=0;
    for(int i=1;i<(1<<n);i++) rev[i]=(rev[i>>1]>>1)+((i&1)<<(n-1));
}
typedef ModInt<int,ll,998244353,3> MI;
class Poly : public vector<MI>{ public:
    using vector<MI>::vector;
    inline int len() const {return size();} // to avoid strange glitches caused by size_t
    Poly subpoly(int pos,int count) const { // imitate std::string::substr with resizing
        Poly B(begin()+min(pos,len()),begin()+min(pos+count,len()));
        B.resize(count); return B;
    }
    friend ostream& operator << (ostream &out,const Poly& A){for(int i=0;i<A.len();i++) out<<A[i]<<' '; return out;}
    
    friend Poly operator + (Poly A,Poly B){
        int n=max(A.len(),B.len()); A.resize(n); B.resize(n);
        for(int i=0;i<n;i++) A[i]+=B[i];
        return A;
    }
    friend Poly operator - (Poly A){
        for(int i=0;i<A.len();i++) A[i]=-A[i];
        return A;
    } friend Poly operator - (const Poly& A,const Poly &B){return A+(-B);}
    friend Poly operator * (Poly A,MI c){
        for(int i=0;i<A.len();i++) A[i]=A[i]*c;
        return A;
    }
    void DFT(int typ){ // to be called by opt() only (with proper length and rev array)
        int n=len();
        for(int i=0;i<n;i++) if(i<rev[i]) std::swap(at(i),at(rev[i]));
        for(int hf=1;hf<n;hf<<=1){
            MI w=MI::uroot(hf<<1); if(typ==-1) w=inv(w);
            for(int i=0;i<n;i+=hf<<1){
                MI wk=1;
                for(int k=0;k<hf;k++){
                    MI x=at(i+k),y=wk*at(i+hf+k);
                    at(i+k)=x+y; at(i+hf+k)=x-y;
                    wk=wk*w;
                }
            }
        }
        if(typ==-1){
            MI inv_n=inv(MI{n}); for(int i=0;i<n;i++) at(i)=at(i)*inv_n;
        }
    }
    friend Poly opt(Poly A,Poly B,int len,function<MI(MI a,MI b)> func){
        int n=log2ceil(len); spawn_rev(n); n=1<<n;
        A.resize(n); A.DFT(n); B.resize(n); B.DFT(n);
        Poly C(n); for(ll i=0;i<n;i++) C[i]=func(A[i],B[i]);
        C.DFT(-1); C.resize(len); return C;
    }
    friend Poly operator * (const Poly& A,const Poly& B){
        return opt(A,B,A.len()+B.len()-1,[](MI a,MI b){return a*b;});
    }
    friend Poly inv(const Poly &A){
        int n=A.len(); Poly B={inv(A[0])};
        for(int hf=1;hf<n;hf<<=1){
            B=opt(A.subpoly(0,hf*2),B,hf*4,[](MI a,MI b){return (2-a*b)*b;});
            B.resize(hf*2);
        } B.resize(n); return B;
    }
    friend Poly sqrt(const Poly &A){
        int n=A.len(); Poly B={1}; // assume a_0 = 1 and b_0 positive
        for(int hf=1;hf<n;hf<<=1){
            B=(A.subpoly(0,2*hf)*inv(B.subpoly(0,2*hf))+B)*inv(MI{2});
            B.resize(hf*2);
        } B.resize(n); return B;
    }
    friend Poly drv(Poly A){ // derivative
        for(int i=0;i<A.len();i++) A[i]=(i+1)*A[i+1];
        A.pop_back(); return A;
    }
    friend Poly itg(Poly A,MI c){ // integral
        A.push_back(0); for(int i=A.len();i>=1;i--) A[i]=A[i-1]*inv(MI{i});
        A[0]=c; return A;
    }
    friend Poly ln(const Poly &A){
        return itg((drv(A)*inv(A)).subpoly(0,A.len()-1),0/*log(A[0])*/);
    }
    friend Poly exp(const Poly &A){
        int n=A.len(); Poly B={1/*exp(A[0])*/};
        for(int hf=1;hf<n;hf<<=1){
            B=B*(Poly{1}-ln(B.subpoly(0,hf*2))+A.subpoly(0,hf*2)); B.resize(hf*2);
        } B.resize(n);
        return B;
    }
    friend Poly qpow(const Poly &A,MI up){return exp(ln(A)*up);} // assume a_0 = 1
};

void entry(){
    ll N=rd()+1,M=rd()+1;
    Poly A(N),B(M);
    for(ll i=0;i<N;i++) A[i]=rd();
    for(ll i=0;i<M;i++) B[i]=rd();
    Poly C=A*B; cout<<C;
    /*ll N=rd(); Poly A(N);
    for(ll i=0;i<N;i++) A[i]=rd();
    A=inv(A); cout<<A;*/
}
int main(){
    //freopen("t1.in","r",stdin);
    //freopen("t1.out","w",stdout);
    //ll T=rd(); while(T--){
        entry();
    //}
    return 0;
}