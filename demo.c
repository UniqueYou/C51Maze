
#include <stdio.h>
#include <reg52.h>
#include <string.h>
#include <stdlib.h>
#include "mouse.h"
#include "demo.h"
#include "algorithm.h"

	

//延时函数
void delay_ms(unsigned int ms){
	 unsigned int times = 0;	 
	 TMOD =0x01;//设置定时器工作方式为16位定时器
   TH0=(65536-1000)/256;//定时器装载初值
   TL0=(65536-1000)%256; 
   EA=1;//开总中断
   TR0=1;//启动定时器0
	while(1)
	{
		if(TF0==1)//如果定时器溢出
		{
			TH0=(65536-1000)/256; //定时器重新装载初值
			TL0=(65536-1000)%256; 
			times++;
			TF0=0;
			if(times==ms)
			break;
		}	
	}
}

//数码管初始化
void lightInit()
{
	light1=1;//关闭数码管
	light2=1;
	P0 = 0xff;	
}

//设置 T2 自动重载寄存器和计数器初值
void setTime2(unsigned int us)
{
		TH2=(65536-us)/256;
		RCAP2H=(65536-us)/256;
		TL2=(65536-us)%256;
		RCAP2L=(65536-us)%256;
}
		

//定时计数器 T2 初始化
void initTime2()
{
		EA=1;//总中断允许

		//T2CON 默认配置为 16 位自动重载计数模式
		//T2MOD 默认不使能减计数和时钟输出
		ET2=1;
		setTime2(5000);//设置为 5ms 中断一次（可自定义，不可太短）
		TR2=1;
}

//T2 中断服务函数
void time2() interrupt 5
{
		static bit ir=0;//标志本次中断时发射还是接收
		int i=0;
	
		TF2=0;//清除 T2 中断标志位
		
	  //数码管显示坐标
		light1 = ir;
		light2 = !ir;
		P0 = light1 ? table[poY]:table[poX];
	
		if(!ir)//设置左前方发光二极管发射红外光
		{			
			if(group==5)
				group=0;
			MOUSE_IR_ON(group);
			
		}
		else//检测接收管返回的电平
		{
			
			switch(group)
			{
				case 0:irC   = irR1==0?1:0;break;			
				case 1:irLU  = irR2==0?1:0;break;			
				case 2:irL   = irR3==0?1:0;break;	
				case 3:irR   = irR4==0?1:0;break;		
				case 4:irRU  = irR5==0?1:0;break;
			}
			group++;
		}

		P0 = 0xff;
		ir=~ir;//改变接收、发射状态
}


//小车直行
void  straightRun()
{
		unsigned char i=0,j=0,count=105;
		for(j=0;j<count;j++)
		{
			for(i=0;i<8;i++)
			{
				P1 = forward[i];
				delay_ms(2);				
			}	
					
			
				if(irR&&irL)
				{
				/*小车直行途中修正	*/	
				if(irRU==1 && irC==0)//检测到前方没有障碍并且左边靠墙
				{		
						for(i=0;i<8;i++)//小车往右修正
						{
							P1 = (left[i]|0x0f);
							delay_ms(3);
						}
						count+=0.5;
				}
							
				if(irLU==1 && irC==0)//检测到前方没有障碍并且右边靠墙
				{
						for(i=0;i<8;i++)//小车往左修正
						{
							P1 = (right[i]|0xf0);
							delay_ms(3);
						}	
							count+=0.5;
				}
				
			}
		}
	
}

//小车掉头
void turnRun()
{
	unsigned char i=0,j=0;
	for(j=0;j<104;j++)
	{
		for(i=0;i<8;i++)
		{
			P1 = (left[i]);
			delay_ms(2);
		}	
	}	
	dir = (dir+2)%4;//改变绝对方向
}

//小车左转弯
void leftRun()
{
	
	unsigned char i=0,j=0;
	for(j=0;j<52;j++)
	{
		for(i=0;i<8;i++)
		{
			P1 = (left[i]);
			delay_ms(2);
		}	
	}
	dir= (dir+3)%4;//改变绝对方向
}

//小车右转弯
void rightRun()
{
	unsigned char i=0,j=0;
	for(j=0;j<52;j++)
	{
		for(i=0;i<8;i++)
		{
			P1 = (right[i]);
			delay_ms(2);
		}	
	}
	dir= (dir+1)%4;//改变绝对方向
}

//右手法则遍历迷宫
void runMaze()
{
	unsigned char tempR=0,tempL=0,tempC=0;
	unsigned char runR=0,runL=0,runC=0;
	unsigned char flag = 0;

	while(flag==0 || (poX!=0 || poY!=0))
	{	
		//扫描
		tempC = Scan1();
		tempR = Scan3();
		tempL = Scan2();
		//保存墙的信息
		saveWalls(tempL,tempR,tempC);
		runR = !tempR && !isVisit((dir+1)%4);
		runC = !tempC && !isVisit(dir);
		runL = !tempL && !isVisit((dir+3)%4);
		flag =1;
		if((runR+runC+runL)>=2)//如果有两条路可以走
		{		
			if(poX!=roadX[top] || poY!=roadY[top])//记录新岔路口信息
			{
				push(poX,poY);//岔路口坐标入栈
			}
		}
	  /*右手法则遍历迷宫*/
		
		if(runR)  //如果右面没有墙
		{
			rightRun(); 
			straightRun();	
		}
		else if(runC) //如果前面没有墙，并且没有走过
		{				
			straightRun();										
		}
		else if(runL)//如果左面没有墙
		{	
			leftRun();
			straightRun();		
		}
		else//无路可走了就回溯
		{					
			back();					
			continue;
		}
		resetPos();//更改坐标	
		saveOneWall((dir+2)%4,poX,poY);//将两个格子间的墙壁打通
		saveDir(dir,poX,poY);//保存来的方向
		
}
	
}

//更改坐标
void resetPos()
{
	
	switch(dir)
	{
		case 0:poY++;break;//小车向前运动
		case 1:poX++;break;//小车向右运动
		case 2:poY--;break;//小车向左运动
		case 3:poX--;break;//小车向后运动
	}
	
}

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

//回溯（回到岔路口的位置）
void back()
{
	 char carDir = 0;//小车需要走的绝对方向
	 char tempDir = 0;//小车与需要走的方向差的转弯系数
	 unsigned char runR,runL,runC;

	while(1)
	{
		//如果回溯到起点
		if(poX==0 && poY==0)
		{
			turnRun();
			delay_ms(1000);
			break;
		}
		
		//判断是否走到岔路口
		if(poX==roadX[top] && poY==roadY[top])
		{
			 runR = isVisit((dir+1)%4) || Scan3();//判断某个方向是否还能遍历，不能为1
			 runL = isVisit((dir+3)%4) || Scan2();
			 runC = isVisit(dir) || Scan1(); 
			
			//如果岔路口所有能走的方向都已经走过了
			if(runR && runC && runL)
			{
				pop();//移除岔路口信息		
			}
			break;
		}
		
		
		carDir = (getDir(poX,poY)+2)%4;
		tempDir = (dir-carDir)%4;
		switch(tempDir)
		{
			case -1:case 3:rightRun();straightRun();break;
			case 0:straightRun();break;
			case -3:case 1:leftRun();straightRun();break;
			case 2:case -2:turnRun();straightRun();break;			
		}
		
		resetPos();	
					
	}
}


//提取迷宫中保存的绝对方向
unsigned char getDir(unsigned char x,unsigned char y)
{
	unsigned char temp;
	unsigned char result=4;
	temp = map[y][x]&0xf0;
	
	switch(temp)
	{
		case 0xe0:result=0;break;
		case 0xd0:result=1;break;
		case 0xb0:result=2;break;
		case 0x70:result=3;break;
	}
	return result;
}

//判断某绝对方向的路是否走过
unsigned char isVisit(unsigned char dir)
{
		
	switch(dir)
	{
		
		case 0:
			if(poY==SIZE-1)
				return 1;
			return map[poY+1][poX]==0xff?0:1;
		case 1:
			if(poX==SIZE-1)
				return 1;
			return map[poY][poX+1]==0xff?0:1;
		case 2:
			if(poY==0)
				return 1;
			return map[poY-1][poX]==0xff?0:1;
		case 3:
				if(poX==0)
				return 1;
			return map[poY][poX-1]==0xff?0:1;
	}
	return 4;
}




//小车遍历迷宫

//记录没有墙的方向(相对方向)
void saveOneWall(unsigned char dir,unsigned char x,unsigned char y)
{

	switch(dir)
	{
		case 0:map[y][x] = map[y][x] &0xfe;break;//前方				
		case 1:map[y][x] = map[y][x] &0xfd;break;//右方
		case 2:map[y][x] = map[y][x] &0xfb;break;//后方
		case 3:map[y][x] = map[y][x] &0xf7;break;//左方	
	}
	
}

//记录没有墙的方向
void saveWalls(unsigned char left,unsigned char right,unsigned char center)
{
	if(!left)
	saveOneWall((dir+3)%4,poX,poY);
	if(!right)
	saveOneWall((dir+1)%4,poX,poY);
	if(!center)
	saveOneWall(dir,poX,poY);
}


//检测前方有无障碍
unsigned char Scan1()
{
	unsigned char i=0;
	for(;i<30;i++)
	{
		if(irC==1)
			return 1;
		delay_ms(3);		
	}
	return 0;
}

//检测左方有无障碍
unsigned char Scan2()
{
	unsigned char i=0;
	for(;i<30;i++)
	{
		if(irL==1)
			return 1;
		delay_ms(3);		
	}
	return 0;
}

//检测右方有无障碍
unsigned char Scan3()
{
	unsigned char i=0;
	for(;i<30;i++)
	{
		if(irR==1)
			return 1;
		delay_ms(3);		
	}
	return 0;
}

/**********************
           栈
***********************/


//初始化迷宫数据
void initMap(unsigned char mapData[SIZE][SIZE])
{
	unsigned char i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			mapData[i][j] = 0xff;//初始化默认四个方向都有墙		
		}	
	}	
}

//路口坐标入栈
void push(unsigned char x,unsigned char y)
{	
	top++;
	roadX[top] = x;
	roadY[top] = y;
	
}

//路口坐标出栈
void pop()
{
	top--;
}

void beepOn()
{
	unsigned char i =0;
	for(;i<50;i++){
	beep = 0;
	delay_ms(2);
	beep = 1;
	}
}

//冲刺部分算法


//判断某个方向是否为墙
unsigned char isWall(unsigned char  dir,unsigned char x,unsigned char y)
{
	return (map[y][x]>>dir)&0x01;
}


//广度优先算法
void bfs()
{	
	struct queue queueNum[20];//队列
	unsigned char queueLen = 1;//队列长度
	struct queue queueHead;//队列头
	struct queue temp;

	unsigned char i,j;		
	
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
	unsigned char i;
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

//小车按照方向运动
void runWay()
{
		
		char carDir = 0;//小车需要走的绝对方向
		char tempDir = 0;//小车与需要走的方向差的转弯系数
		dir=0;//绝对方向初始化
		poX=0;
		poY=0;

		while(1)
		{
			if(poX==SIZE-1&&poY==SIZE-1)
				return;
		
		carDir = (getDir(poX,poY)+2)%4;
		tempDir = (dir-carDir)%4;
		switch(tempDir)
		{
			case -1:case 3:rightRun();straightRun();break;
			case 0:straightRun();break;
			case -3:case 1:leftRun();straightRun();break;
			case 2:case -2:turnRun();straightRun();break;
		}
		delay_ms(1000);
		resetPos();
	}
		
}

//主函数
void main()
{
	
	lightInit();//初始化数码管;
	initTime2();//初始化 T2
	initMap(map);//初始化迷宫数据
	initMap(height);//初始化等高表
	delay_ms(1000);
	runMaze();
	bfs();
	beepOn();
	findWay(); 
	beepOn();
	runWay();
	beepOn();
	while(1);
}
