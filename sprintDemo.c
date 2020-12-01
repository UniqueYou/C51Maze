#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//冲刺部分
#define SIZE 8
unsigned char  height[SIZE][SIZE];
unsigned char  map[SIZE][SIZE] ={
			{0x0e,0x0e,0x0d,0x05,0x06,0x0c,0x05,0x07},
			{0x0a,0x08,0x05,0x05,0x02,0x08,0x05,0x06},
			{0x09,0x01,0x06,0x0e,0x08,0x02,0x0c,0x02},
			{0x0c,0x04,0x02,0x0a,0x0a,0x0a,0x0a,0x0a},
			{0x0a,0x0a,0x08,0x01,0x00,0x02,0x0a,0x0b},
			{0x0a,0x0b,0x08,0x06,0x0a,0x08,0x00,0x06},
			{0x09,0x05,0x02,0x0a,0x0b,0x0b,0x0a,0x0a},			
			{0x0d,0x05,0x03,0x09,0x05,0x05,0x03,0x0b}					
			};			

//迷宫节点结构体
struct queue{//迷宫节点
	unsigned char x;//横坐标
	unsigned char y;//纵坐标
};

//保存绝对方向到迷宫数据中
void saveDir(unsigned char i,unsigned char x,unsigned char y)
{
	
	switch(i)
	{
		case 0:map[y][x] =  map[y][x]&0xef;break;
		case 1:map[y][x] =  map[y][x]&0xdf;break;
		case 2:map[y][x] =  map[y][x]&0xbf;break;
		case 3:map[y][x] =  map[y][x]&0x7f;break;
	}

}

//判断某个方向是否为墙
unsigned char isWall(unsigned char  dir,unsigned char x,unsigned char y)
{
	return (map[y][x]>>dir)&0x01;
}

//初始化迷宫数据
void initMap(unsigned char mapData[SIZE][SIZE])
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			mapData[i][j] = 0xff;//初始化默认四个方向都有墙		
		}	
	}	
}
//广度优先算法
void bfs()
{	
	struct queue queueNum[15];//初始化队列
	unsigned char queueLen = 1;//初始化队列长度
	struct queue queueHead;//初始化队列头
	struct queue temp;//临时变量
	struct queue nowPos;

	unsigned char i,j;		
	initMap(height);//初始化等高表
	height[0][0] = 0;

	queueNum[0].x = 0;//起始坐标入队列
	queueNum[0].y = 0;	
	
	while(queueLen>0)//当队列不为空的时候
	{	
		queueHead = queueNum[0];//队首元素出队
		queueLen--;
		//将队列元素前移一个单位，因为用的是数组
		for(i=0;i<queueLen;i++)
			queueNum[i] = queueNum[i+1];		
		//判断四个方向是否能够入队
		for(j=0;j<4;j++)
		{
			temp=queueHead;
			switch(j)
			{
				case 0:temp.y = queueHead.y+1;break;
				case 1:temp.x = queueHead.x+1;break;
				case 2:temp.y = queueHead.y-1;break;
				case 3:temp.x = queueHead.x-1;break;		
			}	
									
			if(temp.x>=SIZE||temp.y>=SIZE||temp.x<0||temp.y<0) 
				continue;
			
			if(!isWall(j,queueHead.x,queueHead.y) && height[temp.y][temp.x]==0xff)	//如果可以走且第一次走
			{
				height[temp.y][temp.x] = height[queueHead.y][queueHead.x]+1;
				queueNum[queueLen] = temp;//坐标入队
				queueLen++;
			}
		}
	}	
}


//反向查找，寻找最短路径
void findWay()
{
	struct queue nowPos,temp;
	unsigned char i,j;
	nowPos.y=SIZE-1;
	nowPos.x=SIZE-1;
	
	
	
	while(!(nowPos.x==0&&nowPos.y==0))//如果还没有到起点
	{
		for(i=0;i<4;i++)
		{
			temp = nowPos;
			switch(i)
			{
				case 0:temp.y = nowPos.y+1;break;
				case 1:temp.x = nowPos.x+1;break;
				case 2:temp.y = nowPos.y-1;break;
				case 3:temp.x = nowPos.x-1;break;		
			}
		
			if(temp.x>=SIZE||temp.y>=SIZE||temp.x<0||temp.y<0) 
			continue;

			
			//如果可以走并且等高表递减
			if(!isWall(i,nowPos.x,nowPos.y) && (height[temp.y][temp.x]==height[nowPos.y][nowPos.x]-1))
			{
				map[temp.y][temp.x] |= 0xf0;//初始化迷宫高四位
				saveDir(i,temp.x,temp.y);
				nowPos = temp;
				break;
			}
		}
		
		 
	}
	
	
}


//输出数组
void printArray(unsigned char map[SIZE][SIZE]) 
{
	char  i,j;
	
	for(i=SIZE-1;i>=0;i--)
	{
		for(j=0;j<SIZE;j++)
		printf("%d\t",map[i][j]);
		printf("\n");
	}
	
}

//输出迷宫的绝对方向
void printDir(unsigned char map[SIZE][SIZE]) 
{
	char  i,j;
	unsigned char temp;
	for(i=SIZE-1;i>=0;i--)
	{
		for(j=0;j<SIZE;j++)
		{
			temp = map[i][j]&0xf0;
			switch(temp)
			{
				case 0xe0:printf("上\t");break;
				case 0xd0:printf("右\t");break;
				case 0xb0:printf("下\t");break;
				case 0x70:printf("左\t");break;
				default :printf("[]\t");
			}
		}
		printf("\n");
	}
}


void main()
{	
	bfs();
	printf("输出等高表:\n");
	printArray(height);
	printf("输出迷宫绝对方向\n");
	findWay(); 
	printDir(map) ;
	
}


