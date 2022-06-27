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

int gamemode;         //记录游戏模式 
void Basic();         //画开始界面 
void DrawBox();       //画实心方块
void DrawEmptyBox();  //画空心方块       
void Basic_MouseEvent (int x, int y, int button, int event);        //主界面的基础鼠标事件 

/*初始化*/ 
void Main(){
	SetWindowSize(11,7); //设置窗口大小 
	SetWindowTitle("Billiard----Made By Two People");
	InitGraphics();
	Basic();             //画开始界面 
	PlaySound("bgm/Hedwig's Theme.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP);//播放游戏音乐	
}
/*主界面的交互*/ 
void Basic_MouseEvent (int x, int y, int button, int event)
{
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*鼠标移动到按钮*/
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){           //移动到开始按钮 
					SetPenColor("Light Gray");
					DrawBox(4.75 , 3.5, 1.5, 0.6);
					SetPenColor("Black");
					MovePen(5.2,3.7);
					SetPointSize(28);
					DrawTextString("开始");
				}
				else{
					SetPenColor("mywhite");               
					DrawBox(4.75 , 3.5, 1.5, 0.6);
					SetPenColor("Black");
					MovePen(5.2,3.7);
					SetPointSize(28);
					DrawTextString("开始");
				}
				if((mox-5.5)*(mox-5.5)+(moy-2)*(moy-2)<=0.64){            //移动到规则按钮 
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
					DrawTextString("规则");	
				}
				if((mox-2.2)*(mox-2.2)+(moy-3)*(moy-3)<=0.64){       //移动到排行榜按钮 
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
					DrawTextString("排行榜");
				}
				if((mox-8.8)*(mox-8.8)+(moy-3)*(moy-3)<=0.64){     //移动到退出按钮 
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
					DrawTextString("    退出");		
				}	
			break;
		}
		/*按下*/ 
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){          //游戏模式选择 
					cancelMouseEvent();
					GameSelect();
				}
				if((mox-5.5)*(mox-5.5)+(moy-2)*(moy-2)<=0.64){          //规则界面 
					cancelMouseEvent();
					Rule();
				}
				if((mox-2.2)*(mox-2.2)+(moy-3)*(moy-3)<=0.64){          //排行榜 
					cancelMouseEvent();
					RankBasic(); 
				}
				if((mox-8.8)*(mox-8.8)+(moy-3)*(moy-3)<=0.64){          //退出 
					cancelMouseEvent();
					ExitGraphics();                                           
				}
			}
			break;
		}	
	}	
}

/*画游戏主界面*/ 
void Basic(){
	DisplayClear();//更新界面 
	gamemode=0;
	int m,n;
	registerMouseEvent(Basic_MouseEvent);
	SetPenSize(3);
	DefineColor("myblue1",.5742,.7070,.8086);//程序所有颜色的定义 
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

	/*背景*/ 
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);	        
	/*标题*/
	SetPenColor("myyellow"); 
	SetFont("Times");           
	MovePen(4.2,5.0);
	SetPointSize(50);
	SetStyle(2);
	DrawTextString("Billiard2022");
	printf("%d %d",m,n);
	MovePen(4.0,4.8);
	DrawLine(3.2,0); 
	/*排行榜*/
	SetStyle(0);
	SetPenColor("myorange");              
	MovePen(3.0,3.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(1.7,3.0);
	SetPointSize(30);
	DrawTextString("排行榜");
	/*开始按钮*/
	SetPenColor("mywhite");              
	DrawBox(4.75 , 3.5, 1.5, 0.6);
	SetPenColor("Black");
	MovePen(5.2,3.7);
	SetPointSize(28);
	DrawTextString("开始");                     
	/*规则按钮*/ 
	SetPenColor("myred");                
	MovePen(6.3,2.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(5.15,2.0);
	SetPointSize(30);
	DrawTextString("规则");	
	/*退出按钮*/
	SetPenColor("myorange");             
	MovePen(9.6,3.0);
	StartFilledRegion(1);
	DrawArc(.8,0,360);
	EndFilledRegion();
	SetPenColor("Black");
	MovePen(8.1,3.0);
	SetPointSize(30);
	DrawTextString("    退出");	
}

/*画实心矩形*/ 
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

/*画空心矩形*/ 
void DrawEmptyBox(double x, double y, double width, double height)
{
	MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
}


