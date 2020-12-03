
//冲刺部分
unsigned char xdata height[SIZE][SIZE];

//迷宫节点结构体
struct queue{//迷宫节点
	 unsigned char x;//横坐标
	 unsigned char y;//纵坐标
};

void bfs();
void findWay();
unsigned char isWall(unsigned char  dir,unsigned char x,unsigned char y);
void runWay();
void initMazeHigh(unsigned char mapData[SIZE][SIZE]);