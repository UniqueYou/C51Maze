
//��̲���
unsigned char xdata height[SIZE][SIZE];

//�Թ��ڵ�ṹ��
struct queue{//�Թ��ڵ�
	 unsigned char x;//������
	 unsigned char y;//������
};

void bfs();
void findWay();
unsigned char isWall(unsigned char  dir,unsigned char x,unsigned char y);
void runWay();
void initMazeHigh(unsigned char mapData[SIZE][SIZE]);