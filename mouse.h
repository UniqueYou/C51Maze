sfr P4=0xe8;
//�����ַ�ӿڶ���
sbit A0=P4^0;
sbit A1=P2^0;
sbit A2=P2^7;
//���⴫���������źſڶ��壨���յ��ź�ֵΪ 0��
sbit irR1=P2^1; //ǰ
sbit irR2=P2^2; //����
sbit irR3=P2^3; //��
sbit irR4=P2^4; //��
sbit irR5=P2^5; //����
uChar closeScan = 0;
uChar group = 0;//���⴫�������
bit irC=0 ,irL=0,irR=0,irLU=0,irRU=0;//������⴫�������״̬ȫ��λ������Ϊ 0 ���ϰ�
uChar code maze[8][8];//�Թ����ݱ���
//���ⷢ����ƺ궨��(���봫�������)
#define MOUSE_IR_ON(GROUP_NO) \
	do\ 
	{ \
		A0=(GROUP_NO)&0x01; \
		A1=(GROUP_NO)&0x02; \
		A2=(GROUP_NO)&0x04; \
	}while(0)
