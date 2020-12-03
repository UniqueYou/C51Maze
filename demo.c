
#include <stdio.h>
#include <reg52.h>
#include <string.h>
#include <stdlib.h>
#include "mouse.h"
#include "demo.h"
#include "algorithm.h"

	

//��ʱ����
void delay_ms(unsigned int ms){
	 unsigned int times = 0;	 
	 TMOD =0x01;//���ö�ʱ��������ʽΪ16λ��ʱ��
   TH0=(65536-1000)/256;//��ʱ��װ�س�ֵ
   TL0=(65536-1000)%256; 
   EA=1;//�����ж�
   TR0=1;//������ʱ��0
	while(1)
	{
		if(TF0==1)//�����ʱ�����
		{
			TH0=(65536-1000)/256; //��ʱ������װ�س�ֵ
			TL0=(65536-1000)%256; 
			times++;
			TF0=0;
			if(times==ms)
			break;
		}	
	}
}

//����ܳ�ʼ��
void lightInit()
{
	light1=1;//�ر������
	light2=1;
	P0 = 0xff;	
}

//���� T2 �Զ����ؼĴ����ͼ�������ֵ
void setTime2(unsigned int us)
{
		TH2=(65536-us)/256;
		RCAP2H=(65536-us)/256;
		TL2=(65536-us)%256;
		RCAP2L=(65536-us)%256;
}
		

//��ʱ������ T2 ��ʼ��
void initTime2()
{
		EA=1;//���ж�����

		//T2CON Ĭ������Ϊ 16 λ�Զ����ؼ���ģʽ
		//T2MOD Ĭ�ϲ�ʹ�ܼ�������ʱ�����
		ET2=1;
		setTime2(5000);//����Ϊ 5ms �ж�һ�Σ����Զ��壬����̫�̣�
		TR2=1;
}

//T2 �жϷ�����
void time2() interrupt 5
{
		static bit ir=0;//��־�����ж�ʱ���仹�ǽ���
		int i=0;
	
		TF2=0;//��� T2 �жϱ�־λ
		
	  //�������ʾ����
		light1 = ir;
		light2 = !ir;
		P0 = light1 ? table[poY]:table[poX];
	
		if(!ir)//������ǰ����������ܷ�������
		{			
			if(group==5)
				group=0;
			MOUSE_IR_ON(group);
			
		}
		else//�����չܷ��صĵ�ƽ
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
		ir=~ir;//�ı���ա�����״̬
}


//С��ֱ��
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
				/*С��ֱ��;������	*/	
				if(irRU==1 && irC==0)//��⵽ǰ��û���ϰ�������߿�ǽ
				{		
						for(i=0;i<8;i++)//С����������
						{
							P1 = (left[i]|0x0f);
							delay_ms(3);
						}
						count+=0.5;
				}
							
				if(irLU==1 && irC==0)//��⵽ǰ��û���ϰ������ұ߿�ǽ
				{
						for(i=0;i<8;i++)//С����������
						{
							P1 = (right[i]|0xf0);
							delay_ms(3);
						}	
							count+=0.5;
				}
				
			}
		}
	
}

//С����ͷ
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
	dir = (dir+2)%4;//�ı���Է���
}

//С����ת��
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
	dir= (dir+3)%4;//�ı���Է���
}

//С����ת��
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
	dir= (dir+1)%4;//�ı���Է���
}

//���ַ�������Թ�
void runMaze()
{
	unsigned char tempR=0,tempL=0,tempC=0;
	unsigned char runR=0,runL=0,runC=0;
	unsigned char flag = 0;

	while(flag==0 || (poX!=0 || poY!=0))
	{	
		//ɨ��
		tempC = Scan1();
		tempR = Scan3();
		tempL = Scan2();
		//����ǽ����Ϣ
		saveWalls(tempL,tempR,tempC);
		runR = !tempR && !isVisit((dir+1)%4);
		runC = !tempC && !isVisit(dir);
		runL = !tempL && !isVisit((dir+3)%4);
		flag =1;
		if((runR+runC+runL)>=2)//���������·������
		{		
			if(poX!=roadX[top] || poY!=roadY[top])//��¼�²�·����Ϣ
			{
				push(poX,poY);//��·��������ջ
			}
		}
	  /*���ַ�������Թ�*/
		
		if(runR)  //�������û��ǽ
		{
			rightRun(); 
			straightRun();	
		}
		else if(runC) //���ǰ��û��ǽ������û���߹�
		{				
			straightRun();										
		}
		else if(runL)//�������û��ǽ
		{	
			leftRun();
			straightRun();		
		}
		else//��·�����˾ͻ���
		{					
			back();					
			continue;
		}
		resetPos();//��������	
		saveOneWall((dir+2)%4,poX,poY);//���������Ӽ��ǽ�ڴ�ͨ
		saveDir(dir,poX,poY);//�������ķ���
		
}
	
}

//��������
void resetPos()
{
	
	switch(dir)
	{
		case 0:poY++;break;//С����ǰ�˶�
		case 1:poX++;break;//С�������˶�
		case 2:poY--;break;//С�������˶�
		case 3:poX--;break;//С������˶�
	}
	
}

//������Է����Թ�������
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

//���ݣ��ص���·�ڵ�λ�ã�
void back()
{
	 char carDir = 0;//С����Ҫ�ߵľ��Է���
	 char tempDir = 0;//С������Ҫ�ߵķ�����ת��ϵ��
	 unsigned char runR,runL,runC;

	while(1)
	{
		//������ݵ����
		if(poX==0 && poY==0)
		{
			turnRun();
			delay_ms(1000);
			break;
		}
		
		//�ж��Ƿ��ߵ���·��
		if(poX==roadX[top] && poY==roadY[top])
		{
			 runR = isVisit((dir+1)%4) || Scan3();//�ж�ĳ�������Ƿ��ܱ���������Ϊ1
			 runL = isVisit((dir+3)%4) || Scan2();
			 runC = isVisit(dir) || Scan1(); 
			
			//�����·���������ߵķ����Ѿ��߹���
			if(runR && runC && runL)
			{
				pop();//�Ƴ���·����Ϣ		
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


//��ȡ�Թ��б���ľ��Է���
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

//�ж�ĳ���Է����·�Ƿ��߹�
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




//С�������Թ�

//��¼û��ǽ�ķ���(��Է���)
void saveOneWall(unsigned char dir,unsigned char x,unsigned char y)
{

	switch(dir)
	{
		case 0:map[y][x] = map[y][x] &0xfe;break;//ǰ��				
		case 1:map[y][x] = map[y][x] &0xfd;break;//�ҷ�
		case 2:map[y][x] = map[y][x] &0xfb;break;//��
		case 3:map[y][x] = map[y][x] &0xf7;break;//��	
	}
	
}

//��¼û��ǽ�ķ���
void saveWalls(unsigned char left,unsigned char right,unsigned char center)
{
	if(!left)
	saveOneWall((dir+3)%4,poX,poY);
	if(!right)
	saveOneWall((dir+1)%4,poX,poY);
	if(!center)
	saveOneWall(dir,poX,poY);
}


//���ǰ�������ϰ�
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

//����������ϰ�
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

//����ҷ������ϰ�
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
           ջ
***********************/


//��ʼ���Թ�����
void initMap(unsigned char mapData[SIZE][SIZE])
{
	unsigned char i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			mapData[i][j] = 0xff;//��ʼ��Ĭ���ĸ�������ǽ		
		}	
	}	
}

//·��������ջ
void push(unsigned char x,unsigned char y)
{	
	top++;
	roadX[top] = x;
	roadY[top] = y;
	
}

//·�������ջ
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

//��̲����㷨


//�ж�ĳ�������Ƿ�Ϊǽ
unsigned char isWall(unsigned char  dir,unsigned char x,unsigned char y)
{
	return (map[y][x]>>dir)&0x01;
}


//��������㷨
void bfs()
{	
	struct queue queueNum[20];//����
	unsigned char queueLen = 1;//���г���
	struct queue queueHead;//����ͷ
	struct queue temp;

	unsigned char i,j;		
	
	height[0][0] = 0;

	queueNum[0].x = 0;//��ʼ���������
	queueNum[0].y = 0;
	
	while(queueLen>0)//�����в�Ϊ�յ�ʱ��
	{	
		
		queueHead = queueNum[0];//����Ԫ�س���
		queueLen--;
		//������Ԫ��ǰ��һ����λ����Ϊ�õ�������
		for(i=0;i<queueLen;i++)
			queueNum[i] = queueNum[i+1];		
		//�ж��ĸ������Ƿ��ܹ����
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
			
			if(!isWall(j,queueHead.x,queueHead.y) && height[temp.y][temp.x]==0xff)	//����������ҵ�һ����
			{
				height[temp.y][temp.x] = height[queueHead.y][queueHead.x]+1;
				queueNum[queueLen] = temp;//�������
				queueLen++;
			}
		}
	}	
}


//������ң�Ѱ�����·��
void findWay()
{
	struct queue nowPos,temp;
	unsigned char i;
	nowPos.y=SIZE-1;
	nowPos.x=SIZE-1;
	
	
	
	while(!(nowPos.x==0&&nowPos.y==0))//�����û�е����
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

			
			//��������߲��ҵȸ߱�ݼ�
			if(!isWall(i,nowPos.x,nowPos.y) && (height[temp.y][temp.x]==height[nowPos.y][nowPos.x]-1))
			{
				map[temp.y][temp.x] |= 0xf0;//��ʼ���Թ�����λ
				saveDir(i,temp.x,temp.y);
				nowPos = temp;
				break;
			}
		} 
	}
}

//С�����շ����˶�
void runWay()
{
		
		char carDir = 0;//С����Ҫ�ߵľ��Է���
		char tempDir = 0;//С������Ҫ�ߵķ�����ת��ϵ��
		dir=0;//���Է����ʼ��
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

//������
void main()
{
	
	lightInit();//��ʼ�������;
	initTime2();//��ʼ�� T2
	initMap(map);//��ʼ���Թ�����
	initMap(height);//��ʼ���ȸ߱�
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
