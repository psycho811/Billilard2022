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
extern ball Ball[15];
extern gamemode;         //记录游戏模式 
extern double u;         //摩擦产生的加速度（减速） 
extern double timegap;   //时间间隔 
extern int ptime;        //力度条变化的时间 
extern int judgepower;   //判断力度条是否开启 
extern int ipower;       //记录力度大小 
extern int measurepower; //判断是否按下按键，力度条是否停止运动 
extern int startrun;     //判断小球是否在运动 
extern int player;       //预置玩家为1     
extern double cos1,sin1;   //确定小球运动的方向 
extern int judgepower2;   //判断模式2力度条是否开启
extern int ipower2;       //记录模式2力度大小
extern int measurepower2; //判断模式2是否按下按键，力度条是否停止运动
extern int timerecord;    //记录时间
extern char lasttime;     //倒计时的字符串形式 
extern int lastflag;      //判断计时器是否是第一次显示  
extern int score;         //玩家分数 
extern int startrun2;     //判断模式2小球是否在运动 
extern int judgepower3;   //判断模式3力度条是否开启
extern int ipower3;       //记录模式3力度大小
extern int measurepower3; //判断模式3是否按下按键，力度条是否停止运动
extern int startrun3;     //判断模式3小球是否在运动
extern int rulereturn1;   //判断模式1是从哪个界面返回游戏 
extern int rulereturn2;   //判断模式2是从哪个界面返回游戏 
extern int lasttimesize;  //记录模式2传入时间长度 

void ContinueLastBasic();    //继续上次的基础界面 
void ContinueTxt1();    //继续双人模式txt 
void ContinueDat1();    //继续双人模式dat
void ContinueTxt2();    //继续单人模式txt
void ContinueDat2();    //继续单人模式dat
void ContinueTxt3();    //继续练习模式 
void Continue_MouseEvent (int x, int y, int button, int event);        //继续上次界面鼠标相应函数 

extern Gamemode1(),Newed();
extern Gamemode2(),Newed2();
extern Gamemode3(),Newed3();

/*画基本界面*/ 
void ContinueLastBasic(){
	DisplayClear();
	registerMouseEvent(Continue_MouseEvent); 
	SetPenSize(3); 
	SetPointSize(24);     
	SetPenColor("myblue");                
	DrawBox(0.0,0.0,11.0,7.0);
	
	
	/*继续单人限时模式txt*/
	SetPenColor("darkblue");
	DrawBox(4.75,3.3,2.3,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,3.3,2.3,0.6);
	MovePen(4.8,3.5);
	SetPenColor("white");
	SetFont("Times");
	SetStyle(2);
	DrawTextString("继续单人限时txt");
	
	SetPenColor("darkblue");
	DrawBox(4.75,2.3,2.3,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,2.3,2.3,0.6);
	MovePen(4.8,2.5);
	SetPenColor("white");
	DrawTextString("继续单人限时dat");
	
	/*继续双人对战txt*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,5.3,2.3,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,5.3,2.3,0.6);
	MovePen(4.8,5.5);
	SetPenColor("white");
	DrawTextString("继续双人对战txt");
	/*继续双人对战dat*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,4.3,2.3,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,4.3,2.3,0.6);
	MovePen(4.8,4.5);
	SetPenColor("white");
	DrawTextString("继续双人对战dat"); 
	/*返回模式选择*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,1.3,2.3,0.6); 
	SetPenColor("yellow");
	DrawEmptyBox(4.75,1.3,2.3,0.6);
	MovePen(4.8,1.5);
	SetPenColor("white");
	DrawTextString("       Return"); 	
}

/*继续上次界面的鼠标事件*/
void Continue_MouseEvent(int x, int y, int button, int event){
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*移动到按钮状态*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=7.05&&moy>=5.3&&moy<=5.9){           //继续双人对战txt 
					SetPenColor("black");
					DrawEmptyBox(4.75,5.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,5.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=4.3&&moy<=4.9){           //继续双人对战dat
					SetPenColor("black");
					DrawEmptyBox(4.75,4.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,4.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=3.3&&moy<=3.9){            //继续单人限时txt
					SetPenColor("black");
					DrawEmptyBox(4.75,3.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,3.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=2.3&&moy<=2.9){            //继续单人限时dat
					SetPenColor("black");
					DrawEmptyBox(4.75,2.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,2.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=1.3&&moy<=1.9){            //return 按钮 
					SetPenColor("black");
					DrawEmptyBox(4.75,1.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,1.3,2.3,0.6);
				}
			break;
		}
		/*按下按钮交互*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=7.05&&moy>=5.3&&moy<=5.9){          //双人对战txt
					cancelMouseEvent();
					DisplayClear();
					ContinueTxt1();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=4.3&&moy<=4.9){          //双人对战dat
					cancelMouseEvent();
					DisplayClear();
					ContinueDat1();
				}
				
				if(mox>=4.75 &&mox<=7.05&&moy>=3.3&&moy<=3.9){          //单人限时txt 
					cancelMouseEvent();
					DisplayClear();
					ContinueTxt2();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=2.3&&moy<=2.9){          //单人限时dat 
					cancelMouseEvent();
					DisplayClear();
					ContinueDat2();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=1.3&&moy<=1.9){          //返回上一层 
					cancelMouseEvent();
					DisplayClear();
					GameSelect();
				}
			}
			break;
		}	
	}
}

/*读取双人模式文本文件*/ 
void ContinueTxt1(){
	FILE *fp1;
	int i=0;                                                     //i作为循环变量，t作为标记
	if(rulereturn1==1){                                          //判断是否是从help界面返回 
		if((fp1=fopen("Game1Temp.txt","r"))==NULL)               //判断是否能正常打开文件 
			exit(0);
	}
	else if(rulereturn1==0){
		if((fp1=fopen("SaveGameDouble1.txt","r"))==NULL)         //判断是否能正常打开文件     
			exit(0);
	} 	
	Gamemode1();
	/*转存小球数据*/ 
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );     //小球状态
		fscanf(fp1,"%lf",&Ball[i].vx);        //小球速度
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);         //小球位置 
		fscanf(fp1,"%lf",&Ball[i].y);  
	}
	/*转读初始化数据*/
	fscanf(fp1,"%d",&player);
	fscanf(fp1,"%d",&judgepower);             //力度条数据
	fscanf(fp1,"%d",&ipower);
	fscanf(fp1,"%d",&measurepower);
	fscanf(fp1,"%d",&startrun);               //运动数据 
	fscanf(fp1,"%lf",&cos1);                  //储存杆的方向 
	fscanf(fp1,"%lf",&cos1);
	/*防止出现小球有速度，但不滚动的状况*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	if(fclose(fp1)){
		exit(0); 
	}
	Newed();  //更新原来数据 
} 

/*读取双人对战二进制文件*/ 
void ContinueDat1()
{
	Gamemode1();
	int i=0,t=0;
	FILE *fp2;
	if((fp2=fopen("SaveGameDouble2.dat","rb"))==NULL){           //判断能否正常打开"SaveGame2.dat"
		exit(0);
	}
	for(i=0;i<=15;i++){
		fread(&Ball[i].state,sizeof(int),1,fp2);
		fread(&Ball[i].vx,sizeof(double),1,fp2);
		fread(&Ball[i].vy,sizeof(double),1,fp2);
		fread(&Ball[i].x,sizeof(double),1,fp2);
		fread(&Ball[i].y,sizeof(double),1,fp2);
	}
	fread(&player,sizeof(int),1,fp2);                      //玩家数据 
	fread(&judgepower,sizeof(int),1,fp2);                  //力度条相关数据 
	fread(&ipower,sizeof(int),1,fp2);
	fread(&measurepower,sizeof(int),1,fp2);
	fread(&startrun,sizeof(int),1,fp2);
	fread(&sin1,sizeof(double),1,fp2);
	fread(&cos1,sizeof(double),1,fp2);
	/*关闭文件*/
	if(fclose(fp2)){
		exit(0);
	}
	/*防止出现小球有速度，但不滚动的状况*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	Newed();  //更新模式2数据 
}

/*读取单人对战txt文件*/ 
void ContinueTxt2()
{                          
	FILE *fp1;
	int i=0;
	if(rulereturn2==1){                                             //判断是否是从help界面返回                                         
		if((fp1=fopen("Game2Temp.txt","r"))==NULL)                 //判断能否正常打开
			exit(0);
	}
	else if(rulereturn2==0){
		if((fp1=fopen("SaveGameSingle1.txt","r"))==NULL)           //判断能否正常打开
			exit(0);
	}
	Gamemode2();
	/*转读小球数据*/
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );       //储存小球状态 
		fscanf(fp1,"%lf",&Ball[i].vx);          //储存小球速度
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);           //储存小球位置 
		fscanf(fp1,"%lf",&Ball[i].y);
	}
	fscanf(fp1,"%d",&score);
	/*转存初始化数据*/
	fscanf(fp1,"%d",&judgepower2);  //关于力度条数据 
	fscanf(fp1,"%d",&ipower2); 
	fscanf(fp1,"%d",&measurepower2); 
	fscanf(fp1,"%d",&startrun2);    //运动数据 
	fscanf(fp1,"%lf",&sin1);       //储存杆的方向 
	fscanf(fp1,"%lf",&cos1);  
	/*转存时间数据*/
	fscanf(fp1,"%d",&timerecord);  
	fscanf(fp1,"%s",&lasttime);
	fscanf(fp1,"%d",&lastflag); 
	if(fclose(fp1))
		exit(0); 
	/*防止出现小球有速度，但不滚动的状况*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	Newed2();  //更新模式2数据                                                                               
}

/*读取单人对战dat文件*/
void ContinueDat2(){
	FILE *fp2;
	int i=0;
	if((fp2=fopen("SaveGameSingle2.dat","rb"))==NULL){           //判断能否正常打开SaveGameSingle2.dat
		exit(0);
	}
	Gamemode2();
	/*转存小球数据*/
	for(i=0;i<=15;i++){
		fread(&Ball[i].state,sizeof(int),1,fp2);               
		fread(&Ball[i].vx,sizeof(double),1,fp2);
		fread(&Ball[i].vy,sizeof(double),1,fp2);
		fread(&Ball[i].x,sizeof(double),1,fp2);
		fread(&Ball[i].y,sizeof(double),1,fp2);
	}
	fread(&score,sizeof(int),1,fp2);
	/*转存初始化数据*/
	fread(&judgepower2,sizeof(int),1,fp2);                  //力度条相关数据 
	fread(&ipower2,sizeof(int),1,fp2);
	fread(&measurepower2,sizeof(int),1,fp2);
	fread(&startrun2,sizeof(int),1,fp2);                    //小球是否在运动 
	fread(&sin1,sizeof(double),1,fp2);                     //杆子的方向 
	fread(&cos1,sizeof(double),1,fp2);
	/*转存时间数据*/
	fread(&timerecord,sizeof(int),1,fp2);
	fread(&lasttime,sizeof(lasttime),lasttimesize,fp2);
	fread(&lastflag,sizeof(int),1,fp2);
	/*防止出现小球有速度，但不滚动的状况*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	if(fclose(fp2)){
		exit(0); 
	} 
	Newed2(); //更新模式2数据 
}

/*继续模式3中的*/
void ContinueTxt3(){
	Gamemode3();                            
	FILE *fp1;
	int i=0;
	if((fp1=fopen("Game3Temp.txt","r"))==NULL)
		exit(0);
	/*转读小球数据*/
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );       //储存小球状态 
		fscanf(fp1,"%lf",&Ball[i].vx);          //储存小球速度
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);           //储存小球位置 
		fscanf(fp1,"%lf",&Ball[i].y);
	}
	/*转存初始化数据*/
	fscanf(fp1,"%d",&judgepower3);  //关于力度条数据 
	fscanf(fp1,"%d",&ipower3); 
	fscanf(fp1,"%d",&measurepower3); 
	fscanf(fp1,"%d",&startrun3);    //运动数据 
	fscanf(fp1,"%lf",&sin1);       //储存杆的方向 
	fscanf(fp1,"%lf",&cos1);  
	if(fclose(fp1)){
		exit(0); 
	}
	/*防止出现小球有速度，但不滚动的状况*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	} 
	Newed3(); //更新模式3数据                 
} 



