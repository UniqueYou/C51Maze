//数码管显示
unsigned  char code table[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
sbit light1=P4^2;//DS2数码管
sbit light2=P4^3;//DS1数码管
sbit beep = P3^7;//蜂鸣器
uChar code forward[] = {0x11,0x93,0x82,0xc6,0x44,0x6c,0x28,0x39};//前进
uChar code left[] = {0x99,0x88,0xcc,0x44,0x66,0x22,0x33,0x11};//左转
uChar code right[] = {0x11,0x33,0x22,0x66,0x44,0xcc,0x88,0x99};//右转
//迷宫
#define SIZE 8 //迷宫大小

uChar  xdata map[SIZE][SIZE]; 
uChar poX=0,poY=0;//迷宫X坐标，Y坐标
uChar dir =0;//绝对方向
//栈
uChar xdata roadX[20];
uChar xdata roadY[20];
uChar top = 0;

//红外检测
void initTime2();
void delay_ms(uInt ms);
void lightInit();
void setTime2(uInt us);

//小车运动
void straightRun();
void backRun();
void turnRun();
void leftRun();
void rightRun();
//遍历迷宫
void mazeDateInit();
uChar Scan1();
uChar Scan2();
uChar Scan3();
uChar isVisit(uChar dir);
uChar isWall(uChar dir);
void back();
//保存迷宫信息
void saveOneWall(uChar dir);
void saveWalls(uChar left,uChar right,unsigned center);
void saveDir();
uChar getDir(uChar poX,uChar poY);
void resetPos();
void beepOn();



//栈
void push(uChar x,uChar y);
void pop();