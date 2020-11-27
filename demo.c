#define uChar unsigned char 
#define  uInt unsigned int


#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include <stdlib.h>
#include "mouse.h"
#include "demo.h"
#include "algorithm.h"

//��ʱ����
void delay_ms(uInt ms){
	 uInt times = 0;	 
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
void setTime2(uInt us)
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
			
		if(closeScan)//����ر���ɨ��Ͳ�ɨ��
		return;
			
	
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
		uChar i=0,j=0;
		for(j=0;j<105;j++)
		{
			for(i=0;i<8;i++)
			{
				P1 = forward[i];
				delay_ms(2);				
			}	
			
			
			/*С��ֱ��;������	*/	
			if(irRU==1 && irC==0)//��⵽ǰ��û���ϰ�������߿�ǽ
			{		
					for(i=0;i<8;i++)//С����������
					{
						P1 = (left[i] & 0xf0);
						delay_ms(3);
					}								
			}
			
			if(irLU==1 && irC==0)//��⵽ǰ��û���ϰ������ұ߿�ǽ
			{
					for(i=0;i<8;i++)//С����������
					{
						P1 = (right[i] & 0x0f);
						delay_ms(3);
					}	
			}
			

		}
	
}



//С����ͷ
void turnRun()
{
	uChar i=0,j=0;
	for(j=0;j<100;j++)
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
	
	uChar i=0,j=0;
	for(j=0;j<50;j++)
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
	uChar i=0,j=0;
	for(j=0;j<50;j++)
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
	uChar tempR=0,tempL=0,tempC=0;
	uChar runR=0,runL=0,runC=0;
	uChar flag = 0;

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
			push(poX,poY);//��·��������ջ
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
			saveDir();//�������ķ���			
			back();					
			continue;
		}
		resetPos();//��������	
		saveOneWall((dir+2)%4);//���������Ӽ��ǽ�ڴ�ͨ
		saveDir();//�������ķ���
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
void saveDir()
{
	
	switch(dir)
	{
		case 0:map[poY][poX] =  map[poY][poX]&0xef;break;
		case 1:map[poY][poX] =  map[poY][poX]&0xdf;break;
		case 2:map[poY][poX] =  map[poY][poX]&0xbf;break;
		case 3:map[poY][poX] =  map[poY][poX]&0x7f;break;
	}

}

//���ݣ��ص���·�ڵ�λ�ã�
void back()
{
	 char carDir = 0;//С����Ҫ�ߵľ��Է���
	 char tempDir = 0;//С������Ҫ�ߵķ�����ת��ϵ��
	uChar i = 0;
	uChar runR = isVisit((dir+1)%4) || isWall((dir+1)%4);//�ж�ĳ�������Ƿ��ܱ���������Ϊ1
	uChar runL = isVisit((dir+3)%4) || isWall((dir+3)%4);
	uChar runC = isVisit(dir) || isWall(dir);
	while(1)
	{
		//������ݵ����
		if(poX==0 && poY==0)
		{
			turnRun();
			beepOn();
			delay_ms(100);
			beepOn();
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
		
		//�ж��Ƿ��ߵ���·��
		if(poX==roadX[top] && poY==roadY[top])
		{
			beepOn();
			//�����·���������ߵķ����Ѿ��߹���
			if(runR && runC && runL)
				pop();//�Ƴ���·����Ϣ
			break;
		}
		
		
	}
}


//��ȡ�Թ��б���ľ��Է���
uChar getDir(uChar poX,uChar poY)
{
	uChar temp;
	uChar result=0;
	temp = map[poY][poX]&0xf0;
	
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
uChar isVisit(uChar dir)
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


//������
void main()
{
		lightInit();//��ʼ�������;
		initTime2();//��ʼ�� T2	
		mazeDateInit();//��ʼ���Թ�����	
		delay_ms(1000);
		runMaze();
		while(1);

}

//С�������Թ�

//��¼û��ǽ�ķ���(��Է���)
void saveOneWall(uChar dir)
{

	switch(dir)
	{
		case 0:map[poY][poX] &= 0xfe;break;//ǰ��
		case 1:map[poY][poX] &= 0xfd;break;//�ҷ�
		case 2:map[poY][poX] &= 0xfb;break;//��
		case 3:map[poY][poX] &= 0xf7;break;//��	
	}
	
}

//��¼���������Ƿ���ǽ
void saveWalls(uChar left,uChar right,unsigned center)
{
	if(left)
	saveOneWall((dir+3)%4);
	if(right)
	saveOneWall((dir+1)%4);
	if(center)
	saveOneWall(dir);
	
}




//���ǰ�������ϰ�
uChar Scan1()
{
	uChar i=0;
	for(;i<30;i++)
	{
		if(irC==1)
			return 1;
		delay_ms(3);		
	}
	return 0;
}

//����������ϰ�
uChar Scan2()
{
	uChar i=0;
	for(;i<30;i++)
	{
		if(irL==1)
			return 1;
		delay_ms(3);		
	}
	return 0;
}

//����ҷ������ϰ�
uChar Scan3()
{
	uChar i=0;
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
void mazeDateInit()
{
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<SIZE;j++)
		{
			map[i][j] = 0xff;//��ʼ��Ĭ���ĸ�������ǽ		
		}	
	}	
}

//·��������ջ
void push(uChar x,uChar y)
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
	uChar i =0;
	for(;i<50;i++){
	beep = 0;
	delay_ms(5);
	beep = 1;
	delay_ms(5);
	}
}


//����㷨����

//�ж�ĳ�������Ƿ�Ϊǽ
uChar isWall(uChar dir)
{
	switch(dir)
	{
		
		case 0:
			return (map[poY][poX] & 0x01)==0x00?1:0;
		case 1:
			return (map[poY][poX] & 0x02)==0x00?1:0;
		case 2:
			return (map[poY][poX] & 0x04)==0x00?1:0;
		case 3:
			return (map[poY][poX] & 0x08)==0x00?1:0;
	}
	return 4;

}