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

// --- variable mod ---
const int MOD=998244353,PR=3;
#define pmod_m(x,mod) ((x)<(mod)?(x):(x)-(mod))
#define nmod_m(x,mod) ((x)<0?(x)+(mod):(x))
#define hmod_m(x,mod) nmod_m((x)%(mod),(mod)) // slow!
#define pmod(x) pmod_m(x,MOD)
#define nmod(x) nmod_m(x,MOD)
#define hmod(x) hmod_m(x,MOD)
template<typename T=int,typename U=ll>
T qpow(U x,ll up,T mod){
    x=hmod_m(x,mod); T ans=1; 
    for(;up;up>>=1,x=U(x)*x%mod) if(up&1) ans=U(ans)*x%mod;
    return ans;
}
template<typename T=int,typename U=ll>
ll inv(T x,T mod){return qpow<T,U>(x,mod-2);} // assume mod prime

// --- Number Theory ---
ll gcd(ll a,ll b){return b==0?a:gcd(b,a%b);}
ll lcm(ll a,ll b){return a*b/gcd(a,b);}
tuple<ll,ll,ll> exgcd(ll a,ll b){ // capable of +/- integers
    if(b==0) return {1,0,a};
    ll x1,y1,d; tie(x1,y1,d)=exgcd(b,a%b);
    return {y1,x1-(a/b)*y1,d};
}
ll inv(ll a,ll m){
    ll x,y,d; tie(x,y,d)=exgcd(a,m);
    return d==1?hmod_m(x,m):0;
}
tuple<ll,ll,bool> solve_equ(ll a,ll b,ll c){ // return solution with min non-negative x
    ll x,y,d; tie(x,y,d)=exgcd(a,b);
    if(d==0) return {0,0,c==0}; // !!!
    if(c%d!=0) return {0,0,false};
    x*=c/d; y*=c/d; ll dx=b/d,dy=-a/d;
    if(dx<0) dx=-dx,dy=-dy; // ensure dx positive
    ll t=(hmod_m(x,dx)-x)/dx; x+=t*dx; y+=t*dy;
    return {x,y,true};
}
// CAUTION __int128
pair<ll,ll> excrt(pair<ll,ll> p1,pair<ll,ll> p2){ // merge (a1,m1) (a2,m2)
    ll a1,m1,a2,m2; tie(a1,m1)=p1; tie(a2,m2)=p2;
    ll x,y; bool ok; tie(x,y,ok)=solve_equ(m1,m2,a2-a1);
    if(!ok) return {0,0};
    ll l=lcm(m1,m2); return {hmod_m(x*m1+a1,l),l};
}
ll bsgs(ll a,ll b,ll m){ // solve a^x=b mod m, gcd(a,m)=1
    unordered_map<ll,bool> mp; ll sqrtM=ceil(sqrt(m));
    ll cur=1;
    for(ll r=1;r<=sqrtM;r++){
        cur=cur*a%m;
        mp[b*cur%m]=r;
    }
    ll nw=cur;
    for(ll q=1;q<=sqrtM;q++){
        if(mp[nw]) return q*sqrtM-mp[nw];
        nw=nw*cur%m;
    }
    return -1;
}
// --- CAUTION uncensored / __int128 / rand() ---
bool is_prime(ll n){ // miller rabin
    if(n<=2||n%2==0) return n==2;
    ll u=n-1,t=0; while(u%2==0) u/=2,t++;
    ll test_time=10; while(test_time--){
        ll a=rand()%(n-2)+2,v=qpow<ll,__int128>(a,u,n);
        if(v==1) continue;
        bool ok=0;
        for(ll s=0;s<t;s++){
            if(v==n-1){ok=1;break;}
            v=__int128(v)*v%n;
        } if(!ok) return false;
    } return true;
}
// --- CAUTION uncensored __int128 rand() ---
ll pollard_rho(ll n) {
    auto f=[&](ll x,ll c)->ll{return (__int128(x)*x+c)%n;};
    ll c=rand()%(n-1)+1;
    ll t=f(0,c);
    ll r=f(f(0,c),c);
    while (t!=r) {
        ll d=gcd(abs(t-r),n);
        if(d>1) return d;
        t=f(t,c);
        r=f(f(r,c),c);
    } return n;
} ll get_factor(ll n){
    if(n==1||is_prime(n)) return 0;
    if(n==4) return 2;
    ll d=n; while(d==n) d=pollard_rho(n);
    return d;
}
void _prime_test(){
    srand(time(0));
    ll N=rd();
    cout<<(is_prime(N)?"YES":"NO")<<" "<<get_factor(N);
}

// --- constant mod, can be modified to be variable ---
template<typename T,typename U>
class ModInt{public:
    static inline ModInt uroot(T n){return qpow(ModInt(PR),(MOD-1)/n);}

    T dat; ModInt():dat{0}{}
    ModInt(initializer_list<T> lst):dat{*lst.begin()}{} // for mod-free number, use this
    ModInt(U dat):dat{T(hmod(dat))}{} // implicit conversion, slow!
    explicit operator T const(){return dat;}
    friend ostream& operator << (ostream &out,const ModInt& mi){out<<mi.dat; return out;}
    // negative number version
    // friend ostream& operator << (ostream &out,const ModInt& mi){out<<(mi.dat+mi.dat<MOD?mi.dat:mi.dat-MOD)<<" "; return out;}
    friend ModInt operator + (const ModInt a,const ModInt b){return {pmod(a.dat+b.dat)};}
    friend ModInt operator - (const ModInt a){return {nmod(-a.dat)};}
    friend ModInt operator - (const ModInt a,const ModInt b){return {nmod(a.dat-b.dat)};}
    friend ModInt operator * (const ModInt a,const ModInt b){return {T(U{a.dat}*b.dat%MOD)};}
    friend ModInt operator / (const ModInt a,const ModInt b){return {a.dat/b.dat};} // Euclidean division
    friend ModInt operator % (const ModInt a,const ModInt b){return {a.dat%b.dat};} // Euclidean division
    friend ModInt qpow(ModInt x,ll up){
        ModInt ans=1; for(;up;up>>=1,x=x*x) if(up&1) ans=ans*x; return ans;
    } friend inline ModInt inv(ModInt a){return {qpow(a,MOD-2)};} // assume MOD is prime
    void operator += (const ModInt other){*this=*this+other;}
    void operator -= (const ModInt other){*this=*this-other;}
    void operator *= (const ModInt other){*this=*this*other;}
    void operator /= (const ModInt other){*this=*this/other;}
    void operator %= (const ModInt other){*this=*this%other;}
    friend bool operator == (const ModInt a,const ModInt b){return a.dat==b.dat;}
    friend bool operator != (const ModInt a,const ModInt b){return a.dat!=b.dat;}
};
typedef ModInt<int,ll> MI;

// --- linear init ---
vector<MI> fac,facinv,inv_s;
void linear_init(int n){
    fac.resize(n+1); facinv.resize(n+1); inv_s.resize(n+1);
    inv_s[1]=1; for(int i=1;i<=n;i++) inv_s[i]=-(MOD/i)*inv_s[MOD%i];
    fac[0]=1; for(int i=1;i<=n;i++) fac[i]=fac[i-1]*i;
    facinv[n]=inv(MI(fac[n])); for(int i=n-1;i>=0;i--) facinv[i]=facinv[i+1]*(i+1);
}

// --- polynomial ---
int log2ceil(int n){int cnt=0; for(int t=1;t<n;t<<=1) cnt++; return cnt;}
vector<int> rev;
void spawn_rev(int n){ // n=log2ceil(N)
    rev.resize(1<<n); rev[0]=0;
    for(int i=1;i<(1<<n);i++) rev[i]=(rev[i>>1]>>1)+((i&1)<<(n-1));
}
class Poly : public vector<MI>{ public:
    using vector<MI>::vector;
    inline int len() const {return size();} // to avoid strange glitches caused by size_t
    Poly subpoly(int l,int r) const { // [l,r), zero padded (support negative number)
        Poly B; for(int i=l;i<r;i++) B.push_back(i>=0&&i<len()?at(i):0); return B;
    }
    friend ostream& operator << (ostream &out,const Poly& A){for(int i=0;i<A.len();i++) out<<A[i]<<' '; return out;}
    friend Poly operator + (Poly A,const Poly& B){
        int n=max(A.len(),B.len()); A.resize(n);
        for(int i=0;i<n;i++) A[i]+=i<B.len()?B[i]:0; return A;
    }
    friend Poly operator - (Poly A){for(int i=0;i<A.len();i++) A[i]=-A[i]; return A;}
    friend Poly operator - (const Poly& A,const Poly &B){return A+(-B);}
    friend Poly operator * (Poly A,MI c){for(int i=0;i<A.len();i++) A[i]=A[i]*c; return A;}
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
                    wk*=w;
                }
            }
        }
        if(typ==-1){MI inv_n=inv(MI{n}); for(int i=0;i<n;i++) at(i)=at(i)*inv_n;}
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
        A.push_back(0); for(int i=A.len()-1;i>=1;i--) A[i]=A[i-1]*inv(MI{i});
        A[0]=c; return A;
    }
    friend inline Poly ln(const Poly &A){
        return itg((drv(A)*inv(A)).subpoly(0,A.len()-1),0/*log(A[0])*/);
    }
    friend Poly exp(const Poly &A){
        int n=A.len(); Poly B={1/*exp(A[0])*/};
        for(int hf=1;hf<n;hf<<=1){
            B=B*(Poly{1}-ln(B.subpoly(0,hf*2))+A.subpoly(0,hf*2)); B.resize(hf*2);
        } B.resize(n);
        return B;
    }
    friend inline Poly qpow(const Poly &A,MI up){return exp(ln(A)*up);} // assume a_0 = 1
    
    static void _test(){
        ll N=rd()+1,M=rd()+1;
        Poly A(N),B(M);
        for(ll i=0;i<N;i++) A[i]=rd();
        for(ll i=0;i<M;i++) B[i]=rd();
        Poly C=A*B; cout<<C;
        /*ll N=rd(); Poly A(N);
        for(ll i=0;i<N;i++) A[i]=rd();
        A=inv(A); cout<<A;*/
    }
};
namespace divntt{
    Poly A,B,C;
    void divntt(ll l,ll r){ // capable of non-negative [l,r), "i<j" order
        if(l+1==r) return;
        ll mid=(l+r)/2;
        Poly T=A.subpoly(l,mid)*B.subpoly(mid,r);
        for(ll k=0;k<T.len()&&l+mid+k<C.len();k++){ // CAUTION: time complexity
            C[l+mid+k]+=T[k];
        }
        divntt(l,mid); divntt(mid,r);
    }
}

// --- matrix ---
class Matrix : public vector<MI>{public:
    static Matrix I(int n){
        Matrix A(n,n); for(int i=0;i<n;i++) A(i,i)=1;
        return A;
    }
    int row,col;
    template<typename... Args>
    Matrix(int row,int col,Args... args) : row{row},col{col},vector<MI>(row*col,args...){}
    Matrix(int row,int col,initializer_list<MI> lst) : row{row},col{col},vector<MI>(lst){}
    MI& at(int i,int j){return vector<MI>::at(i*col+j);}
    MI cat(int i,int j)const{return *(cbegin()+i*col+j);}
    MI& operator () (int i,int j){return at(i,j);}
    Matrix submat(int sr,int sc,int tr,int tc){
        Matrix A(tr-sr,tc-sc,{});
        for(int i=sr;i<tr;i++) for(int j=sc;j<tc;j++) A(i-sr,j-sc)=at(i,j);
        return A;
    }
    friend ostream& operator << (ostream &out,const Matrix& A){
        for(int i=0;i<A.row;i++){for(int j=0;j<A.col;j++) cout<<A.cat(i,j)<<' ';cout<<'\n';} return out;
    }

    friend Matrix operator * (const Matrix &A,const Matrix &B){
        Matrix C(A.row,B.col); if(A.col!=B.row) return Matrix(0,0);
        for(int i=0;i<A.row;i++)
            for(int j=0;j<B.col;j++)
                for(int k=0;k<A.col;k++)
                    C(i,j)+=A.cat(i,k)*B.cat(k,j);
        return C;
    }
    friend Matrix qpow(Matrix X,ll up){
        Matrix A=Matrix::I(X.row); if(X.row!=X.col) return Matrix(0,0);
        for(;up;up>>=1,X=X*X) if(up&1) A=A*X; return A;
    }
    bool stair(){ // row reduce to upper stair matrix, return swap time % 2. capable of any ring
        bool swp=0;
        for(int c=0,p=0;c<min(row,col);c++){
            auto swpl=[&](int a,int b){
                swp^=1; for(int k=c;k<col;k++) std::swap(at(a,k),at(b,k));
            };
            for(int r=p+1;r<row;r++){
                while(at(p,c)!=0){
                    MI factor=at(r,c)/at(p,c);
                    for(int k=c;k<col;k++) at(r,k)-=at(p,k)*factor;
                    swpl(p,r);
                } swpl(p,r);
            } if(at(p,c)!=0) p++;
        } return swp;
    }
    friend MI det(Matrix A){
        int n=A.row; if(A.row!=A.col) return 0;
        bool opt=A.stair();
        MI ans=1; for(int c=0;c<n;c++) ans*=A(c,c);
        return opt?-ans:ans;
    }
    friend int rk(Matrix A){ // rank (confuse with std::rank)
        A.stair();
        for(int r=A.row-1;r>=0;r--)
            for(int c=0;c<A.col;c++)
                if(A(r,c)!=0) return r+1;
        return 0; 
    }

    static void _test(){
        ll N=rd(),M=rd(); Matrix A(N,M,{});
        for(ll i=0;i<N;i++) for(ll j=0;j<M;j++) A.push_back(rd());
        A.stair(); cout<<A;
    }
};

// --- automata ---
template <int PTN,int STRN,int CHAR,char OFFSET>
class ACAutomation{public: // id: 0 is null, 1 is start
    struct Vtx{
        int fail;
        array<int,CHAR> ch;
    }vtx[PTN];
    int last[STRN];
    int vn,sn;
    ACAutomation(){
        sn=0; vn=0; vtx[++vn]={0};
        for(int c=0;c<CHAR;c++) vtx[0].ch[c]=1;
    }
    void insert(string s){
        int p=1;
        for(char c : s){ c-=OFFSET;
            if(!vtx[p].ch[c]) vtx[p].ch[c]=++vn;
            p=vtx[p].ch[c];
        } last[++sn]=p;
    }
    void buildFail(){
        queue<int> que; que.push(1);
        while(!que.empty()){
            int p=que.front(),f=vtx[p].fail; que.pop();
            for(int c=0;c<CHAR;c++){ int q=vtx[p].ch[c];
                if(q) vtx[q].fail=vtx[f].ch[c],que.push(q);
                else vtx[p].ch[c]=vtx[f].ch[c];
            }
        }
    }
    // below: subject to problem Lugou P5357
    int cnt[PTN];
    void match(string s){
        for(int i=1;i<=vn;i++) cnt[i]=0;
        int p=1;
        for(char c : s){ c-=OFFSET;
            p=vtx[p].ch[c];
            cnt[p]++;
        }
    }
    vector<int> edge[PTN];
    void buildTree(){ // fail tree
        for(int u=1;u<=vn;u++) edge[u].clear();
        for(int u=2;u<=vn;u++) edge[vtx[u].fail].push_back(u);
        function<void(int)> dfs=[&](int u){
            for(int v : edge[u]) dfs(v),cnt[u]+=cnt[v];
        }; dfs(1);
    }
    void answer(){
        for(int i=1;i<=sn;i++) cout<<cnt[last[i]]<<'\n';
    }
};
typedef ACAutomation<int(1E6+5),int(1E6+5),26,'a'> ACAM;

template<int PTN> // 2 * length of string suffices
class SuffixAutomation{public: // id: 0 is null, 1 is start
    struct Vtx{
        int fa,len; bool real;
        map<char,int> ch; // feel free to modify to array<int,26>
    }vtx[PTN];
    int n,last;
    SuffixAutomation(){n=last=0; vtx[++n]={0,0,true}; last=n;}
    void insert(char c){
        int p=last,cur=++n; vtx[cur]={0,vtx[last].len+1,true};
        for(;p&&!vtx[p].ch[c];p=vtx[p].fa) vtx[p].ch[c]=cur;
        int q=vtx[p].ch[c];
        if(!p) vtx[cur].fa=1;
        else if(vtx[q].len==vtx[p].len+1) vtx[cur].fa=q;
        else{ // vtx[q].len>vtx[p].len+1, partition needed
            vtx[++n]=vtx[q]; vtx[n].real=false; vtx[n].len=vtx[p].len+1;
            for(;p&&vtx[p].ch[c]==q;p=vtx[p].fa) vtx[p].ch[c]=n;
            vtx[cur].fa=vtx[q].fa=n;
        }
        last=cur;
    }
    ll LCSWith(string s){ // as an example of matching
        ll p=1; ll ans=0,cnt=0;
        for(char c : s){
            if(vtx[p].ch[c]) cnt++;
            else{
                for(;p&&!vtx[p].ch[c];p=vtx[p].fa);
                cnt=p?vtx[p].len+1:0;
            } ans=max(ans,cnt);
            p=p?vtx[p].ch[c]:1;
        } return ans;
    }
    // below: subject to problem Luogu P3804
    vector<int> edge[PTN]; int sz[PTN];
    void buildTree(){
        for(int u=1;u<=n;u++) edge[u].clear();
        for(int u=2;u<=n;u++) edge[vtx[u].fa].push_back(u);
        function<void(int)> dfs=[&](int u){
            sz[u]=vtx[u].real;
            for(int v : edge[u]) dfs(v),sz[u]+=sz[v];
        }; dfs(1);
    }
    ll answer(){
        ll ans=0;
        for(ll u=1;u<=n;u++) if(sz[u]>1) ans=max(ans,1LL*sz[u]*vtx[u].len);
        return ans; 
    }
};
typedef SuffixAutomation<int(2E6+5)> SAM;

template <int PTN>
class PalindromicAutomation{public: // id: 1 is odd root, 0 is even root (as fallback)
    struct Vtx{
        int fail,len;
        map<char,int> ch; // feel free to modify to array<int,26>
    }vtx[PTN];
    int vn,sn,last; char s[PTN];
    int cnt[PTN]; // subject to problem Luogu P5496
    PalindromicAutomation(){
        vn=1; vtx[0]={1,0}; vtx[1]={0,-1}; // fail[even]=odd, fail[odd]=even
        last=0; sn=0;
    }
    int getValid(int p){ // even is never valid, odd is always valid
        for(;s[sn-vtx[p].len-1]!=s[sn];p=vtx[p].fail);
        return p;
    }
    void insert(char c){
        s[++sn]=c; int p=getValid(last); 
        if(!vtx[p].ch[c]){
            int f=getValid(vtx[p].fail); // when p=odd, fail[p]=even, then f is still odd
            vtx[++vn]={vtx[f].ch[c],vtx[p].len+2}; // len[odd]=-1 useful here
                                                   // vtx[f].ch[c]=0 only happens when p=odd
                                                   // for s[sn-vtx[p].len-1..sn] is already a palindrome
            vtx[p].ch[c]=vn;
            cnt[vn]=cnt[vtx[vn].fail]+1; // subject...
        } last=vtx[p].ch[c];
        cout<<cnt[last]<<" "; // subject...
    }
};
typedef PalindromicAutomation<int(1E6+5)> PAM;

void entry(){
    
}
int main(){
    //freopen("t1.in","r",stdin);
    //freopen("t1.out","w",stdout);
    //ll T=rd(); while(T--){
        entry();
    //}
    return 0;
}