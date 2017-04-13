#include <bits/stdc++.h>
#define LL long long
#define ULL unsigned long long
#define FOR(i,a,b) for(int i=a;i<=b;i++)
#define FO(i,a,b) for(int i=a;i<b;i++)
#define FOD(i,a,b) for(int i=a;i>b;i--)
#define FORD(i,a,b) for(int i=a;i>=b;i--)
#define FORV(i,a) for(typeof(a.begin()) i = a.begin(); i != a.end(); i++)
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define debug cout << "YES" << endl

using namespace std;

typedef pair<int,int>II;
template<class T> int getbit(T s, int i) { return (s >> i) & 1; }
template<class T> T onbit(T s, int i) { return s | (T(1) << i); }
template<class T> T offbit(T s, int i) { return s & (~(T(1) << i)); }
template<class T> int cntbit(T s) { return __builtin_popcount(s);}
template<class T> T gcd(T a, T b){ T r; while (b != 0) { r = a % b; a = b; b = r; } return a;}

const long double PI = 2*acos(0.0);
const long double eps = 1e-15;
//const int infi = 1e9;
const LL Linfi = 1e18;
const LL MOD = 1000000021;
const int c1 = 31;
const int c2 = 37;
#define maxn 5010

struct player{
    int number;
    int x;
    int y;
};

struct step{
    int x;
    int y;
    int last_move;
    step(int _x, int _y, int _last_move){
        x = _x;
        y = _y;
        last_move = _last_move;
    }
};

int m = 20, n = 20; // map dimension
int a[100][100];    // map
int num_player;     // number of players
int mine;           // id of this player
player p[10];       // list player
int c[100][100];    // cost array to BFS
int atk[100][100];  // cost array to BFS for another player
int track[100][100];// cost tracking of BFS
int pre[10000];     // backtrach for BFS
int last_move;      // last move of this player
int point[10];      // point of each direction
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
string ans_str[] = {"RIGHT", "DOWN", "LEFT", "UP"};
/// UP 0
/// RIGHT 1
/// DOWN 2
/// LEFT 3

int pair2int(II tmp){
    return tmp.fi * n + tmp.se;
}

II int2pair(int x){
    return mp(x/n, x%n);
}

int BFSEscape(int x, int y, int last_move){
    memset(c, 0, sizeof(c));
    queue<step> q;
    q.push(step(x, y, last_move));
    c[x][y] = 1;
    while(!q.empty()){
        step s = q.front();
        q.pop();
        FO(i, 0, 4){
            if(s.last_move == (i+2)%4) continue;
            int u = s.x + dx[i];
            int v = s.y + dy[i];
            if(u < 0 || m <= u || v < 0 || n <= v) continue;
            if(c[u][v] != 0) continue;
            if(a[u][v] == 2*mine) continue;
            if(a[u][v] == 2*mine-1){
                c[u][v] = c[s.x][s.y] + 1;
                q.push(step(u, v, i));
                continue;
            }
            return c[s.x][s.y] + 1;
        }
    }
    return c[x][y];
}

int BFSBack(int x, int y, int last_move){
    memset(c, 0, sizeof(c));
    memset(pre, -1, sizeof(pre));
    memset(track, 0, sizeof(track));
    queue<step> q;
    q.push(step(x, y, last_move));
    c[x][y] = 1;
    int ok = 0;
    int total;
    while(!q.empty()){
        step s = q.front();
        q.pop();
        FO(i, 0, 4){
            if(s.last_move == (i+2)%4) continue;
            int u = s.x + dx[i];
            int v = s.y + dy[i];
            if(u < 0 || m <= u || v < 0 || n <= v) continue;
            if(c[u][v] != 0) continue;
            if(a[u][v] == 2*mine) continue;
            if(a[u][v] == 2*mine-1){
                c[u][v] = c[s.x][s.y] + 1;
                total = c[u][v];
                pre[pair2int(mp(u,v))] = pair2int(mp(s.x, s.y));
                while(pre[pair2int(mp(u,v))] != -1){
                    track[u][v] = c[u][v];
                    int tmp = pre[pair2int(mp(u,v))];
                    u = int2pair(tmp).fi;
                    v = int2pair(tmp).se;
                }
                ok = 1;
                break;
            }
            c[u][v] = c[s.x][s.y] + 1;
            pre[pair2int(mp(u,v))] = pair2int(mp(s.x, s.y));
            q.push(step(u, v, i));
        }
        if(ok) break;
    }

    FOR(pi, 1, num_player){
        if(pi == mine) continue;
        if(p[pi].x == -1 && p[pi].y == -1) continue;
        memset(atk, -1, sizeof(atk));
        q = queue<step>();
        q.push(step(p[pi].x, p[pi].y, -1));
        atk[p[pi].x][p[pi].y] = 0;
        int atk_step;
        while(!q.empty()){
            step s = q.front();
            q.pop();
            FO(i, 0, 4){
                if(s.last_move == (i+2)%4) continue;
                int u = s.x + dx[i];
                int v = s.y + dy[i];
                if(u < 0 || m <= u || v < 0 || n <= v) continue;
                if(atk[u][v] != -1) continue;
                if(a[u][v] == 2*pi-1) continue;
                atk[u][v] = atk[s.x][s.y] + 1;
                if(atk[u][v] >= total) continue;
                if(a[u][v] == 2*mine-1 && atk[u][v] < total){
                    return -1;
                }
                if(0 < track[u][v] && track[u][v] <= atk[u][v] && a[u][v] < total){
                    return -1;
                }
                q.push(step(u, v, i));
            }
        }
    }
    return total;
}

void process(){
    memset(point, -1, sizeof(point));
    FO(i, 0, 4){
        if(last_move != -1 && last_move == (i+2)%4){
            point[i] = -1000000;
            continue;
        }
        int x = p[mine].x + dx[i];
        int y = p[mine].y + dy[i];
        if(x < 0 || m <= x || y < 0 || n <= y){
            point[i] = -1000000;
            continue;
        };
        if(a[x][y] == 2*mine){
            point[i] = -1000000;
            continue;
        }
        if(a[x][y] == 2*mine-1){
            point[i] = BFSEscape(x, y, i);
            continue;
        }
        point[i] = BFSBack(x, y ,i)*1000;
    }
    int maxPoint = -1;
    int ans = 0;
    FO(i, 0, 4){
        //cout << ans_str[i] << " " << point[i] << endl;
        if(point[i] > maxPoint){
            maxPoint = point[i];
            ans = i;
        }
    }
    cout << ans_str[ans] << endl;
    last_move = ans;
}

int main(){
    ios::sync_with_stdio(false);
    #ifndef ONLINE_JUDGE
//    freopen("test.inp","r",stdin);
//    freopen("test.out","w",stdout);
    #endif

    cin >> num_player >> mine;
    last_move = -1;
    while(true){
        FO(i, 0, m){
            string s;
            cin >> s;
            FO(j, 0, n) a[i][j] = s[j] - '0';
        }
        FOR(i, 1, num_player){
            p[i].number = i;
            cin >> p[i].x >> p[i].y;
        }
        process();
    }


    return 0;
}
