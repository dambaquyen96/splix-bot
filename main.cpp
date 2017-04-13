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

int m = 20, n = 30; // map dimension
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
int cx[] = {-1, -1, 1, 1};
int cy[] = {-1, 1, -1, 1};
string ans_str[] = {"RIGHT", "DOWN", "LEFT", "UP"};
/// RIGHT 0
/// DOWN 1
/// LEFT 2
/// UP 3

const int PROTECT = 10000;
const int KILL_ANOTHER = 8000;
const int SAFE_MOVE = 5000;
const int SCORE = 3000;
const int GUESS_MOVE = 2000;
const int NORMAL_MOVE = 1000; 

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

int BFSBack(int pi){
    memset(c, 0, sizeof(c));
    queue<step> q;
    FO(i, 0, 4){
        int u = p[pi].x + dx[i];
        int v = p[pi].y + dy[i];
        if(a[u][v] == 2*pi) continue;
        q.push(step(u, v, -1));
        c[u][v] = 1;
    }
    while(!q.empty()){
        step s = q.front();
        q.pop();
        FO(i, 0, 4){
            if(s.last_move == (i+2)%4) continue;
            int u = s.x + dx[i];
            int v = s.y + dy[i];
            if(u < 0 || m <= u || v < 0 || n <= v) continue;
            if(c[u][v] != 0) continue;
            if(a[u][v] == 2*pi) continue;
            if(a[u][v] == 2*mine-1){
                c[u][v] = c[s.x][s.y] + 1;
                return c[u][v];
            }
            c[u][v] = c[s.x][s.y] + 1;
            q.push(step(u, v, i));
        }
    }
    return 0;
}

int BFSAttack(){
    int res = 1000000;
    FOR(pi, 1, num_player){
        if(pi == mine) continue;
        if(p[pi].x == -1 && p[pi].y == -1) continue;
        memset(atk, -1, sizeof(atk));
        queue<step> q;
        q.push(step(p[pi].x, p[pi].y, -1));
        atk[p[pi].x][p[pi].y] = 0;
        int ok = 0;
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
                if(a[u][v] == 2*mine-1){
                    atk_step = atk[u][v];
                    ok = 1;
                    break;
                }
                q.push(step(u, v, i));
            }
            if(ok) break;
        }
        res = min(res, atk_step);
    }
    return res;
}

int checkNear(int x, int y){
    FO(i, 0, 4){
        int u = x+dx[i];
        int v = y+dy[i];
        if(0 <= u && u < m && 0 <= v && v < n && a[u][v] == 2*mine-1) return 2;
    }
    FO(i, 0, 4){
        int u = x+cx[i];
        int v = y+cy[i];
        if(0 <= u && u < m && 0 <= v && v < n && a[u][v] == 2*mine) return 1;
    }
    return 0;
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
        if(a[ p[mine].x ][ p[mine].y ] == 2*mine-1){
            if(a[x][y] == 2*mine-1){
                point[i] = BFSEscape(x, y, i);
            }
            else if(a[x][y] != 0 && a[x][y] %2 == 0){
                if(BFSBack(a[x][y]/2) > 1)
                    point[i] = KILL_ANOTHER;
                else
                    point[i] = GUESS_MOVE;
            }
            else point[i] = NORMAL_MOVE;
        }
        else{
            if(a[x][y] == 2*mine-1){
                if(BFSAttack() <= 1)
                    point[i] = PROTECT;
                else
                    point[i] = SCORE;
            }
            else if(a[x][y] != 0 && a[x][y] % 2 == 0){
                if(BFSBack(a[x][y]/2) > 1)
                    point[i] = KILL_ANOTHER;
                else
                    point[i] = GUESS_MOVE;
            }
            else{
                int dis = checkNear(x, y);
                if(dis == 2)
                    point[i] = SAFE_MOVE;
                else if(dis == 1)
                    point[i] = GUESS_MOVE;
                else 
                    point[i] = NORMAL_MOVE;
            }
        }
        //point[i] = BFSBack(x, y ,i)*1000;
    }
    int maxPoint = -10000000;
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
    //string tmp;
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
