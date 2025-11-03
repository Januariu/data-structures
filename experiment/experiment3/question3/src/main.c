#include "stack.h"
#include "time.h"

typedef struct maze{
    int** map;
    int row ,cols;
}*MazePtr;

typedef struct {
    int x,y;
}coord;

typedef struct {
    stack x,y;
}path;


MazePtr init_maze(int row, int cols){
    MazePtr m = (MazePtr)malloc(sizeof(struct maze));

    m->map = (int**)malloc(sizeof(int *) * row);

    if(m->map == NULL){        
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i<row; i++){
        m->map[i] = (int *)malloc(sizeof(int) * cols);
        if(m->map[i] == NULL){        
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        for(int j=0; j<cols; j++){
            m->map[i][j] = 0;
        }
    }

    m->cols = cols;
    m->row = row;

    return m;
}

void input_maze(MazePtr m){
    printf("pleace input maze (1 is wall, 0 is passable, 2 is start and end)\n");
    for(int i=0; i < m->row; i++)
        for(int j=0; j < m->cols; j++)
            scanf("%d", &m->map[i][j]);

}

void print_maze(MazePtr m){
    for(int i=0; i < m->row; i++){
        for(int j=0; j < m->cols; j++)
            printf("%d ", m->map[i][j]);
        printf("\n");
        }
}

coord coord_add(coord a, coord b){
    a.x += b.x;
    a.y += b.y;
    return a;
}

bool dfs(MazePtr m, coord current, coord end, MazePtr vis, path* p){
    

    vis->map[current.x][current.y] = 1;
    push(p->x, current.x);
    push(p->y, current.y);

    if (current.x == end.x && current.y == end.y) return true;

    coord adj[4] ={{0,1},{0,-1},{-1,0}, {1,0}};
    for(int i = 0; i<4; i++){
        coord temp = coord_add(current, adj[i]);
        if (temp.x < 0 || temp.x >= m->row || temp.y < 0 || temp.y >= m->cols) continue;
        if (m->map[temp.x][temp.y] == 1) continue;
        if(!vis->map[temp.x][temp.y] && dfs(m, temp, end, vis, p))
        return true;
    }

    pop(p->x);
    pop(p->y);

    return false;
}

bool solve_maze(MazePtr m){
    
    //get start and end
    coord start = {-1,-1}, end;
    for(int i=0; i < m->row; i++){
        for(int j=0; j < m->cols; j++){
            if(m->map[i][j] == 2){
                if(start.x == -1 || start.y == -1){
                    start.x = i;
                    start.y = j;
                }else {
                    end.x = i;
                    end.y = j;
                }
            }
        }
    }

    printf("start is (%d, %d), end is (%d, %d)\n",start.x, start.y, end.x, end.y);
    MazePtr vis = init_maze(m->row, m->cols);
    path* p = (path *)malloc(sizeof(path));
    p->x = init_stack();
    p->y = init_stack();
    bool is_solve = dfs(m, start, end, vis, p);
    if(is_solve == true){
        while(!stack_empty(p->x)){
            m->map[pop(p->x)][pop(p->y)] = 9;
        }

        print_maze(m);
        
        return true;
    }else{ 
        printf("no solve");
        return false;
    }
}


MazePtr generate_maze(int row, int cols, double wall_density) {
    /*
    随机生成迷宫：
    - row × cols 的二维数组
    - 按 wall_density 比例生成墙 (1)
    - 其他为通路 (0)
    - 起点 (2) 和终点 (2) 随机分配在两个不同的通路位置
    */

    MazePtr m = init_maze(row, cols);
    srand((unsigned)time(NULL));  // 设置随机种子

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < cols; j++) {
            // 边界设为墙，保证外圈封闭
            if (i == 0 || i == row - 1 || j == 0 || j == cols - 1)
                m->map[i][j] = 1;
            else
                m->map[i][j] = ((double)rand() / RAND_MAX < wall_density) ? 1 : 0;
        }
    }

    // 随机生成起点和终点
    coord start, end;
    do {
        start.x = rand() % row;
        start.y = rand() % cols;
    } while (m->map[start.x][start.y] == 1);

    do {
        end.x = rand() % row;
        end.y = rand() % cols;
    } while ((end.x == start.x && end.y == start.y) || m->map[end.x][end.y] == 1);

    // 设置起点和终点
    m->map[start.x][start.y] = 2;
    m->map[end.x][end.y] = 2;

    printf("Random maze generated.\nStart: (%d, %d), End: (%d, %d)\n",
           start.x, start.y, end.x, end.y);

    return m;
}


void test_rand_maze(){
    MazePtr m = generate_maze(10, 10, 0.3);
    print_maze(m);
    // input_maze(m);
    solve_maze(m);
    // print_maze(m);
}

void test_maze(){
    int row, cols;
    printf("please imput maze row and cols:\n");
    scanf("%d %d",&row, &cols);
    MazePtr m = init_maze(row, cols);
    // print_maze(m);
    input_maze(m);
    solve_maze(m);
    // print_maze(m);
}

int main(){
    test_maze();
    return 0;
}

/*
1 1 1 1 1 1 1 1 1 1
2 0 0 0 0 0 1 0 0 1
1 1 1 1 1 0 1 0 1 1
1 0 0 0 0 0 1 0 0 1
1 0 1 0 1 0 1 0 0 2
1 0 0 0 0 0 1 0 1 1 
1 0 0 1 0 0 0 0 0 1
1 0 1 0 1 0 1 0 0 1
1 0 0 0 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1
*/