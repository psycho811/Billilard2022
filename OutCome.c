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
#include<string.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include <time.h>

#include "imgui.h"

char mode1date[80] = "\0";                 //记录模式1结束时间 
char mode2date[80] = "\0";                 //记录模式2结束时间 
char WinnerName[80] = "Winner Name";       //记录模式1赢家名字 
char LoserName[80] = "Loser Name";         //记录模式1输家名字 
char mode2name[80] ="Name";                //记录模式2玩家名字 
double winwidth=11.0, winheight=7.0;      // 窗口尺寸                          

void Outcome1();                                             //模式1的终止界面
void Outcome2();                                             //模式2的终止界面 
void DrawBackButton();                                       //返回按钮的绘制 
void BackGame(int x,int y,int button,int event);             //回到基础界面的按钮
void OutDisplay();                                           //结束界面刷新 
void drawEditText1();                                        //模式1最终结果的文本编辑 
void drawEditText2();                                        //模式2最终结果的文本编辑 
void Result1Input();                                               //模式1的最终结果
void Score2Input();                                                //模式1的最终结果 
void Gettime();                                                    //获取游戏结束时间 
 
/*使用文本框需要带的*/ 
void OutCharEventProcess(char ch);                           //字符输入 
void OutKeyboardEventProcess(int key, int event);                    
void OutMouseEventProcess(int x, int y, int button, int event); 
       
extern int winner;                   //传进winner
extern int gamemode;                 //传进gamemode 
extern int score;                    //传入分数
extern void Basic();                 //开始界面
 
/*OUTCOME界面字符事件响应函数*/ 
void OutCharEventProcess(char ch){
	uiGetChar(ch);    // GUI字符输入
	OutDisplay();     //刷新显示
}

/*OUTCOME界面键盘事件响应函数*/
void OutKeyboardEventProcess(int key, int event){
	uiGetKeyboard(key,event); // GUI获取键盘
	OutDisplay();             // 刷新显示
}

/*刷新界面*/ 
void OutDisplay(){
	DisplayClear(); 	// 清屏
	if(gamemode==1) {   // 绘制界面
		Outcome1(winner);	
	}   
	else if(gamemode==2) {
		Outcome2(); 
	}   
}

/*双人对战的结果记录*/
void Result1Input(){
	FILE *fp;
	int i=0;
	if((fp=fopen("FinalResult1.txt","a"))==NULL){           //判断能否正常打开FinalResult1.txt 
		exit(0);
	}
	Gettime();                                               //获取游戏结束时间 
	fprintf(fp,"%s\n",mode1date);                            //结束时间 
	fprintf(fp,"%s\n",WinnerName);                           //赢家的名字
	fprintf(fp,"%s\n",LoserName);                            //输家的名字
	if(fclose(fp)){
		exit(0); 
	}  
}

/*获取游戏结束时间*/ 
void Gettime(){
	time_t curtime;
	struct tm *tm_now;
	time(&curtime);
	tm_now = localtime(&curtime);
	if(gamemode==1)
	sprintf(mode1date,"%d/%d/%d %d:%d:%d",1900+tm_now->tm_year,1+tm_now->tm_mon ,tm_now->tm_mday,tm_now->tm_hour, tm_now->tm_min,tm_now->tm_sec);
	if(gamemode==2)
	sprintf(mode2date,"%d/%d/%d %d:%d:%d",1900+tm_now->tm_year,1+tm_now->tm_mon ,tm_now->tm_mday,tm_now->tm_hour, tm_now->tm_min,tm_now->tm_sec);
}

/*单人对战的分数记录*/ 
void Score2Input(){
	FILE *fp;
	if((fp=fopen("FinalScore2.txt","a"))==NULL){           //打开FinalScore2.txt
		exit(0);
	}
	Gettime();
	fprintf(fp,"%s\n",mode2date);   //游戏的时间 
	fprintf(fp,"%s\n",mode2name);    //玩家的名字
	fprintf(fp,"%d\n",score);       //玩家的分数 
	if(fclose(fp)){
		exit(0); 
	}
}

/*模式2最终结果的文本编辑*/ 
void drawEditText2(){
	SetPointSize(20);
	double fH = GetFontHeight();
	double h = fH*2;         // 控件高度
	double w = winwidth/4.3; // 控件宽度
	double x = winwidth*3/6.7;
	double y = winheight/1.7-h;
	SetPenColor("graygreen");
	setTextBoxColors("white", "black", "listyellow", "blue", 1);
	drawLabel(x-fH/1.5-TextStringWidth("WinnName     "), (y-=h*2.7)+fH*0.7, "Type the Name:");
	textbox(GenUIID(0), x+0.3, y, w, h, mode2name, sizeof(mode2name)); 
}

/*模式1最终结果的文本编辑*/ 
void drawEditText1(){
	SetPointSize(20);
	double fH = GetFontHeight();
	double h = fH*2; // 控件高度
	double w = winwidth/4.3; // 控件宽度
	double x = winwidth*3/6.7;
	double y = winheight/1.7-h;
	SetPenColor("yaliyellow");
	setTextBoxColors("white", "black", "listyellow", "blue", 1);
	drawLabel(x-fH/1.5-TextStringWidth("Winner Name"), (y-=h*1.2)+fH*0.7, "Winner Name");
	textbox(GenUIID(0), x, y, w, h, WinnerName, sizeof(WinnerName)); 
	SetPenColor("yaliyellow");
	drawLabel(x-fH/1.5-TextStringWidth("aLoser Name"), (y-=h*1.5)+fH*0.7, "Loser Name");	
	setTextBoxColors("white", "black", "listyellow", "blue", 1);
    textbox(GenUIID(0), x, y, w, h,LoserName, sizeof(LoserName));	
}

/*画返回界面的按钮*/ 
void DrawBackButton(){
	SetPenSize(4);
	SetPointSize(17);
	/*回到开始界面*/ 
	SetPenColor("myorange");
	DrawBox(4.3,1.7,0.9,0.35);
	MovePen(4.42,1.8);
	SetPenColor("graygreen");
	DrawTextString("RETURN");                      
	SetPenColor("yellow");
	DrawEmptyBox(4.3,1.7,0.9,0.35);

	/*退出按钮*/  
	SetPenColor("myorange");
	DrawBox(5.8,1.7,0.9,0.35);
	MovePen(5.92,1.8);
	SetPenColor("graygreen");
	DrawTextString("   EXIT");                                
	SetPenColor("yellow");
	DrawEmptyBox(5.8,1.7,0.9,0.35);
}

/*模式2基础界面*/ 
void Outcome2(){
	char score1[100]="\0";                          //记录成绩的位置 
	registerCharEvent(OutCharEventProcess);         // 字符事件 
	registerKeyboardEvent(OutKeyboardEventProcess); // 键盘事件 
	registerMouseEvent(BackGame);                   // 返回初始界面 
	/*背景*/
	DisplayClear();
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);
	SetStyle(2);
	/*显示成绩*/ 
	MovePen(1.8,4.8);
	SetPenColor("myyellow");
	SetPointSize(90);
	sprintf(score1,"%d",score);
	DrawTextString("Your Final Score is ");
	MovePen(5.3,3.45);
	SetPointSize(120);
	SetPenColor("myred");
	DrawTextString(score1);   
	/*画返回按钮*/ 
	DrawBackButton();	
	/*模式2的文本输入*/ 
	drawEditText2();
}

/*模式1结算基础界面*/
void Outcome1(int winnernum)
{
	registerCharEvent(OutCharEventProcess);        // 字符事件 
	registerKeyboardEvent(OutKeyboardEventProcess);// 键盘事件 
	registerMouseEvent(BackGame);                  // 返回初始界面 
	/*背景*/
	DisplayClear();
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);
	SetStyle(2);
	if(winnernum==1){                             //如果玩家一获胜 
		MovePen(3.0,4.5);
		SetPenColor("myyellow");
		SetPointSize(100);
		DrawTextString("Player1 Win !");        		 
	}
	else if (winnernum==2){                      //如果玩家二获胜 
		MovePen(3.0,4.5);
		SetPenColor("myyellow");
		SetPointSize(100);
		DrawTextString("Player2 Win !"); 
	}
	/*画返回按钮*/ 
	DrawBackButton();
	/*画文本框*/	
	drawEditText1();                            
}

/*回到基础界面的按钮交互*/ 
void BackGame(int x,int y,int button,int event){
	uiGetMouse(x,y,button,event); //GUI获取鼠标
	OutDisplay();                 // 刷新显示	
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	switch(event){
		case MOUSEMOVE:{
			/*移动到返回按钮*/ 
			if(mox>=4.3&&mox<=5.2&&moy>=1.7&&moy<=2.05){
				SetPenSize(4);
				SetPenColor("darkblue");
				DrawEmptyBox(4.3,1.7,0.9,0.35);
			}
			else{
				SetPenColor("yellow");
				SetPenSize(4);
				DrawEmptyBox(4.3,1.7,0.9,0.35);
			}
			/*移动到退出按钮*/ 
			if(mox>=5.8&&mox<=6.7&&moy>=1.7&&moy<=2.05){
				SetPenSize(4);
				SetPenColor("darkblue");
				DrawEmptyBox(5.8,1.7,0.9,0.35);
			}
			else{
				SetPenSize(4);
				SetPenColor("yellow");
				DrawEmptyBox(5.8,1.7,0.9,0.35);
			}
			break;
		}
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.3&&mox<=5.2&&moy>=1.7&&moy<=2.05){
					cancelMouseEvent();
					cancelKeyboardEvent();
					cancelCharEvent();
					if(gamemode==1) Result1Input();   //双人对战结果记录 
					if(gamemode==2) Score2Input();    //单人模式结果记录 
					Basic();
				}
				if(mox>=5.8&&mox<=6.7&&moy>=1.7&&moy<=2.05){
					cancelMouseEvent();
					cancelKeyboardEvent();
					cancelCharEvent();
					if(gamemode==1) Result1Input();   //双人模式结果记录 
					if(gamemode==2) Score2Input();    //单人模式结果记录 
					ExitGraphics(); 
				}
			}
			break;
		}
	}
}






