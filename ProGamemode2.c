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

/*球的参数*/ 
typedef struct   
{
	double x;   //横坐标 
	double y;   //纵坐标 
	double vx;  //横向初速度 
	double vy;  //纵向初速度 
	int state;  //球的状态（0表示静止，1表示运动，2表示进袋） 
	
	/*记录颜色参数*/ 
	double R; 
	double G; 
	double B;
	
	
}ball;


ball Ball[16];

extern void Table();                //桌面 
extern void InitialBall();          //初始化球的位置
void UnderDisplayPro();             //下面的菜单栏 
void MouseEventProcessPro(int x,int y,int button,int event);
extern void TimerEventProcess(int timerID);        //判断速度，力度条，球位置的timer 
extern void PowerDisplay();                        //力度条颜色改变
extern void Power_Keyboard(int key, int event);    //决定力度大小的键盘事件 
extern void Newed2();              //刷新界面 
extern void DrawBall();            //画球 
extern void VilocityMonitor();     //速度检测 
extern void BorderMonitor(int i);  //边界碰撞 
extern void CollisionMonitor(int i,int j);//碰撞监测
extern void PotMonitor(int i);     //进袋监测 
extern void Adjustlen(int i,int j);//小球重叠微调 
extern void Playerchange();        //交换玩家 
extern void InitialPower();        //初始化力度条
extern void BallNumberRecord();    //画记录进球的数字 

extern void WinnerJudge();             //判断游戏胜负 
extern void Outcome(int winernum);     //结算界面 
extern int Runningjudge();        //判断是否有小球滚动
extern double DistanceBall();    //计算两球之间的距离 
extern void DrawBox();           //实心方块 
extern void DrawEmptyBox();      //空心方块 

extern int gamemode;


/*模式2准备阶段*/
void ProGamemode2()     
{
	DisplayClear();
	Table();
	SetPenSize(3);
	gamemode=25;          //转换为模式2开始界面 
	/*开始按钮*/
	SetPenColor("yaliyellow");              
	DrawBox(4.75,4.0,1.5,0.6);
	SetPenColor("lightorange");
	DrawEmptyBox(4.75,4.0,1.5,0.6);
	MovePen(5.17,4.2);
	SetStyle(2);  
	SetPointSize(30);   
	SetPenColor("black");
	DrawTextString("开始");
	UnderDisplayPro(); 
	
	registerMouseEvent(MouseEventProcessPro);
}

/*鼠标回调函数*/
void MouseEventProcessPro(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	
	switch(event){
		/*移动到按钮状态*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=4.0&&moy<=4.6){            //开始按钮 
					SetPenColor("black");
					DrawEmptyBox(4.75,4.0,1.5,0.6);
				}
				else{
					SetPenColor("lightorange");
					DrawEmptyBox(4.75,4.0,1.5,0.6);
				}
			break;
		}
		/*按下按钮交互*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=4.0&&moy<=4.6){          //按下开始按钮 
					cancelMouseEvent;
					Gamemode2();
				}
			}
			break;
		}
		
	}	
	/*鼠标在标签栏*/ 
	if(moy>=0&&moy<=1.5){                   
		switch(event){
			case MOUSEMOVE:{
				SetPenColor("gray");
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //移动到返回按下状态 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.64,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.64,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //移动到退出按下状态 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.36,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.36,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.08&&moy<=.3){   //帮助按下状态
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
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //移动到RETURN按下状态 
					cancelMouseEvent();
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //移动到HELP按下状态 
					cancelMouseEvent();
					DisplayClear();
					Rule();	
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.08&&moy<=.3){   //帮助EXIT状态
					cancelMouseEvent();
					ExitGraphics();
				}
				break;
			}
		}
	}		
}

/*下部分数菜单栏*/ 
void UnderDisplayPro(){
	/*底面边框*/ 
	DefineColor("lightorange",.95,.63,.0351);
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
	DrawLine(0.0,-1.45);
	MovePen(6.46,1.45);
	DrawLine(0.0,-1.45);
	
	/*力度条初始化*/
	double i=0;
	SetPenColor("gray");
	for(i=0;i<=6;i++){                       //7个力度档次 
		DrawBox(.3+i*0.3,.425,.13,0.65);
	}
	MovePen(1.6,0.9);
	DefineColor("graygreen",.5390,.4101,.507);
	SetPenColor("graygreen");
	SetPointSize(14);
	SetStyle(1);
	MovePen(0.3,1.2);
	DrawTextString("POWER SELECT");
	
	/*按键初始化*/
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
	DrawTextString(" Return");         //返回主界面
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //退出
	MovePen(10.44,.385);
	DrawTextString(" Help");           //帮助 
 }
