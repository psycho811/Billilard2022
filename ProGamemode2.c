#include <stdio.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>

#include "imgui.h"

/*��Ĳ���*/ 
typedef struct   
{
	double x;   //������ 
	double y;   //������ 
	double vx;  //������ٶ� 
	double vy;  //������ٶ� 
	int state;  //���״̬��0��ʾ��ֹ��1��ʾ�˶���2��ʾ������ 
	
	/*��¼��ɫ����*/ 
	double R; 
	double G; 
	double B;
	
	
}ball;


ball Ball[16];

extern void Table();                //���� 
extern void InitialBall();          //��ʼ�����λ��
void UnderDisplayPro();             //����Ĳ˵��� 
void MouseEventProcessPro(int x,int y,int button,int event);
extern void TimerEventProcess(int timerID);        //�ж��ٶȣ�����������λ�õ�timer 
extern void PowerDisplay();                        //��������ɫ�ı�
extern void Power_Keyboard(int key, int event);    //�������ȴ�С�ļ����¼� 
extern void Newed2();              //ˢ�½��� 
extern void DrawBall();            //���� 
extern void VilocityMonitor();     //�ٶȼ�� 
extern void BorderMonitor(int i);  //�߽���ײ 
extern void CollisionMonitor(int i,int j);//��ײ���
extern void PotMonitor(int i);     //������� 
extern void Adjustlen(int i,int j);//С���ص�΢�� 
extern void Playerchange();        //������� 
extern void InitialPower();        //��ʼ��������
extern void BallNumberRecord();    //����¼��������� 

extern void WinnerJudge();             //�ж���Ϸʤ�� 
extern void Outcome(int winernum);     //������� 
extern int Runningjudge();        //�ж��Ƿ���С�����
extern double DistanceBall();    //��������֮��ľ��� 
extern void DrawBox();           //ʵ�ķ��� 
extern void DrawEmptyBox();      //���ķ��� 

extern int gamemode;


/*ģʽ2׼���׶�*/
void ProGamemode2()     
{
	DisplayClear();
	Table();
	SetPenSize(3);
	gamemode=25;          //ת��Ϊģʽ2��ʼ���� 
	/*��ʼ��ť*/
	SetPenColor("yaliyellow");              
	DrawBox(4.75,4.0,1.5,0.6);
	SetPenColor("lightorange");
	DrawEmptyBox(4.75,4.0,1.5,0.6);
	MovePen(5.17,4.2);
	SetStyle(2);  
	SetPointSize(30);   
	SetPenColor("black");
	DrawTextString("��ʼ");
	UnderDisplayPro(); 
	
	registerMouseEvent(MouseEventProcessPro);
}

/*���ص�����*/
void MouseEventProcessPro(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	
	switch(event){
		/*�ƶ�����ť״̬*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=4.0&&moy<=4.6){            //��ʼ��ť 
					SetPenColor("black");
					DrawEmptyBox(4.75,4.0,1.5,0.6);
				}
				else{
					SetPenColor("lightorange");
					DrawEmptyBox(4.75,4.0,1.5,0.6);
				}
			break;
		}
		/*���°�ť����*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=4.0&&moy<=4.6){          //���¿�ʼ��ť 
					cancelMouseEvent;
					Gamemode2();
				}
			}
			break;
		}
		
	}	
	/*����ڱ�ǩ��*/ 
	if(moy>=0&&moy<=1.5){                   
		switch(event){
			case MOUSEMOVE:{
				SetPenColor("gray");
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //�ƶ������ذ���״̬ 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.64,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.64,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //�ƶ����˳�����״̬ 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.36,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.36,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.08&&moy<=.3){   //��������״̬
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.08,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.08,0.5,0.22);
				}
				break;
			}
			case BUTTON_UP:{
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //�ƶ���RETURN����״̬ 
					cancelMouseEvent();
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //�ƶ���HELP����״̬ 
					cancelMouseEvent();
					DisplayClear();
					Rule();	
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.08&&moy<=.3){   //����EXIT״̬
					cancelMouseEvent();
					ExitGraphics();
				}
				break;
			}
		}
	}		
}

/*�²������˵���*/ 
void UnderDisplayPro(){
	/*����߿�*/ 
	DefineColor("lightorange",.95,.63,.0351);
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
	DrawLine(0.0,-1.45);
	MovePen(6.46,1.45);
	DrawLine(0.0,-1.45);
	
	/*��������ʼ��*/
	double i=0;
	SetPenColor("gray");
	for(i=0;i<=6;i++){                       //7�����ȵ��� 
		DrawBox(.3+i*0.3,.425,.13,0.65);
	}
	MovePen(1.6,0.9);
	DefineColor("graygreen",.5390,.4101,.507);
	SetPenColor("graygreen");
	SetPointSize(14);
	SetStyle(1);
	MovePen(0.3,1.2);
	DrawTextString("POWER SELECT");
	
	/*������ʼ��*/
	int t=0;
	SetStyle(0);
	SetPointSize(14);
	SetPenSize(3);
	DefineColor("listyellow",.9648,.9023,.0546);
	SetPenColor("listyellow");
	for(t=0;t<=2;t++){
		DrawBox(10.42,.08+t*0.28,0.5,0.22);
	}  
	DefineColor("yaliyellow",.9804,.7812,.1835);
	SetPenColor("yaliyellow");
	for(t=0;t<=2;t++){
		DrawEmptyBox(10.42,.08+t*0.28,0.5,0.22); 
	}
	SetPenColor("graygreen");
	SetPointSize(14);
	MovePen(10.44,.665);
	DrawTextString(" Return");         //����������
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //�˳�
	MovePen(10.44,.385);
	DrawTextString(" Help");           //���� 
 }
