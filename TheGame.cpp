#include <iostream>
#include <fstream>
#include <Windows.h>
#include <time.h>
#include <random>
#include <string>
using namespace std;

#define height 25
#define width 100
#define stepsize 4

#define a 1
#define refresh 37
#define atktime 40
#define roomnum 8

char grid[height+1][width];
int strtx = 0, strty = 0;

struct room
{
    int x, y, w, h;
};

int distance(int x1, int x2, int y1, int y2)
{
    float D = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return (int)D;
}

void setCursorPosition(int x, int y)
{
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void DisplayMap(string map)
{
    char ch;

    //GET MAP
    ifstream file;
    file.open(map, ios::in);
    file.seekg(0, ios::beg);

    if (file.is_open())
    {
        while (!file.eof())
        {
            for (int i = 0; i < height+1; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    file.read(&ch, sizeof(char));
                    grid[i][j] = ch;
                    if (ch == '?')
                    {
                        strtx = j;
                        strty = i;
                    }
                }
            }
        }
    }
    file.close();

    //GIB MAP
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == width - 1)
            {
                _putchar_nolock('\n');
            }
            else
            {
                _putchar_nolock(grid[i][j]);
            }
        }
    }
}

void Update(int &x, int &y, int xold, int yold, bool &playing)
{
    if(grid[y][x]=='#')
    {   
        x = xold; y = yold;
    }
    if (grid[y][x] == '$')
    {
        setCursorPosition(0, 0);
        system("CLS");
        printf("GAME OVER");
        playing = false;
    }
    grid[y][x] = '0';
}

void Flush(int x, int y, int xold, int yold, int atkx, int atky, bool atkstate)
{
    Sleep(refresh);
    setCursorPosition(x, y);
    _putchar_nolock(grid[y][x]);

    if (atkstate)
    {
        Sleep(0);
        setCursorPosition(atkx, atky);
        _putchar_nolock('+');
        Sleep(atktime);
        setCursorPosition(atkx, atky);
        _putchar_nolock(grid[atky][atkx]);
        setCursorPosition(atkx, atky);
    }

    if (x != xold || y != yold)
    {
        Sleep(0);
        grid[yold][xold] = ' ';
        setCursorPosition(xold, yold);
        _putchar_nolock(' ');
    }
}

void Game(string map)
{
    DisplayMap(map);

    int x = strtx, y = strty, atkx = strtx, atky = strty;
       
    bool playing = true;
    while (playing)
    {
        if (GetKeyState('Q') & 0x8000)
        {
            setCursorPosition(0, 0);
            system("CLS");
            printf("GAME OVER");
            playing = false;
        }

        int xold = x, yold = y;
        bool atkstate = false;

        if (GetKeyState('W') & 0x8000)
        {
            y--;
            atkx = x; atky = y - a;
        }
        if (GetKeyState('A') & 0x8000)
        {
            x--;
            atkx = x - a; atky = y;
        }
        if (GetKeyState('S') & 0x8000)
        {
            y++;
            atkx = x; atky = y + a;
        }
        if (GetKeyState('D') & 0x8000)
        {
            x++;
            atkx = x + a; atky = y;
        }
        if (GetKeyState('P') & 0x8000)
        {
            atkstate = true;
        }

        Update(x, y, xold, yold, playing);

        Flush(x, y, xold, yold, atkx, atky, atkstate);

        setCursorPosition(x, y);
    }
}

void AddRoom(char (&map)[height][width], int x, int y, int w, int h)
{
    for (int i = y - h / 2; i < y + h / 2; i++)
    {
        for (int j = x - w / 2; j < x + w / 2; j++)
        {
            map[i][j] = ' ';
        }
    }
}

bool IsRoom(int x, int y, room rm)
{
    if (x > rm.x - rm.w / 2 && x < rm.x * rm.w / 2 && y > rm.y - rm.h / 2 && y < rm.y + rm.h / 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IsAnyRoom(int x, int y, room rm[roomnum])
{
    bool flag = false;
    for (int i = 0; i < roomnum; i++)
    {
        if (x > rm[i].x - rm[i].w / 2 && x < rm[i].x * rm[i].w / 2 && y > rm[i].y - rm[i].h / 2 && y < rm[i].y + rm[i].h / 2)
        {
            flag = true;
        }
    }
    return flag;
}

int ClosestRoom(room rm[roomnum], room orig, room rd[roomnum])
{
    int min = distance(0,width,0,height),addr=0;

    for (int i = roomnum; i >= 0; i--)
    {
        bool flag = true;
        for (int j = 0; j < roomnum; j++)
        {
            if (rd[j].x == rm[i].x && rd[j].y == rm[i].y)
            {
                flag = false;
            }
        }
        if ((rm[i].x != orig.x || rm[i].y != orig.y) && flag==true)
        {
            if (distance(orig.x, rm[i].x, orig.y, rm[i].y) < min)
            {
                min = distance(orig.x, rm[i].x, orig.y, rm[i].y);
                addr = i;
            }
        }
    }
    return addr;
}

room GibRoom(room rm[roomnum], int dir)
{
    int addr = 0;
    int minx, miny, min;

    switch (dir)
    {
    case 1:
        minx = 0; miny = 0;

        min = distance(minx, width, miny, height);
        for (int i = 0; i < roomnum; i++)
        {
            int dist = distance(minx, rm[i].x, miny, rm[i].x);
            if (dist < min)
            {
                min = dist;
                addr = i;
            }
        }

        return rm[addr];
        break;
    case 2:
        minx = width; miny = 0;

        min = distance(minx, width, miny, height);
        for (int i = 0; i < roomnum; i++)
        {
            int dist = distance(minx, rm[i].x, miny, rm[i].x);
            if (dist < min)
            {
                min = dist;
                addr = i;
            }
        }

        return rm[addr];
        break;
    case 3:
        minx = width; miny = height;

        min = distance(minx, width, miny, height);
        for (int i = 0; i < roomnum; i++)
        {
            int dist = distance(minx, rm[i].x, miny, rm[i].x);
            if (dist < min)
            {
                min = dist;
                addr = i;
            }
        }

        return rm[addr];
        break;
    case 4:
        minx = 0, miny = height;

        min = distance(minx, width, miny, height);
        for (int i = 0; i < roomnum; i++)
        {
            int dist = distance(minx, rm[i].x, miny, rm[i].x);
            if (dist < min)
            {
                min = dist;
                addr = i;
            }
        }

        return rm[addr];
        break;
    default:
        break;
    }
}

bool IsOk(char (&map)[height][width], int x, int y, room rm[roomnum])
{
    if (x > 0 && x < width - 1 && y>0 && y < height - 1)
    {
        if (map[y][x]!=' ')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void brancher(char (&map)[height][width], room rm[roomnum], int x, int y, int t, int m)
{
    if (IsOk(map, x, y - t, rm))
    {
        for (int i = 0; i < t; i++)
        {
            map[y - i][x] = ' ';
        }
        brancher(map, rm, x, y - t, t,m);
    }
    if (IsOk(map, x, y + t, rm))
    {
        for (int i = 0; i < t; i++)
        {
            map[y + i][x] = ' ';
        }
        brancher(map, rm, x, y + t, t,m);
    }
    if (IsOk(map, x + m, y, rm))
    {
        for (int i = 0; i < m; i++)
        {
            map[y][x + i] = ' ';
        }
        brancher(map, rm, x + m, y, t,m);
    }
    if (IsOk(map, x - m, y , rm))
    {
        for (int i = 0; i < m; i++)
        {
            map[y][x - i] = ' ';
        }
        brancher(map, rm, x - m, y, t,m);
    }
}

void AddPath(char (&map)[height][width], room rm[roomnum])
{   
    brancher(map, rm, 1, 1, stepsize, stepsize*2);
}

string MapMaker()
{
    srand(time(0));
    Sleep(1000);

    int N = roomnum;
    room R[roomnum];

    char Map[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j != width - 1)
            {
                Map[i][j] = '#';
            }
            else
            {
                Map[i][j] = '\n';
            }
        }
    }

    for (int i = 0; i < N; i++)
    {
        go:
        bool flag = true;

        int t = 2;
        int maxw = 40; int maxh = 8;
        int minw = 10; int minh = 4;

        int w = rand() % (maxw - minw + 1) + minw;
        int h = rand() % (maxh - minh + 1) + minh;

        int x = rand() % (w - width) + w / 2;
        int y = rand() % (h - height) + h / 2 + 1;

        for (int i = y - h / 2 -t; i < y + h / 2 + t; i++)
        {
            for (int j = x - w / 2 -t; j < x + w / 2 +t; j++)
            {
                if (Map[i][j] == ' ' || x + w / 2 == width - 1 || y + h / 2 == height || x - w / 2 == 0 || y - h / 2 == 0)
                {
                    flag = false;
                }
            }
        }

        if (flag == true)
        {
            R[i].h = h; R[i].w = w; R[i].x = x; R[i].y = y;
            AddRoom(Map, x, y, w, h);
        }
        else
        {
            goto go;
        }
    }

    room start = GibRoom(R, 1), newroom, ordR[roomnum];
    for (int i = 0; i < roomnum; i++)
    {
        ordR[i] = start;
        newroom = R[ClosestRoom(R, start, ordR)];
        start = newroom;
    }

    AddPath(Map, ordR);

    int dir = 4;

    Map[1][1] = '?';
    Map[ordR[roomnum-1].y][ordR[roomnum - 1].x]='$';

    //bug fix
    room Bug = ordR[roomnum - 1];
    for (int i = 0; i < stepsize; i++)
    {
        Map[Bug.y + (int)(Bug.h / 2) + i][Bug.x] = ' ';
        Map[Bug.y][Bug.x - (int)(Bug.w / 2) - i] = ' ';
        Map[Bug.y][Bug.x + (int)(Bug.w / 2) + i] = ' ';
        Map[Bug.y - (int)(Bug.h / 2) - i][Bug.x] = ' ';
    }
    for (int i = 0; i < width-1; i++)
    {
        Map[0][i] = '#';
        Map[height-1][i] = '#';
    }
    for (int i = 0; i < height - 1; i++)
    {
        Map[i][width-2] = '#';
        Map[i][0] = '#';
    }

    string name = "map" + to_string(rand()) + ".txt";
    
    fstream file;
    file.open(name, ios::out);

    if (file.is_open())
    {    
       for (int i = 0; i < height; i++)
       {
           for (int j = 0; j < width; j++)
           {
               file.write(&Map[i][j], sizeof(char));
           }
       }
    }

    file.close();
    return name;
}

void main()
{
    Game(MapMaker());
}
