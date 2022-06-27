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

/*模式2中的数据*/
double distance2[20][20];
int score=0;                //得分 
char a[10]="\0";                 //用来以字符形式记录得分方便显示 
int ContinueShooting=0;     //连击得分记录 
double timegap2=0.006;      //时间间隔 
int ptime2=100;             //力度条变化的时间 
int judgepower2=0;          //判断力度条是否开启 
int ipower2=0;              //记录力度大小 
int measurepower2=0;        //判断是否按下按键，力度条是否停止运动 
int startrun2=0;            //判断小球是否在运动 
int timerecord=0;           //记录游戏已经进行的时间 
int rulereturn2;            //判断是否从规则界面返回 

int lastflag=1;      //判断计时器是否是第一次显示   
double cos1,sin1;   //确定小球运动的方向 
char lasttime[10];   //倒计时 
int lasttimesize;    //倒计时字符串的长度 
ball Ball[16];

extern int gamemode;     //游戏模式 

extern void Table();                //桌面 
void MonitorSwitch2();
extern void InitialBall();         //初始化球的位置
extern void DrawBall();            //画球 
extern void BorderMonitor(int i);  //边界碰撞 
extern void CollisionMonitor(int i,int j);//碰撞监测
extern void Adjustlen(int i,int j);//小球重叠微调 

void UnderDisplay2();         //下面的菜单栏 
void MouseEventProcess2(int x,int y,int button,int event);
void TimerEventProcess2(int timerID);         //判断速度，力度条，球位置的timer 
void PowerDisplay2();                        //力度条颜色改变
void Power_Keyboard2(int key, int event);      //决定力度大小的键盘事件 
void Gamemode2();            //游戏模式2 
void Newed2();               //刷新界面
void InitialTimer();         //初始化倒计时，避免return后再次打开时间出现问题 
void VilocityMonitor2();     //速度检测 
void PotMonitor2(int i);     //进袋监测
void ContinuePot(int i);          //连续击球监测 
void InitialPower2();        //初始化力度条
void WinnerJudge2();             //判断游戏胜负 
void ScoreRecord();         //计算玩家得分 
void SaveTxt2();            //保存文本文件
void SaveBinary2();         //保存二进制文件 

extern int Runningjudge();        //判断是否有小球滚动
extern double DistanceBall();    //计算两球之间的距离
extern void Outcome2();      //结算界面 
extern void DrawBox();
extern void DrawEmptyBox();


void CountDown();                  //倒计时函数 
void Playercontinue();             //连续击球 


/*模式2*/
void Gamemode2()     
{
	int i,j;
	/*分数，时间初始化*/ 
	sprintf(a,"%d",0); 
	score=0;
	ContinueShooting=0;      
	DisplayClear();
	InitialBall();
	InitialPower2();
	InitialTimer();
	
	gamemode=2;               //转换gamemode2模式 
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance2[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //设置球的初始距离 
		    
	registerTimerEvent(TimerEventProcess2);
    registerMouseEvent(MouseEventProcess2);
	startTimer(2,ptime2);
	startTimer(3,1000);
	Newed2();
}

/*初始化时间*/                      //防止在返回后倒计时显示出现问题 
void InitialTimer(){
	timerecord=0;
	lastflag=1;
	sprintf(lasttime,"%c",'\0');
} 

/*力度条初始化*/                     //防止在返回后界面力度条和杆子显示出现问题 
void InitialPower2(){
	judgepower2=0;    
	ipower2=0;        
    measurepower2=0;  
    startrun2=0;     
} 

/*速度监测*/ 
void VilocityMonitor2()
{
	int i,j; 
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);       //边界碰撞监测 
		PotMonitor2(i);         //进袋监测 
		
		sin=Ball[i].vy/(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)));
		cos=Ball[i].vx/(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)));
	     
		Ball[i].x=Ball[i].x+Ball[i].vx;
 		Ball[i].y=Ball[i].y+Ball[i].vy;
  		Ball[i].vx=Ball[i].vx*0.99;  //阻尼运动速度减小 
 		Ball[i].vy=Ball[i].vy*0.99; 
		
		/*判断小球速度*/
		if (Ball[i].state!=2&&(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)))<=0.001){             //小于一定之后为0 
			Ball[i].vx=0;
			Ball[i].vy=0; 
			Ball[i].state=0;
		} 
		 /*判断小球状态*/
		if(Ball[i].state!=2)        
			if(Ball[i].vx==0&&Ball[i].vy==0)
			{
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		MonitorSwitch2();      //碰撞监测 
	}
}

/*速度监测转碰撞监测或微调函数*/
/*这部分原理是，当两球距离小于两个半径时，记录当时两球的距离tempdistance*/
/*当tempdistance小于原来的距离Ball[i].distance[j]时，即两球在靠近，触发碰撞模型改变两球速度*/
/*反之，表示两球碰撞完了正在相互远离，触发微调模型减少穿模现象*/ 
void MonitorSwitch2() 
{
	int i,j; 
	double tempdistance;
	for(i=0;i<=14;i++)
	{
		for(j=0;j<=i-1;j++)
		{
	  	  if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)   //球心距离小于等于两个半径，即两球接触，发生碰撞或碰撞刚好结束
	   	 	{
	    		tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));  //临时计算两球球心距离
				if(tempdistance<distance2[i][j])   //两球距离减小，表示两球在靠近，触发碰撞函数 
				{
					CollisionMonitor(i,j);
					distance2[i][j]=tempdistance;     //记录当前两球距离 
					distance2[j][i]=tempdistance;
				}
				else
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))     //距离增大，两球正在远离，触发微调 
						Adjustlen(i,j);
					  else Adjustlen(j,i);
				}
			}
			else {
					distance2[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));     //未碰撞，直接记录两球距离 
					distance2[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
			}
		for(j=i+1;j<=15;j++)   //同上 
		{
	  		if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
	   		{
	   			tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));
				if(tempdistance<distance2[i][j])
				{
					CollisionMonitor(i,j);
					distance2[i][j]=tempdistance;
					distance2[j][i]=tempdistance;
				}
				else 
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
					    Adjustlen(i,j);
					  else Adjustlen(j,i);
				}
			}
			else {
					distance2[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
					distance2[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
		}
	}
}

/*进袋监测*/
void PotMonitor2(int i)    //当球心距离某个袋口圆心的距离小于等于袋口半径时可判定该球进洞，速度和坐标都重置，状态改为2（进袋） 
{
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		ContinuePot(i);
		Newed2();
	}
}

/*连续击球监测*/     //计算一杆进多球的额外分 
void ContinuePot(int i)      
{
	int j=0;
	if(i!=0)
	{
		j++;                     
		if(j>1) ContinueShooting=ContinueShooting+pow(2,j-2);   //用于计算连续击球 
	}
}

/*判断是否有球在动*/     //返回值：有球运动为1，全部禁止为0 
int Runningjudge2(){
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	if(t==1){ 
		Playercontinue();      //控制继续击球的函数 
	}
	return 0;
}

/*定时器回调函数*/ 
void TimerEventProcess2(int timerID)
{
 	if(timerID==1){  //球的定时器 
 		VilocityMonitor2();   //模拟速度 
 		Newed2();    //刷新每一帧画面 
 		startrun2=Runningjudge2();
 		if(startrun2==0){
 			judgepower2=0;
 			cancelTimer(1);     //关闭计时器1，避免影响力度条显示 
		}
 	}
 	if(timerID==2)  //力度条定时器 
 		PowerDisplay2();
 	if(timerID==3)  //真正的计时器
	{
	 	timerecord++;    //记录时间逐秒增加 
	 	CountDown();     //更新实时倒计时显示
		if(timerecord>=120)    
		{
			cancelTimer(1);
			cancelTimer(2);
			cancelTimer(3);
			ScoreRecord();
			Outcome2();
		} 
	} 
}
/*鼠标回调函数*/
void MouseEventProcess2(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*鼠标在台球桌面*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower2==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //生成引导线和球杆 
						Newed2();  //刷新界面 
		
						if(Ball[0].state==0){      //白球停止生成引导线                     
		   				 	/*生成引导线*/ 
		   					SetPenColor("White");
			    			MovePen(mox,moy);
			    			DrawLine(Ball[0].x - mox,Ball[0].y - moy);
			    			MovePen(mox+0.2,moy);
		     				DrawArc(0.2,0,360);
		
			   			 	/*生成球杆*/
			    			int pen;
			    			double sin,cos;
			    			pen=GetPenSize();
							SetPenSize(pen+4);
							DefineColor("Temp",0.5585,0.1054,0.1054	);
							SetPenColor("Temp");
							sin=(moy-Ball[0].y)/sqrt(pow(mox-Ball[0].x,2)+pow(moy-Ball[0].y,2));
							cos=(mox-Ball[0].x)/sqrt(pow(mox-Ball[0].x,2)+pow(moy-Ball[0].y,2));
							sin1=sin;
							cos1=cos;
							MovePen(Ball[0].x-0.2*cos,Ball[0].y-0.2*sin);
							DrawLine(-cos*3,-sin*3);
							SetPenSize(pen);
            			}
        			}
    			}
				break;
			}
			case BUTTON_UP:{
				if(button==LEFT_BUTTON){
					judgepower2=1;  //判断力度条是否锁定的参数 
				}
				break;
			}
		}
	}
	/*鼠标在标签栏*/ 
	else if(moy>=0&&moy<=1.5){                   
		switch(event){
			case MOUSEMOVE:{
				SetPenColor("gray");
				if(mox>=10.42&&mox<=10.92&&moy>=1.20&&moy<=1.42){     //移动到保存1状态 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,1.20,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,1.20,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //移动到保存2按下状态 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.92,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.92,0.5,0.22);
				}
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
				if(mox>=10.42&&mox<=10.92&&moy>=1.20&&moy<=1.42){     //移动到保存1状态 
					cancelMouseEvent();
					cancelTimer(3);
					cancelTimer(2);
					cancelTimer(1);  //关闭计时器，避免影响界面显示 
					SaveTxt2();      //保存数据 
					DisplayClear();  //清屏
					GameSelect();    //跳回游戏模式选择 
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //移动到保存2按下状态 
					cancelMouseEvent();
					cancelTimer(3);
					cancelTimer(2);
					cancelTimer(1);  //关闭计时器，避免影响界面显示 
					SaveBinary2();   //保存数据 
					DisplayClear();  //清屏
					GameSelect();    //跳回游戏模式选择
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //移动到RETURN按下状态 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1); 
					cancelTimer(3);  //关闭计时器，避免影响界面显示 
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //移动到HELP按下状态 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1); 
					cancelTimer(3);
					DisplayClear();
					rulereturn2=1;
					SaveTxt2();                    //储存临时小球位置 
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

/*刷新台球面*/ 
void Newed2()   
{
	DisplayClear();
	Table();
	UnderDisplay2(); 
	DrawBall();
	
	MovePen(5,0.6);   //画分数 
	SetPointSize(30);
	DrawTextString(a);
}

/*下部分数菜单栏*/ 
void UnderDisplay2(){
	/*底面边框*/ 
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
	MovePen(0.3,1.2);
	SetStyle(1);
	DrawTextString("POWER SELECT");
	
	/*按键初始化*/
	int t=0;
	SetPenSize(3);
	DefineColor("listyellow",.9648,.9023,.0546);
	SetPenColor("listyellow");
	for(t=0;t<=4;t++){
		DrawBox(10.42,.08+t*0.28,0.5,0.22);
	}  
	DefineColor("yaliyellow",.9804,.7812,.1835);
	SetPenColor("yaliyellow");
	for(t=0;t<=4;t++){
		DrawEmptyBox(10.42,.08+t*0.28,0.5,0.22); 
	}
	SetPenColor("graygreen");
	SetPointSize(14);
	SetStyle(0);
	MovePen(10.44,1.225);
	DrawTextString("保存txt");          //保存文本文件 
	MovePen(10.44,.925);
	DrawTextString("保存dat");          //保存二进制文件 
	MovePen(10.44,.665);
	DrawTextString(" Return");         //返回主界面
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //退出
	MovePen(10.44,.385);
	DrawTextString(" Help");           //帮助 
	
	SetPenColor("Black");
	SetPointSize(30);
	SetPenSize(5);
	MovePen(3.5,0.6);
	DrawTextString("得分："); 
	MovePen(7.46,0.6);
	DrawTextString("时间："); 
	MovePen(9,0.6);
	if(lastflag==1) DrawTextString("120");   //防止初始化120无法显示 
	DrawTextString(lasttime);                //防止球运动的时候倒计时条不动 
}
 
 /*记录分数，规则：进球分加连续击球分*/
void ScoreRecord()
{
    int j,i;
	i=0; 
	for(j=1;j<=15;j++)
	{
		if(Ball[j].state==2) i++;
	}
	score=i+ContinueShooting;
	sprintf(a,"%d",score);
} 
 
 /*倒计时器函数,总时间为120s*/ 
 void CountDown()
 {
 	int time=120-timerecord;
 	if(time<0) time=0;
 	lastflag=0;
 	sprintf(lasttime,"%d",time);
 	/*更新倒计时画面*/ 
 	SetPenColor("white");               
 	DrawBox(9.0,0.3,0.5,0.6);
 	SetPenColor("black");
 	MovePen(9.0,0.6);
 	SetPointSize(30);
 	DrawTextString(lasttime);
 }
 
  /*力度条变化*/ 
 void PowerDisplay2()
 {
	if(judgepower2==1&&measurepower2==0){
		registerKeyboardEvent(Power_Keyboard2);
		if(ipower2<=6){
			SetPenColor("myyellow");
			DrawBox(.3+ipower2*0.3,.425,.13,0.65);
		}
		else if(ipower2>=7 && ipower2<=13){
			SetPenColor("gray");
			DrawBox(4.2-ipower2*0.3,.425,.13,0.65);
		}
		if(ipower2==13) ipower2=-1;
		ipower2++;
	}
 }

/*力度条的键盘事件*/ 
void Power_Keyboard2(int key, int event){
 	switch(key)
	{
		/*按下空格，确定力度*/
		case VK_SPACE:{
			if(judgepower2==1)
				measurepower2=1;  //标志球已经确定力度，防止力度条再次滚动 
			/*赋予白球速度*/ 
			if(judgepower2==1&&measurepower2==1)    
			{
				double power=0;//打击力的大小 
				if(ipower2>=0 && ipower2<=6){
					power=3+ipower2;
				}
				else if(ipower2>=7 && ipower2<=13){
					power=16-ipower2;
				}
				startTimer(1,timegap2);       //开启球滚动的timer，赋予白球速度 
				Ball[0].vx=0.015*cos1*power;
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun2=1;                  //小球开始处于运动 
				cancelKeyboardEvent();
			}
			break;
		}
	}
 }

/*实现连续击球*/ 
void Playercontinue()
{
	int j=0,i=0;
 	measurepower2=0;            //力度条初始化 
	ipower2=0;                  //力度条变化初始化 
	if(Ball[0].state==2 ){
		Ball[0].state =0;
		Ball[0].x=2.6;
		Ball[0].y=4.25;
		for(i=0;i<=15;i++)   //进行微调，防止白球重新生成时与其他球重叠 
			for(j=0;j<=15;j++)
			{
	  	  		if(i!=j&&sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
				if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
					Adjustlen(i,j);
			    else Adjustlen(j,i);
			}
	} 
	ScoreRecord();            //计算当前得分 
	Newed2();                 //更新界面
}

/*以文本模式保存*/           
void SaveTxt2()
{                               
	FILE *fp1;
	int i=0;
	if(rulereturn2==0){
		if((fp1=fopen("SaveGameSingle1.txt","w"))==NULL)     //判断能否正常打开SaveGameSingle1.txt
			exit(0);
	}	
	else if(rulereturn2==1){
		if((fp1=fopen("Game2Temp.txt","w"))==NULL)     //判断能否正常打开SaveGameSingle1.txt
			exit(0);
	}
	/*转存小球数据*/
	for(i=0;i<=15;i++){
		fprintf(fp1,"%d\n",Ball[i].state );       //储存小球状态 
		fprintf(fp1,"%lf\n",Ball[i].vx);          //储存小球速度
		fprintf(fp1,"%lf\n",Ball[i].vy);
		fprintf(fp1,"%lf\n",Ball[i].x);           //储存小球位置 
		fprintf(fp1,"%lf\n",Ball[i].y);
	}
	fprintf(fp1,"%d\n",score);
	/*转存初始化数据*/
	fprintf(fp1,"%d\n",judgepower2);  //关于力度条数据 
	fprintf(fp1,"%d\n",ipower2); 
	fprintf(fp1,"%d\n",measurepower2); 
	fprintf(fp1,"%d\n",startrun2);    //运动数据 
	fprintf(fp1,"%lf\n",sin1);       //储存杆的方向 
	fprintf(fp1,"%lf\n",cos1);  
	/*转存时间数据*/
	fprintf(fp1,"%d\n",timerecord);  
	fprintf(fp1,"%s\n",lasttime);
	fprintf(fp1,"%d\n",lastflag); 
	if(fclose(fp1)){
		exit(0); 
	}                                                                                
}
/*以二进制模式保存*/ 
void SaveBinary2(){
	FILE *fp2;
	int i=0;
	if((fp2=fopen("SaveGameSingle2.dat","wb"))==NULL){           //判断能否正常打开SaveGameSingle2.dat
		exit(0);
	}
	/*转存小球数据*/
	for(i=0;i<=15;i++){
		fwrite(&Ball[i].state,sizeof(int),1,fp2);               
		fwrite(&Ball[i].vx,sizeof(double),1,fp2);
		fwrite(&Ball[i].vy,sizeof(double),1,fp2);
		fwrite(&Ball[i].x,sizeof(double),1,fp2);
		fwrite(&Ball[i].y,sizeof(double),1,fp2);
	}
	fwrite(&score,sizeof(int),1,fp2);
	/*转存初始化数据*/
	fwrite(&judgepower2,sizeof(int),1,fp2);                  //力度条相关数据 
	fwrite(&ipower2,sizeof(int),1,fp2);
	fwrite(&measurepower2,sizeof(int),1,fp2);
	fwrite(&startrun2,sizeof(int),1,fp2);                    //小球是否在运动 
	fwrite(&sin1,sizeof(double),1,fp2);                      //杆子的方向 
	fwrite(&cos1,sizeof(double),1,fp2);
	/*转存时间数据*/
	lasttimesize=strlen(lasttime);
	fwrite(&timerecord,sizeof(int),1,fp2);
	fwrite(&lasttime,sizeof(lasttime),lasttimesize,fp2);
	fwrite(&lastflag,sizeof(int),1,fp2);
	if(fclose(fp2)){
		exit(0); 
	}  
}

 
 

