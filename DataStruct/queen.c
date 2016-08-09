#include <stdio.h>

#define N 8

typedef struct _tag_Pos
{
    int ios;
    int jos;
} Pos;

static char board[N+2][N+2];
static Pos pos[] = { {-1, -1}, {-1, 0}, {-1, 1} };
static int count = 0;

void init()
{
    int i = 0;
    int j = 0;
    
    for(i=0; i<N+2; i++)
    {
        board[0][i] = '#';
        board[N+1][i] = '#';
        board[i][0] = '#';
        board[i][N+1] = '#';
    }
    
    for(i=1; i<=N; i++)
    {
        for(j=1; j<=N; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void display()
{
    int i = 0;
    int j = 1;
    
    for(i=0; i<N+2; i++)
    {
        for(j=0; j<N+2; j++)
        {
            printf("%c", board[i][j]);
        }
        
        printf("\n");
    }
}

int check(int i, int j)
{
    int ret = 1;
    int p = 0;
    
    for(p=0; p<3; p++)
    {
        int ni = i;
        int nj = j;
        
        while( ret && (board[ni][nj] != '#') )
        {
            ni = ni + pos[p].ios;
            nj = nj + pos[p].jos;
            
            ret = ret && (board[ni][nj] != '*');
        }
    }
    
    return ret;
}

void find(int i)
{
    int j = 0;
    
    if( i > N )
    {
        count++;
        
        printf("Solution: %d\n", count);
        
        display();
        
        getchar();
    }
    else
    {
        for(j=1; j<=N; j++)
        {
            if( check(i, j) )
            {
                board[i][j] = '*';
                
                find(i+1);
                
                board[i][j] = ' ';
            }
        }
    }
}

int main()
{
    init();
    find(1);
    
    return 0;
}
