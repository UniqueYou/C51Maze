//�������ʾ
unsigned  char code table[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit light1=P4^2;//DS2�����
sbit light2=P4^3;//DS1�����
sbit beep = P3^7;//������
uChar code forward[] = {0x11,0x93,0x82,0xc6,0x44,0x6c,0x28,0x39};//ǰ��
uChar code left[] = {0x99,0x88,0xcc,0x44,0x66,0x22,0x33,0x11};//��ת
uChar code right[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};//��ת
//�Թ�
#define SIZE 8 //�Թ���С

uChar  xdata map[SIZE][SIZE]; 
uChar poX=0,poY=0;//�Թ�X���꣬Y����
uChar dir =0;//���Է���
//ջ
uChar xdata roadX[20];
uChar xdata roadY[20];
uChar top = 0;

//������
void initTime2();
void delay_ms(uInt ms);
void lightInit();
void setTime2(uInt us);

//С���˶�
void straightRun();
void backRun();
void turnRun();
void leftRun();
void rightRun();
//�����Թ�
void mazeDateInit();
uChar Scan1();
uChar Scan2();
uChar Scan3();
uChar isVisit(uChar dir);
uChar isWall(uChar dir);
void back();
//�����Թ���Ϣ
void saveOneWall(uChar dir);
void saveWalls(uChar left,uChar right,unsigned center);
void saveDir();
uChar getDir(uChar poX,uChar poY);
void resetPos();
void beepOn();



//ջ
void push(uChar x,uChar y);
void pop();