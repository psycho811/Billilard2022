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

 
/*模式1中的数据*/ 
double distance[20][20];   //球之间的距离
double timegap=0.006;   //时间间隔 
int ptime=100;      //力度条变化的时间 
int judgepower=0;   //判断力度条是否开启 
int ipower=0;       //记录力度大小 
int measurepower=0; //判断是否按下按键，力度条是否停止运动 
int startrun=0;     //判断小球是否在运动 
int player=1;       //预置玩家为1 
int winner=1;       //预置胜者为1 
double cos1,sin1;   //确定小球运动的方向 
ball Ball[16];      //定义球
int rulereturn1;     //判断是否从规则界面返回  

extern int gamemode;     //记录模式gamemode1 

void MonitorSwitch();        //速度监测转碰撞监测或微调函数
void Table();                //桌面 
void UnderDisplay();         //下面的菜单栏 
void MouseEventProcess(int x,int y,int button,int event);
void TimerEventProcess(int timerID);         //判断速度，力度条，球位置的timer 
void PowerDisplay();                        //力度条颜色改变
void Power_Keyboard(int key, int event);      //决定力度大小的键盘事件 
void Gamemode1();           //双人对战模式 
void Newed();               //刷新界面 
void DrawBall();            //画球 
void InitialBall();         //初始化球的位置 
void VilocityMonitor();     //速度检测 
void BorderMonitor(int i);  //边界碰撞 
void CollisionMonitor(int i,int j);//碰撞监测
void PotMonitor(int i);     //进袋监测 
void Adjustlen(int i,int j);//小球重叠微调 
void Playerchange();        //交换玩家 
void InitialPower();        //初始化力度条
void BallNumberRecord();    //画记录进球的数字 
void WinnerJudge();             //判断游戏胜负 
void Outcome(int winernum);      //结算界面 
void SaveBinary1();          //存储二进制文件
void SaveTxt1();             //保存文本文件 

int Runningjudge();        //判断是否有小球滚动
double DistanceBall();    //计算两球之间的距离 

extern void DrawBox();
extern void DrawEmptyBox();
extern void Basic();
extern void Outcome1();


/*模式1*/
void Gamemode1()     
{
	InitialBall();   //初始化球参数 
	InitialPower();  //初始化力度条 
	int i,j; 
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //设置球的初始距离 
	registerTimerEvent(TimerEventProcess);
    registerMouseEvent(MouseEventProcess);
    gamemode=1;      //记录模式为模式1（存档时需要用） 
	startTimer(2,ptime);
	Newed();
}

/*力度条初始化*/                     //防止在保存返回后界面力度条和杆子显示出现问题 
void InitialPower(){
	judgepower=0;    
	ipower=0;        
    measurepower=0;  
    startrun=0;     
} 

/*碰撞监测*/
void CollisionMonitor(int i,int j)
{
	double sy,sx,tx,ty;  //s表示法向（球心连线方向），t表示切向 
	
	sx=Ball[i].x-Ball[j].x;   // 计算法向切向向量 
	sy=Ball[i].y-Ball[j].y;
	tx=-sy;
	ty=sx;

	double e;           
	e=sqrt(sx*sx+sy*sy); //求出法向的单位向量
	sx=sx/e;      
	sy=sy/e;
	e=sqrt(tx*tx+ty*ty);  //求出切向的单位向量 
	tx=tx/e;
	ty=ty/e;

	double tvi,tvj,svi,svj;              //计算两球的切向和法向分速度 
	svi=Ball[i].vx*sx+Ball[i].vy*sy;
	svj=Ball[j].vx*sx+Ball[j].vy*sy;
	tvi=Ball[i].vx*tx+Ball[i].vy*ty;
	tvj=Ball[j].vx*tx+Ball[j].vy*ty;

	double temp;        //计算碰撞后的速度，原理：法向相当于正碰，交换动量，即交换速度；切向速度不改变 
	temp=svi;
	svi=svj;
	svj=temp;

	Ball[i].vx=svi*sx+tvi*tx;      //求出碰撞结束后的最终速度 
	Ball[i].vy=svi*sy+tvi*ty;
	Ball[j].vx=svj*sx+tvj*tx;
	Ball[j].vy=svj*sy+tvj*ty;
}

/*速度监测转碰撞监测或微调函数*/
/*这部分原理是，当两球距离小于两个半径时，记录当时两球的距离tempdistance*/
/*当tempdistance小于原来的距离Ball[i].distance[j]时，即两球在靠近，触发碰撞模型改变两球速度*/
/*反之，表示两球碰撞完了正在相互远离，触发微调模型减少穿模现象*/ 
void MonitorSwitch() 
{
	int i,j; 
	double tempdistance;   //临时记录两球距离 
	for(i=0;i<=14;i++)
	{
		for(j=0;j<=i-1;j++)
		{
	  	  if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)   //球心距离小于等于两个半径，即两球接触，发生碰撞或碰撞刚好结束 
	   	 	{
				tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));   //临时计算两球球心距离 
				if(tempdistance<distance[i][j]&&(Ball[i].state==1||Ball[j].state==1))      //两球在靠近，触发碰撞模型改变两球速度
				{
					CollisionMonitor(i,j);  //碰撞 
					distance[i][j]=tempdistance;   //将临时求出的两球距离存入距离参数distance中 
					distance[j][i]=tempdistance;
				}
				else                      //两球碰撞完了正在相互远离，触发微调模型减少穿模现象
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))  //微调函数中只对Ball[i]进行微调，选择速度偏小的那一个球作为Ball[i] 
						Adjustlen(i,j);  //微调 
					  else Adjustlen(j,i);
				}
			}
			else {
					distance[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //球心距离大于两个半径，不发生碰撞，直接将临时求出的两球距离存入距离参数distance中
					distance[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
		}
		for(j=i+1;j<=15;j++)        //同上，两部分合起来就是i球与所有其他球（除其本身）检测是否碰撞 
		{
	  		if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
	   		{
	   			tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));
				if(tempdistance<distance[i][j]&&(Ball[i].state==1||Ball[j].state==1))
				{
					CollisionMonitor(i,j);
					distance[i][j]=tempdistance;
					distance[j][i]=tempdistance;
				}
				else 
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
					    Adjustlen(i,j);
					  else Adjustlen(j,i);
				}
			}
			else {
					distance[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
					distance[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
		}
	}
}

/*速度监测*/ 
void VilocityMonitor()
{
	int i,j;
	int xf=0,yf=0;  //记录vx，vy的正负 
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);  //检测是否触碰边界反弹 
		PotMonitor(i);    //检测是否进袋 
		
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
		if(Ball[i].state!=2){        //判断小球状态 ，静止为0，运动为1 
			if(Ball[i].vx==0&&Ball[i].vy==0){
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		}
		MonitorSwitch();   //检测是否发生碰撞 
	}
}		


/*当球重叠时微调球的位置*/                 //先运动再判断是否碰撞 
void Adjustlen(int i,int j)
{
	double linelen;   //两球球心距离
	
	linelen=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));
	
	Ball[i].x=0.4/linelen*(Ball[i].x-Ball[j].x)+Ball[j].x;
	Ball[i].y=0.4/linelen*(Ball[i].y-Ball[j].y)+Ball[j].y;
}

/*边界碰撞监测*/
void BorderMonitor(int i)     //边界碰撞应用镜面反射原理 ，触碰横向边界时y反向，x不变；触碰纵向边界时，x反向，y不变 
{
	if(Ball[i].x<=0.8)
	{
		Ball[i].x=0.8;
		Ball[i].vx=-Ball[i].vx;
	}
	if(Ball[i].x>=10.2)
	{
		Ball[i].x=10.2;
		Ball[i].vx=-Ball[i].vx;
	}
	if(Ball[i].y>=6.2)
	{
		Ball[i].y=6.2;
		Ball[i].vy=-Ball[i].vy;
	}
	if(Ball[i].y<=2.3)
	{
		Ball[i].y=2.3;
		Ball[i].vy=-Ball[i].vy;
	}
}

/*进袋监测*/
void PotMonitor(int i)   //当球心距离某个袋口圆心的距离小于等于袋口半径时可判定该球进洞，速度和坐标都重置，状态改为2（进袋） 
{
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed();
	}
}

/*定时器回调函数*/ 
void TimerEventProcess(int timerID)
{
 	if(timerID==1){  //球的定时器 
 		VilocityMonitor();     //速度监测，控制球的运动 
 		Newed();      //刷新每一帧 
 		startrun=Runningjudge();     //判断所有球是否都静止，如果都静止，则startrun=0 
 		if(startrun==0){             //如果所有球静止，judgepower=0，即开启力度条等待击球 
 			judgepower=0;
 			cancelTimer(1);     //关闭计时器1，避免影响力度条显示 
		}
 	}
 	if(timerID==2)  //力度条定时器 
 		PowerDisplay();    ///画动态力度条 
}

/*判断是否有球在动*/ 
int Runningjudge(){     //1表示有球在动，0表示全都静止 
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	if(t==1){ 
		if(Ball[8].state==2)     //判断黑（8）是否进袋，若已进袋，则游戏结束，进行胜负判定 
		{
			cancelTimer(1);
			cancelTimer(2);
			WinnerJudge();    //胜负判定 
		}
		else Playerchange();           //如果球都停下来了，就交换玩家 
	}
	return 0;
}

/*玩家交换*/
void Playerchange(){
	int i,j;
	measurepower=0;            //力度条初始化 
	ipower=0;                  //力度条变化初始化 
	if(player==1) player=2;
	else player=1;            //交换杆的颜色 
	/*白球进洞的话重新初始化白球位置*/ 
	if(Ball[0].state==2 ){
		Ball[0].state =0;   //状态改为静止 
		Ball[0].x=2.6;     //初始化坐标 
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
	Newed();                 //更新界面 
} 

/*判断游戏胜负*/
void WinnerJudge()   //原理：进黑（8）后，若当前玩家的球（玩家1是色球，玩家2是花球）已经全部进袋，则该玩家获胜；若仍然有未进袋的该玩家的球，则判其对手获胜 
{
	int i;
	if(player==1)
	{
		for(i=1;i<=7;i++)
		    if(Ball[i].state!=2)
		    	winner=2;
		if(winner!=2) winner=1;
	}
	if(player==2)
	{
		for(i=9;i<=15;i++)
		    if(Ball[i].state!=2)
		    	winner=1;
		if(winner!=1) winner=2;
	}
	Outcome1(winner);   //进入结算界面 
}



/*台球桌面鼠标回调函数*/
void MouseEventProcess(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*鼠标在台球桌面*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower==0){
				//if(measurepower==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //生成引导线和球杆 
						Newed();  //刷新界面 
		
						if(Ball[0].state==0){      //白球停止生成引导线                     
		   				 	/*生成引导线*/ 
		   					SetPenColor("White");
			    			MovePen(mox,moy);
			    			DrawLine(Ball[0].x - mox,Ball[0].y - moy);
			    			MovePen(mox+0.2,moy);
		     				DrawArc(0.2,0,360);
		
			   			 	/*生成球杆，玩家1棕杆，玩家2暗红杆*/
			    			int pen;
			    			double sin,cos;
			    			pen=GetPenSize();
			    			if(player==1){                      //玩家1杆子颜色为棕 
				    			SetPenSize(pen+4);
				    			DefineColor("Temp",0.2305,0.2227,0.043);
				    			SetPenColor("Temp");
				    			sin=(moy-Ball[0].y)/sqrt(pow(mox-Ball[0].x,2)+pow(moy-Ball[0].y,2));
								cos=(mox-Ball[0].x)/sqrt(pow(mox-Ball[0].x,2)+pow(moy-Ball[0].y,2));
								cos1=cos;
								sin1=sin;
								MovePen(Ball[0].x-0.2*cos,Ball[0].y-0.2*sin);
								DrawLine(-cos*3,-sin*3);
								SetPenSize(pen);	
	        				} 
	        				if(player==2){                      //玩家2杆子为暗红色 
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
    			}
				break;
			}
			case BUTTON_UP:{
				if(button==LEFT_BUTTON){
					judgepower=1;  //判断方向是否锁定 
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
					cancelTimer(2);
					cancelTimer(1);                                  //关闭计时器，避免影响界面显示 
					SaveTxt1();
					DisplayClear();  //清屏
					GameSelect();    //跳回游戏模式选择 
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //移动到保存2按下状态 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //关闭计时器，避免影响界面显示 
					SaveBinary1();
					DisplayClear();  //清屏
					GameSelect();    //跳回游戏模式选择
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //移动到RETURN按下状态 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //关闭计时器，避免影响界面显示 
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //移动到HELP按下状态 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //关闭计时器，避免影响界面显示 
					rulereturn1=1;
					SaveTxt1();
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



/*设置球的初始状态*/
void InitialBall()   
{
	int i;
	for(i=0;i<=15;i++)  //设置初速度为零，状态为静止 
	{
		Ball[i].state=0;
		Ball[i].vx=0;
		Ball[i].vy=0;
	}
	/*设置坐标和颜色参数*/ 
	Ball[0].x=2.6;
	Ball[0].y=4.25;
	Ball[0].R=1;
	Ball[0].G=1;
	Ball[0].B=1;
	
	Ball[1].x=7;
	Ball[1].y=4.25;
	Ball[1].R=0.9844;
	Ball[1].G=0.9922;
	Ball[1].B=0.2500;
	
	Ball[2].x=Ball[1].x+0.2*sqrt(3);
	Ball[2].y=4.45;
	Ball[2].R=0.0703;
	Ball[2].G=0.0546;
	Ball[2].B=0.5703;
	
	Ball[3].x=Ball[1].x+0.2*sqrt(3);
	Ball[3].y=4.05;
	Ball[3].R=0.9531;
	Ball[3].G=0.1172;
	Ball[3].B=0.1484;
	
	Ball[4].x=Ball[1].x+0.4*sqrt(3);
	Ball[4].y=4.25;
	Ball[4].R=0.5156;
	Ball[4].G=0.2578;
	Ball[4].B=0.0273;
	
	Ball[5].x=Ball[1].x+0.4*sqrt(3);
	Ball[5].y=4.65;
	Ball[5].R=0.9766;
	Ball[5].G=0.4141;
	Ball[5].B=0.043;
	
	Ball[6].x=Ball[1].x+0.4*sqrt(3);
	Ball[6].y=3.85;
	Ball[6].R=0;
	Ball[6].G=0.5;
	Ball[6].B=0.2422;
	
	Ball[7].x=Ball[1].x+0.6*sqrt(3);
	Ball[7].y=4.45;
	Ball[7].R=0.7148;
	Ball[7].G=0.0938;
	Ball[7].B=0.8945;
	
	Ball[8].x=Ball[1].x+0.6*sqrt(3);
	Ball[8].y=4.85;
	Ball[8].R=0;
	Ball[8].G=0;
	Ball[8].B=0;
	
	Ball[9].x=Ball[1].x+0.6*sqrt(3);
	Ball[9].y=4.05;
	Ball[9].R=0.9844;
	Ball[9].G=0.9922;
	Ball[9].B=0.2500;
	
	Ball[10].x=Ball[1].x+0.6*sqrt(3);
	Ball[10].y=3.65;
	Ball[10].R=0.0703;
	Ball[10].G=0.0546;
	Ball[10].B=0.5703;
	
	Ball[11].x=Ball[1].x+0.8*sqrt(3);
	Ball[11].y=4.25;
	Ball[11].R=0.9531;
	Ball[11].G=0.1172;
	Ball[11].B=0.1484;
	
	Ball[12].x=Ball[1].x+0.8*sqrt(3);
	Ball[12].y=4.65;
	Ball[12].R=0.5156;
	Ball[12].G=0.2578;
	Ball[12].B=0.0273;
	
	Ball[13].x=Ball[1].x+0.8*sqrt(3);
	Ball[13].y=5.05;
	Ball[13].R=0.9766;
	Ball[13].G=0.4141;
	Ball[13].B=0.043;
	
	Ball[14].x=Ball[1].x+0.8*sqrt(3);
	Ball[14].y=3.85;
	Ball[14].R=0;
	Ball[14].G=0.5;
	Ball[14].B=0.2422;
	
	Ball[15].x=Ball[1].x+0.8*sqrt(3);
	Ball[15].y=3.45;
	Ball[15].R=0.7148;
	Ball[15].G=0.0938;
	Ball[15].B=0.8945;
}

/*刷新台球面*/ 
void Newed()   
{
	DisplayClear();  //清屏 
	Table();      //重画桌面 
	UnderDisplay();   //重画底部栏目 
	DrawBall();   //画出当前状态的所有的球 
}
 

/*画球*/ 
void DrawBall()  
{
	int i;
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2)
		{
			/*画底色*/
			SetPenSize(1);
			DefineColor("Temp",Ball[i].R,Ball[i].G,Ball[i].B);
			SetPenColor("Temp");
			MovePen(Ball[i].x+0.2,Ball[i].y);
			StartFilledRegion(1);
	        DrawArc(0.2,0,360);
	        EndFilledRegion();
	        
	        /*画9-15号球的白块*/
			if(i>8)
	        {
	        	SetPenColor("White");
			    MovePen(Ball[i].x+0.12,Ball[i].y);
			    StartFilledRegion(1);
	            DrawArc(0.12,0.0,360.0);
	            EndFilledRegion();
			}
			
			/*画球边缘线*/
			SetPenSize(5);
			SetPenColor("Black");
			MovePen(Ball[i].x+0.2,Ball[i].y);
			DrawArc(0.2,0.0,360.0);
			SetPenSize(1);
			
			/*标数字*/
			char label[100];
			if(i>0&&i<=8)
			{
			    SetPointSize(27);            
				SetPenColor("White");
				MovePen(Ball[i].x-0.09,Ball[i].y-0.1);
			    label[0]=48+i;
			    label[1]=0;
				DrawTextString(label);
				SetPointSize(12);
		    }
		    if(i==9)
		    {
			    SetPointSize(27);
				SetPenColor("Black");
				MovePen(Ball[i].x-0.09,Ball[i].y-0.1);
			    label[0]=48+i;
			    label[1]=0;
				DrawTextString(label);
				SetPointSize(12);
		    }
			if(i>9)
		    {
			    SetPointSize(12+10);
				SetPenColor("Black");
				MovePen(Ball[i].x-0.1,Ball[i].y-0.07);
			    label[0]=49;
			    label[1]=48+i-10;
			    label[2]=0;
				DrawTextString(label);
				SetPointSize(12);
		    }
		}
	}
}

/*桌面*/ 
void Table()   
{
	DefineColor("Deepgreen",0.3398,0.4883,0.0586);
	DefineColor("Shibrawn",0.2695,0.0547,0.0547);
	SetPenColor("Shibrawn");
	SetPenSize(1);
	DrawBox(0.0,1.5,11.0,5.5);
	SetPenColor("Deepgreen");
	DrawBox(0.6,2.1,9.8,4.3);
	
	DefineColor("Coalgrey",0.4531,0.4570,0.4453); 
	SetPenColor("Coalgrey");
	MovePen(1,2.1);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
	
	MovePen(10.8,2.1);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
	
	MovePen(10.8,6.4);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
	
	MovePen(1,6.4);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
	
	MovePen(5.9,2.1);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
	
	MovePen(5.9,6.4);
	StartFilledRegion(1);
	DrawArc(0.4,0,360);
	EndFilledRegion();
}

/*下部分数菜单栏*/ 
void UnderDisplay(){
	/*底面边框*/ 
	DefineColor("lightorange",.95,.63,.0351);
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
	DrawLine(0.0,-1.45);
	MovePen(6.46,1.45);
	DrawLine(0.0,-1.45);
	
	/*画提示语*/
	SetStyle(0);
	SetPenColor("Red");
	SetPointSize(30);
	SetPenSize(5);
	if(player==1) MovePen(3.5,0.25);
	    else MovePen(7.6,0.25);
	DrawTextString("轮到你击球了！"); 
	
	/*画表示进球的数字*/
	BallNumberRecord();
	
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
	SetStyle(1);      //画笔加粗 
	DrawTextString("POWER SELECT");
	
	/*按键初始化*/
	SetStyle(0);    //回归原来 
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
	MovePen(10.44,1.226);
	DrawTextString("保存txt");          //保存文本文件 
	MovePen(10.44,.926);
	DrawTextString("保存dat");          //保存二进制文件 
	MovePen(10.44,.665);
	DrawTextString(" Return");         //返回主界面
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //退出
	MovePen(10.44,.385);
	DrawTextString(" Help");           //帮助 
	
	/*玩家模块初始化*/
	SetPointSize(20);
	MovePen(4.3,1.2);
	DrawTextString("Player1");
	MovePen(8.4,1.2);
	DrawTextString("Player2"); 
 }

/*画记录进球的数字*/ 
void BallNumberRecord()      //进袋的球标为橙色，未进袋的标为黑色 
{
	int i;
	double gap=0.5;
	SetPointSize(30);
	for(i=1;i<=7;i++)
	{
		if(Ball[i].state==2) SetPenColor("lightorange");
		    else SetPenColor("Black");
		switch(i){
			case 1:MovePen(2.7+gap,0.7);
				   DrawTextString("1");
				   break;
			case 2:MovePen(3.1+gap,0.7);
				   DrawTextString("2");
				   break;
			case 3:MovePen(3.5+gap,0.7);
				   DrawTextString("3");
				   break;
			case 4:MovePen(3.9+gap,0.7);
				   DrawTextString("4");
				   break;
			case 5:MovePen(4.3+gap,0.7);
				   DrawTextString("5");
				   break;
			case 6:MovePen(4.7+gap,0.7);
				   DrawTextString("6");
				   break;
			case 7:MovePen(5.1+gap,0.7);
				   DrawTextString("7");
				   break;
		}
	}
	
	gap=0.2;	
	for(i=9;i<=15;i++)
	{
		if(Ball[i].state==2) SetPenColor("lightorange");
		    else SetPenColor("Black");
		switch(i){
			case 9:MovePen(6.8+gap,0.7);
				   DrawTextString("9");
				   break;
			case 10:MovePen(7.1+gap,0.7);
				   DrawTextString("10");
				   break;
			case 11:MovePen(7.6+gap,0.7);
				   DrawTextString("11");
				   break;
			case 12:MovePen(8.1+gap,0.7);
				   DrawTextString("12");
				   break;
			case 13:MovePen(8.6+gap,0.7);
				   DrawTextString("13");
				   break;
			case 14:MovePen(9.1+gap,0.7);
				   DrawTextString("14");
				   break;
			case 15:MovePen(9.6+gap,0.7);
				   DrawTextString("15");
				   break;
		}
	}
}

 
 /*力度条变化*/ 
 void PowerDisplay()
 {
	int i=0;
	if(judgepower==1&&measurepower==0){
		registerKeyboardEvent(Power_Keyboard);
		if(ipower<=6){
			SetPenColor("myyellow");
			DrawBox(.3+ipower*0.3,.425,.13,0.65);	
		}
		else if(ipower>=7 && ipower<=13){
			SetPenColor("gray");
			DrawBox(4.2-ipower*0.3,.425,.13,0.65);
		}
		if(ipower==13) ipower=-1;
		ipower++;
	}    
 }
 
 void Power_Keyboard(int key, int event){
 	switch(key)
	{
		/*按下空格，确定力度*/
		case VK_SPACE:{
			if(judgepower==1)
				measurepower=1;  //标志球已经确定力度，防止力度条再次滚动 
			/*赋予白球速度*/ 
			if(judgepower==1&&measurepower==1)    
			{
				double power=0;  //打击力的大小 
				if(ipower>=0 && ipower<=6){
					power=3+ipower;
				}
				else if(ipower>=7 && ipower<=13){
					power=16-ipower;
				}
				startTimer(1,timegap);       //开启球滚动的timer，赋予白球速度 
				Ball[0].vx=0.015*cos1*power;    
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun=1;                  //小球开始处于运动 
				cancelKeyboardEvent();       //关闭键盘操作，避免球运动过程中误触键盘给白球重新赋予初速度 
			}
			break;
		}
	}
 }
 
 
/*以文本模式保存*/           
void SaveTxt1(){                                
	FILE *fp1;
	int i=0;
	if(rulereturn1==1){
		if((fp1=fopen("Game1Temp.txt","w"))==NULL)           //判断能否正常打开Game1Temp.txt
			exit(0);
	}	
	else if(rulereturn1==0) {
		if((fp1=fopen("SaveGameDouble1.txt","w"))==NULL)      //判断能否正常打开SaveGameDouble1.txt
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
	/*转存初始化数据*/
	fprintf(fp1,"%d\n",player);
	fprintf(fp1,"%d\n",judgepower);  //关于力度条数据 
	fprintf(fp1,"%d\n",ipower); 
	fprintf(fp1,"%d\n",measurepower); 
	fprintf(fp1,"%d\n",startrun);    //运动数据 
	fprintf(fp1,"%lf\n",sin1);       //储存杆的方向 
	fprintf(fp1,"%lf\n",cos1);  
	if(fclose(fp1)){
		exit(0); 
	}                                                                                
}

/*保存当前游戏数据到“SaveGameDouble2.dat ”二进制文件*/
void SaveBinary1()
{
	FILE *fp2;
	int i;
	if((fp2=fopen("SaveGameDouble2.dat","wb"))==NULL){    //判断能否正常打开Game1Temp.txt
		exit(0);
	}
	/*转存小球数据*/
	for(i=0;i<=15;i++)                                       
	{
		fwrite(&Ball[i].state,sizeof(int),1,fp2);         //储存小球状态 
		fwrite(&Ball[i].vx,sizeof(double),1,fp2);         //储存小球速度 
		fwrite(&Ball[i].vy,sizeof(double),1,fp2);        
		fwrite(&Ball[i].x,sizeof(double),1,fp2);          //储存小球位置 
		fwrite(&Ball[i].y,sizeof(double),1,fp2);
	}
	/*转存初始化数据*/
	fwrite(&player,sizeof(int),1,fp2);                      //玩家数据 
	fwrite(&judgepower,sizeof(int),1,fp2);                  //力度条相关数据 
	fwrite(&ipower,sizeof(int),1,fp2);
	fwrite(&measurepower,sizeof(int),1,fp2);
	fwrite(&startrun,sizeof(int),1,fp2);                    //小球是否在运动 
	fwrite(&sin1,sizeof(double),1,fp2);                     //杆子的方向 
	fwrite(&cos1,sizeof(double),1,fp2);
	if(fclose(fp2)){                                       //关闭文件 
		exit(0);
	}
}


