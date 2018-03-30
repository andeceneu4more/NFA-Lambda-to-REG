#include <fstream>
#include <list>
#include <vector>
#define nmax 500
using namespace std;
class tranzitie
{
private:
    string s;
    int urm,sens;
public:
    tranzitie(string,int,int);
    tranzitie(const tranzitie&);
    ~tranzitie();
    string get_s();
    int get_urm();
    int get_sens();
    void set_s(string);
    void set_urm(int);
    void set_sens(int);
    friend istream& operator>>(istream&,tranzitie&);
    friend ostream& operator<<(ostream&,tranzitie&);
    friend bool operator==(const tranzitie&,const tranzitie&);
};
tranzitie::tranzitie(string ss="",int uurm=0,int ssens=0)
{
    s=ss;
    urm=uurm;
    sens=ssens;
}
tranzitie::tranzitie(const tranzitie &T)
{
    s=T.s;
    urm=T.urm;
    sens=T.sens;
}
tranzitie::~tranzitie()
{
    s="";
    urm=0;
    sens=0;
}
string tranzitie::get_s()
{
    return s;
}
int tranzitie::get_urm()
{
    return urm;
}
int tranzitie::get_sens()
{
    return sens;
}
void tranzitie::set_s(string ss)
{
    s=ss;
}
void tranzitie::set_urm(int uurm)
{
    urm=uurm;
}
void tranzitie::set_sens(int ssens)
{
    sens=ssens;
}
istream& operator>>(istream &in, tranzitie &T)
{
    in>>T.urm>>T.s;
    return in;
}
ostream& operator<<(ostream &out, tranzitie &T)
{
    out<<T.urm<<' '<<T.sens<<' '<<T.s;
    return out;
}
bool operator==(const tranzitie &A,const tranzitie &B)
{
    return ((A.urm==B.urm) && (A.sens==B.sens) && (A.s==B.s));
}
class automat
{
private:
    list <tranzitie> Q[nmax];
    int qs,qf,n;
public:
    automat();
    automat(automat&);
    ~automat();
    void adauga(int,tranzitie&);
    friend istream& operator>>(istream&,automat&);
    friend ostream& operator<<(ostream&,automat&);
    void initializeaza();
    void sterge(int);
    void elimina(int);
    string gata();
    string REG();
};
automat::automat()
{
    qs=qf=n=0;
}
automat::automat(automat &A)
{
    n=A.n;
    qs=A.qs;
    qf=A.qf;
    int i;
    list <tranzitie>::iterator it;
    for (i=1;i<=n;i++)
    {
        Q[i].clear();
        for(it=A.Q[i].begin();it!=A.Q[i].end();it++)
        {
            Q[i].push_back(*it);
        }
    }
}
automat::~automat()
{
    for (int i=0;i<n;i++)
        Q[i].clear();
}
void automat::adauga(int x, tranzitie &T)
{
    T.set_sens(1);
    Q[x].push_back(T);
    int aux=T.get_urm();
    swap(aux,x);
    T.set_urm(aux);
    T.set_sens(-1);
    Q[x].push_back(T);
}
istream& operator>>(istream& in, automat& A)
{
    string s;
    int F,i,x,m;
    list <tranzitie>::iterator it;
    tranzitie T("#");
    in>>A.n;
    A.n--;
    in.ignore();
    in>>s;
    //alfabetul
    in>>A.qs;
    in>>F;
    if (F!=1)
    {
        A.qf=++A.n;
        //n+1=noua si singura stare finala
        T.set_urm(A.qf);
        for (i=1;i<=F;i++)
        {
            in>>x;
            A.adauga(x,T);
        }
    }
    else
        in>>A.qf;
    in>>m;
    for (i=1;i<=m;i++)
    {
        in>>x>>T;
        A.adauga(x,T);
    }
    return in;
}
ostream& operator<<(ostream &out,automat &A)
{
    int i;
    list <tranzitie>::iterator it;
    out<<A.n+1<<' '<<A.qs<<' '<<A.qf<<'\n';
    for (i=0;i<=A.n;i++)
    {
        for (it=A.Q[i].begin();it!=A.Q[i].end();it++)
        {
            out<<i<<' '<<(*it)<<'\n';
        }
    }
    return out;
}
void automat::initializeaza()
{
    list <tranzitie>::iterator it;
    bool ok;
    tranzitie T("#");
    //in starea initiala nu intra nimeni
    ok=true;
    for (it=Q[qs].begin();it!=Q[qs].end()&&ok;it++)
    {
        if ((*it).get_sens()==-1)
            ok=false;
    }
    if (!ok)
    {
        ++n;
        T.set_s("#");
        T.set_urm(qs);
        adauga(n,T);
        qs=n;
    }
    //din starea finala nu iese nimeni
    ok=true;
    for (it=Q[qf].begin();it!=Q[qf].end()&&ok;it++)
    {
        if ((*it).get_sens()==1)
            ok=false;
    }
    if (!ok)
    {
        ++n;
        T.set_s("#");
        T.set_urm(n);
        adauga(qf,T);
        qf=n;
    }
}
void automat::sterge(int x)
{
    list <tranzitie>::iterator it;
    int y;
    tranzitie A;
    for (it=Q[x].begin();it!=Q[x].end();it++)
    {
        y=(*it).get_urm();
        if (y!=x)
        {
            A.set_urm(x);
            A.set_sens(-(*it).get_sens());
            A.set_s((*it).get_s());
            Q[y].remove(A);
        }
    }
    Q[x].clear();
}
void automat::elimina(int x)
{
    string rkk="",nou;
    vector <tranzitie> intra;
    vector <tranzitie> bucla;
    vector <tranzitie> ies;
    list <tranzitie>::iterator it;
    tranzitie A,B,C;
    unsigned int i,j;
    for (it=Q[x].begin();it!=Q[x].end();it++)
    {
        A=*it;
        if (A.get_urm()==x)
        {
            bucla.push_back(A);
            if ( A.get_s()!="#" && A.get_sens()==1 )
                    rkk+=(A.get_s()+" + ");
        }
        else
        {
            if (A.get_sens()==-1)
                intra.push_back(A);
            else
                ies.push_back(A);
        }
    }
    sterge(x);
    if (rkk.size())
    {
        j=rkk.size();
        rkk.replace(j-3,j-3,"");
        rkk=" ( "+rkk+" )* ";
    }
    else
        rkk=" ";
    for (i=0;i<intra.size();i++)
    {
        A=intra[i];
        if (A.get_s()=="#")
            A.set_s("");
        for (j=0;j<ies.size();j++)
        {
            B=ies[j];
            if (B.get_s()=="#")
                B.set_s("");
            nou=A.get_s()+rkk+B.get_s();
            C.set_urm(B.get_urm());
            C.set_s(nou);
            C.set_sens(1);
            x=A.get_urm();
            adauga(x,C);
            //(A.urm,B.urm) cu stringul nou
        }
    }
    intra.clear();
    bucla.clear();
    ies.clear();
}
string automat::gata()
{
    string ss="";
    list <tranzitie>::iterator it;
    for (it=Q[qs].begin();it!=Q[qs].end();it++)
    {
        if ((*it).get_s()!="#")
            ss=ss+"("+(*it).get_s()+") + ";
    }
    int i=ss.size();
    ss.replace(i-3,i-3,"");
    return ss;
}
string automat::REG()
{
    initializeaza();
    int i;
    for (i=0;i<=n;i++)
        if ( i!=qs && i!=qf )
            elimina(i);
    return gata();
}
int main()
{
    ifstream fin("conversie.in");
    ofstream fout("conversie.out");
    automat A;
    fin>>A;
    automat B(A);
    fout<<A.REG()<<'\n';
    //fout<<B;
    fin.close();
    fout.close();
    return 0;
}
