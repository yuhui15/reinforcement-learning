#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<map>
using namespace std;
map < int, map<string, map<int, double> > >Q, P1, P2;
map<string,int>sum;
string s = "";
int turn_x(int number)
{
    return number / 3;
}
int turn_y(int number)
{
    return number % 3;
}
bool out(int x, int y)
{
    return x < 0 || y < 0 || x >= 3 || y >= 3;
}
bool check_horizon(string s,char flag)
{
    for(int x=0; x<3 ;x++)
        for (int y = 0; y < 3; y++)
        {
            int x1 = x, y1 = y,cnt=1;
            for (int i = 0; i < 3; i++)
            {
                if (out(x1, y1) || s[x1 * 3 + y1] != flag) { cnt = 0; break; }
                y1++;
            }
            if (cnt) return 1;
        }
    return 0;
}
bool check_vertice(string s, char flag)
{
    for(int x=0; x<3; x++)
        for (int y = 0; y < 3; y++)
        {
            int x1 = x, y1 = y,cnt=1;
            for (int i = 0; i < 3; i++)
            {
                if (out(x1, y1)|| s[x1 * 3 + y1] != flag) { cnt = 0; break; }
                x1++;
            }
            if (cnt) return 1;
        }
    return 0;
}
bool check_digonal(string s, char flag)
{
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
        {
            int x1 = x, y1 = y, cnt = 1;
            for (int i = 0; i < 3; i++)
            {
                if (out(x1, y1) || s[x1 * 3 + y1] != flag) { cnt = 0; break; }
                x1++, y1++;
            }
            if (cnt) return 1;
        }
    return 0;
}
bool check(string S, char flag)
{
    if(check_horizon(S, flag)) return 1;
    if(check_vertice(S, flag)) return 1;
    if(check_digonal(S,flag)) return 1;
    return 0;
}
void memory_search(int round, string S, int A)
{   
	if(S[A]!='0') return ;
    if (Q[round][S][A] != -1) return;
    string S1 = S;
    S1[A] = '1';
    if (check(S1,'1')) { Q[round][S][A] = 1; return;  }
    else if (check(S1,'2')) { Q[round][S][A] = 0; return; }
    Q[round][S][A] = 0;
    double part_1 = 0;
    for (int B = 0; B < 9; B++)
    {
        if (S1[B] == '0')
        {
            double part_2 = 0;
            string S2 = S1;
            S2[B] = '2';
            for (int a = 0; a < 9; a++)
            {
                if (S2[a] == '0')
                {
                    memory_search(round, S2, a);
                    part_2 += P1[round][S2][a] * Q[round][S2][a];
                }
            }

            Q[round][S][A] += P2[round][S1][B] * part_2;
        }
    }
    return;
}
string inverse(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '1') s[i] = '2';
        else if (s[i] == '2') s[i] = '1';
    }
    return s;
}
int count(string s)
{
    int count = 0;
    for (int i = 0; i < s.length(); i++)
        if (s[i] == '0') count++;
    return count;
}
void dfs(int round, int position)
{
    if (position == 9)
    {
        for (int a = 0; a < 9; a++)
        {
            Q[round][s][a] = -1;
            P1[round][s][a] = -1;
            P2[round][s][a] = -1;
        }
        return;
    }
    char list[3] = { '0','1','2' };
    for (int i = 0; i < 3; i++)
    {
        s = s + list[i];
        dfs(round, position + 1);
        s=s.substr(0,s.length()-1);
    }
}
void dfs2(int round, int position,int flag)
{
    if (position == 9)
    {
    	sum[s]=0;
    	if(round>0)
    	for(int a=0; a<9; a++)
    	{
    	if(Q[round-1][s][a]>=0) sum[s]+=Q[round-1][s][a];
        if(Q[round-1][inverse(s)][a]>=0) sum[s]+=Q[round-1][inverse(s)][a];
		}
        for (int a = 0; a < 9; a++)
        {
            if (flag == 1)
            {
                if (round == 0)
                {
                    P1[round][s][a] = 1 / double(count(s));
                    P2[round][s][a] = 1 / double(count(s));
                }
                else
                {
                    if(Q[round - 1][s][a]>=0&&sum[s]>0) P1[round][s][a] = Q[round - 1][s][a]/sum[s];
                    else P1[round][s][a] = 0;
                    if(Q[round - 1][inverse(s)][a]>=0&&sum[s]>0) P2[round][s][a] = Q[round - 1][inverse(s)][a]/sum[s];
                	else P2[round][s][a] = 0;
				}	
            }
            if (flag == 2)
            {
                memory_search(round, s, a);
                if(round==9)
                cout<<s<<' '<<a<<' '<<Q[round][s][a]<<endl;
            }
        }
        return;
    }
    char list[3] = { '0','1','2' };
    for (int i = 0; i < 3; i++)
    {
        s = s + list[i];
        dfs2(round, position + 1,flag);
        s=s.substr(0,s.length()-1);
    }
}
int main()
{
	freopen("output_data","w",stdout);
    for (int round = 0; round < 10; round++)
        dfs(round, 0);
    
    for (int round = 0; round < 10; round++)
    {
        dfs2(round, 0,1);
        dfs2(round, 0,2);
    }
    
    return 0;
}
