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
