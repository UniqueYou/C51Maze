//�������ʾ
unsigned  char code table[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit light1=P4^2;//DS2�����
sbit light2=P4^3;//DS1�����
sbit beep = P3^7;//������
unsigned char code forward[] = {0x11,0x93,0x82,0xc6,0x44,0x6c,0x28,0x39};//ǰ��
unsigned char code left[] = {0x11,0x99,0x88,0xcc,0x44,0x66,0x22,0x33};//��ת
unsigned char code right[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};//��ת
//�Թ�
#define SIZE 8//�Թ���С
unsigned char xdata map[SIZE][SIZE];
unsigned char poX=0,poY=0;//�Թ�X���꣬Y����
unsigned char dir =0;//���Է���
//ջ
unsigned char xdata roadX[20];
unsigned char xdata roadY[20];
unsigned char top = 0;

//������
void initTime2();
void delay_ms(unsigned int ms);
void lightInit();
void setTime2(unsigned int us);

//С���˶�
void straightRun();
void backRun();
void turnRun();
void leftRun();
void rightRun();
//�����Թ�
void initMap(unsigned char mapData[SIZE][SIZE]);
unsigned char scanC();
unsigned char scanL();
unsigned char scanR();
unsigned char isVisit(unsigned char dir);
void back();
//�����Թ���Ϣ
void saveOneWall(unsigned char dir,unsigned char x,unsigned char y);
void saveWalls(unsigned char left,unsigned char right,unsigned char center);
void saveDir(unsigned char i,unsigned char x,unsigned char y);
unsigned char getDir(unsigned char x,unsigned char y);
void resetPos();
void beepOn();

//ջ
void push(unsigned char x,unsigned char y);
void pop();

