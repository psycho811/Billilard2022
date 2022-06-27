#include <stdio.h>
#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>

#include "imgui.h"

int gamemode;         //��¼��Ϸģʽ 
void Basic();         //����ʼ���� 
void DrawBox();       //��ʵ�ķ���
void DrawEmptyBox();  //�����ķ���       
void Basic_MouseEvent (int x, int y, int button, int event);        //������Ļ�������¼� 

/*��ʼ��*/ 
void Main(){
	SetWindowSize(11,7); //���ô��ڴ�С 
	SetWindowTitle("Billiard----Made By Two People");
	InitGraphics();
	Basic();             //����ʼ���� 
	PlaySound("bgm/Hedwig's Theme.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);//������Ϸ����	
}
/*������Ľ���*/ 
void Basic_MouseEvent (int x, int y, int button, int event)
{
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*����ƶ�����ť*/
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){           //�ƶ�����ʼ��ť 
					SetPenColor("Light Gray");
					DrawBox(4.75 , 3.5, 1.5, 0.6);
					SetPenColor("Black");
					MovePen(5.2,3.7);
					SetPointSize(28);
					DrawTextString("��ʼ");
				}
				else{
					SetPenColor("mywhite");               
					DrawBox(4.75 , 3.5, 1.5, 0.6);
					SetPenColor("Black");
					MovePen(5.2,3.7);
					SetPointSize(28);
					DrawTextString("��ʼ");
				}
				if((mox-5.5)*(mox-5.5)+(moy-2)*(moy-2)<=0.64){            //�ƶ�������ť 
					SetPenColor("Red");
					MovePen(6.3,2.0);
					DrawArc(.8,0,360);
					SetPenColor("Black");
				}

				else{
					SetPenColor("myred");               
					MovePen(6.3,2.0);
					StartFilledRegion(1);
					DrawArc(.8,0,360);
					EndFilledRegion();
					SetPenColor("Black");
					MovePen(5.15,2.0);
					SetPointSize(30);
					DrawTextString("����");	
				}
				if((mox-2.2)*(mox-2.2)+(moy-3)*(moy-3)<=0.64){       //�ƶ������а�ť 
					SetPenColor("Orange");                               
					MovePen(3.0,3.0);
					DrawArc(.8,0,360);
				}
				else{
					SetPenColor("myorange");               
					MovePen(3.0,3.0);
					StartFilledRegion(1);
					DrawArc(.8,0,360);
					EndFilledRegion();
					SetPenColor("Black");
					MovePen(1.7,3.0);
					SetPointSize(30);
					DrawTextString("���а�");
				}
				if((mox-8.8)*(mox-8.8)+(moy-3)*(moy-3)<=0.64){     //�ƶ����˳���ť 
					SetPenColor("Orange");                             
					MovePen(9.6,3.0);
					DrawArc(.8,0,360);
				}
				else{
					SetPenColor("myorange");             
					MovePen(9.6,3.0);
					StartFilledRegion(1);
					DrawArc(.8,0,360);
					EndFilledRegion();
					SetPenColor("Black");
					MovePen(8.1,3.0);
					SetPointSize(30);
					DrawTextString("    �˳�");		
				}	
			break;
		}
		/*����*/ 
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){          //��Ϸģʽѡ�� 
					cancelMouseEvent();
					GameSelect();
				}
				if((mox-5.5)*(mox-5.5)+(moy-2)*(moy-2)<=0.64){          //������� 
					cancelMouseEvent();
					Rule();
				}
				if((mox-2.2)*(mox-2.2)+(moy-3)*(moy-3)<=0.64){          //���а� 
					cancelMouseEvent();
					RankBasic(); 
				}
				if((mox-8.8)*(mox-8.8)+(moy-3)*(moy-3)<=0.64){          //�˳� 
					cancelMouseEvent();
					ExitGraphics();                                           
				}
			}
			break;
		}	
	}	
}

/*����Ϸ������*/ 
void Basic(){
	DisplayClear();//���½��� 
	gamemode=0;
	int m,n;
	registerMouseEvent(Basic_MouseEvent);
	SetPenSize(3);
	DefineColor("myblue1",.5742,.7070,.8086);//����������ɫ�Ķ��� 
	DefineColor("blue222",.3789,.6015,.7617);
	DefineColor("blue333",.1289,.4648,.7188);
	DefineColor("myblue",.3,.6,.8); 
	DefineColor("myyellow",.96875,.960,.45) ; 
	DefineColor("mywhite",.9,.9,.9);
	DefineColor("myorange",.94,.703,.25);
	DefineColor("myred",.94,.47,.25);
	DefineColor("mygreen",.589,.851,.488);
	DefineColor("darkblue",.3125,.324,.984);
	DefineColor("yaliyellow",.9804,.7812,.1835);
	DefineColor("listyellow",.9648,.9023,.0546);
	DefineColor("graygreen",.5390,.4101,.507);
	DefineColor("lightorange",.95,.63,.0351);
	DefineColor("fenshu",.1015,.406,.25);

	/*����*/ 
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);	        
	/*����*/
	SetPenColor("myyellow"); 
	SetFont("Times");           
	MovePen(4.2,5.0);
	SetPointSize(50);
	SetStyle(2);
	DrawTextString("Billiard2022");
	printf("%d %d",m,n);
	MovePen(4.0,4.8);
	DrawLine(3.2,0); 
	/*���а�*/
	SetStyle(0);
	SetPenColor("myorange");              
	MovePen(3.0,3.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(1.7,3.0);
	SetPointSize(30);
	DrawTextString("���а�");
	/*��ʼ��ť*/
	SetPenColor("mywhite");              
	DrawBox(4.75 , 3.5, 1.5, 0.6);
	SetPenColor("Black");
	MovePen(5.2,3.7);
	SetPointSize(28);
	DrawTextString("��ʼ");                     
	/*����ť*/ 
	SetPenColor("myred");                
	MovePen(6.3,2.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(5.15,2.0);
	SetPointSize(30);
	DrawTextString("����");	
	/*�˳���ť*/
	SetPenColor("myorange");             
	MovePen(9.6,3.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(8.1,3.0);
	SetPointSize(30);
	DrawTextString("    �˳�");	
}

/*��ʵ�ľ���*/ 
void DrawBox(double x, double y, double width, double height)       
{
	StartFilledRegion(1);
    MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
    EndFilledRegion();
}

/*�����ľ���*/ 
void DrawEmptyBox(double x, double y, double width, double height)
{
	MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
}


