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

/*模式3中的数据*/ 
extern int gamemode;      
double distance3[20][20];
double u3=9;         //摩擦产生的加速度（减速） 
double timegap3=0.006;//时间间隔 
int ptime3=60;      //力度条变化的时间 
int judgepower3=0;   //判断力度条是否开启 
int ipower3=0;       //记录力度大小 
int measurepower3=0; //判断是否按下按键，力度条是否停止运动 
int startrun3=0;     //判断小球是否在运动 
double cos1,sin1;   //确定小球运动的方向 

ball Ball[16];
 
void SaveTxt3();    //暂时保存mode3中的数据 
void InitialPower3();              //初始化力度条 

void UnderDisplay3();              //下面的菜单栏 
void MouseEventProcess3(int x,int y,int button,int event);
void TimerEventProcess3(int timerID);         //判断速度，力度条，球位置的timer 
void PowerDisplay3();                        //力度条颜色改变
void Power_Keyboard3(int key, int event);     //决定力度大小的键盘事件 
void Gamemode3();            //游戏模式3 
void Newed3();               //刷新界面 
void MonitorSwitch3();       //速度监测转碰撞监测或微调函数
void VilocityMonitor3();     //速度检测 
void PotMonitor3(int i);     //进袋监测 
void Playercontinue3();      //继续击球 

extern void BorderMonitor(int i);  //边界碰撞 
extern void CollisionMonitor(int i,int j);//碰撞监测
extern void DrawBall();      //画球 
extern void Adjustlen(int i,int j);//小球重叠微调
extern void Table();               //桌面 
extern void InitialBall();         //初始化球的位置 
extern void Playerchange();        //交换玩家 
extern void InitialPower();        //初始化力度条
extern void BallNumberRecord();    //画记录进球的数字 
extern int Runningjudge();        //判断是否有小球滚动
extern double DistanceBall();    //计算两球之间的距离 
extern void DrawBox();
extern void DrawEmptyBox();

/*模式3*/
void Gamemode3()     
{
	InitialBall();  //球参数初始化 
	InitialPower3(); //力度条初始化 
	gamemode=3;     //转变为游戏模式3 
	int i,j;
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance3[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //设置球的初始距离 
		    
	registerTimerEvent(TimerEventProcess3);
    registerMouseEvent(MouseEventProcess3); 
	startTimer(2,ptime3);
	startTimer(3,1000);
	Newed3();
}

/*速度监测*/ 
void VilocityMonitor3()
{
	int i,j;
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);   //边界碰撞监测 
		PotMonitor3(i);     //进袋监测 
		
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
		
		if(Ball[i].state!=2)        //判断小球状态 
			if(Ball[i].vx==0&&Ball[i].vy==0)
			{
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		
		MonitorSwitch3();   //监测是否发生碰撞 
	}
}

/*速度监测转碰撞监测或微调函数*/
/*这部分原理是，当两球距离小于两个半径时，记录当时两球的距离tempdistance*/
/*当tempdistance小于原来的距离Ball[i].distance[j]时，即两球在靠近，触发碰撞模型改变两球速度*/
/*反之，表示两球碰撞完了正在相互远离，触发微调模型减少穿模现象*/ 
void MonitorSwitch3() 
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
				if(tempdistance<distance3[i][j])      //两球在靠近，触发碰撞模型改变两球速度
				{
					CollisionMonitor(i,j);  //碰撞 
					distance3[i][j]=tempdistance;   //将临时求出的两球距离存入距离参数distance中 
					distance3[j][i]=tempdistance;
				}
				else                     //两球碰撞完了正在相互远离，触发微调模型减少穿模现象
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))   //微调函数中只对Ball[i]进行微调，选择速度偏小的那一个球作为Ball[i]
						Adjustlen(i,j);  //微调 
					  else Adjustlen(j,i);
				}
			}
			else
			 {
					distance3[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));     //球心距离大于两个半径，不发生碰撞，直接将临时求出的两球距离存入距离参数distance中
					distance3[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
			}
		for(j=i+1;j<=15;j++)        //同上，两部分合起来就是i球与所有其他球（除其本身）检测是否碰撞 
		{
	  		if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
	   		{
	   			tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));
				if(tempdistance<distance3[i][j])
				{
					CollisionMonitor(i,j);
					distance3[i][j]=tempdistance;
					distance3[j][i]=tempdistance;
				}
				else 
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
					    Adjustlen(i,j);
					  else Adjustlen(j,i);
				}
			}
			else {
					distance3[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
					distance3[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
		}
	}
}

/*进袋监测*/
void PotMonitor3(int i)   //当球心距离某个袋口圆心的距离小于等于袋口半径时可判定该球进洞，速度和坐标都重置，状态改为2（进袋）
{
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3(); 
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-2.1,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3();
	}
	if(pow(Ball[i].x-0.6,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3();
	}
	if(pow(Ball[i].x-5.5,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3();
	}
	if(pow(Ball[i].x-10.4,2)+pow(Ball[i].y-6.4,2)<=0.16)
	{
		Ball[i].x=0;Ball[i].y=0;
		Ball[i].vx=0;Ball[i].vy=0;
		Ball[i].state=2;
		Newed3();
	}
}

/*判断是否有球在动*/ 
int Runningjudge3(){     //1表示有球在动，0表示全都静止 
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	Playercontinue3();   //继续击球 
	return 0;
}

/*定时器回调函数*/ 
void TimerEventProcess3(int timerID)
{
 	int i,j;
	 if(timerID==1){  //球的定时器 
 		VilocityMonitor3();
 		Newed3();
 		startrun3=Runningjudge3();
 		if(startrun3==0){
 			judgepower3=0;
 			for(i=1;i<=15;i++)
 			{
 				if(Ball[i].state==2)     //如果球进袋了，将球的坐标重置，返回初始位置 
 				{
 					switch(i)
 					{
 						case 1:Ball[1].x=7;
							   Ball[1].y=4.25;
							   Ball[1].state=0;
							   break;
						case 2:Ball[2].x=7+0.2*sqrt(3);
							   Ball[2].y=4.45;
							   Ball[2].state=0;
							   break;
						case 3:Ball[3].x=7+0.2*sqrt(3);
           				       Ball[3].y=4.05;
						       Ball[3].state=0;
						       break;
						case 4:Ball[4].x=7+0.4*sqrt(3);
							   Ball[4].y=4.25;
							   Ball[4].state=0;
						       break;
						case 5:Ball[5].x=7+0.4*sqrt(3);
							   Ball[5].y=4.65;
							   Ball[5].state=0;
							   break;
					    case 6:Ball[6].x=7+0.4*sqrt(3);
	                           Ball[6].y=3.85;
	                           Ball[6].state=0;
	                           break;
	                    case 7:Ball[7].x=7+0.6*sqrt(3);
	                           Ball[7].y=4.45;
	                           Ball[7].state=0;
	                           break;
	                    case 8:Ball[8].x=7+0.6*sqrt(3);
							   Ball[8].y=4.85;
							   Ball[8].state=0;
							   break;
						case 9:Ball[9].x=7+0.6*sqrt(3);
							   Ball[9].y=4.05;
						       Ball[9].state=0;
							   break;
						case 10:Ball[10].state=0;
								Ball[10].x=7+0.6*sqrt(3);
								Ball[10].y=3.65;
							   break;
						case 11:Ball[11].state=0;
						        Ball[11].x=7+0.8*sqrt(3);
								Ball[11].y=4.25;
							   break;
						case 12:Ball[12].state=0;
								Ball[12].x=7+0.8*sqrt(3);
								Ball[12].y=4.65;
							   break;
						case 13:Ball[13].state=0;
								Ball[13].x=7+0.8*sqrt(3);
								Ball[13].y=5.05;
							   break;
						case 14:Ball[14].state=0;
								Ball[14].x=7+0.8*sqrt(3);
								Ball[14].y=3.85;
							   break;
						case 15:Ball[15].state=0;
								Ball[15].x=7+0.8*sqrt(3);
								Ball[15].y=3.45;
							   break;
					 }
					 for(j=0;j<=i-1;j++)    //球重生时可能会和别的球重叠，进行微调 
					{
	  	  				if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
					 	if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
							Adjustlen(i,j);
					  	else Adjustlen(j,i);
					}
			    }
			}
			Newed3();
 			cancelTimer(1);     //关闭计时器1，避免影响力度条显示 
		}
 	}
 	if(timerID==2)  //力度条定时器 
 		PowerDisplay3();
}

void MouseEventProcess3(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*鼠标在台球桌面*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower3==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //生成引导线和球杆 
						Newed3();  //刷新界面 
		
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
					judgepower3=1;
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
					SaveTxt3();
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
void Newed3()   
{
	int i,j;
	DisplayClear();
	Table();
	UnderDisplay3();
	for(i=1;i<=15;i++){
 		if(Ball[i].state==2)     //如果球进袋了，将球的坐标重置，返回初始位置 
 		{
			for(j=0;j<=i-1;j++)    //球重生时可能会和别的球重叠，进行微调 
			{
	  			if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
						Adjustlen(i,j);
				else Adjustlen(j,i);
			}
		}
	}
	DrawBall();
}

/*下部分数菜单栏*/ 
void UnderDisplay3(){
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
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
	DrawTextString("POWER SELECT");
	
	/*按键初始化*/
	int t=0;
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
	SetPointSize(9);
	SetStyle(0);
	SetPointSize(14);
	MovePen(10.44,.665);
	DrawTextString(" Return");         //返回主界面
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //退出
	MovePen(10.44,.385);
	DrawTextString(" Help");           //帮助 
 }
 
  /*力度条变化*/ 
 void PowerDisplay3()
 {
	if(judgepower3==1&&measurepower3==0){
		registerKeyboardEvent(Power_Keyboard3);
		if(ipower3<=6){
			SetPenColor("myyellow");
			DrawBox(.3+ipower3*0.3,.425,.13,0.65);
		}
		else if(ipower3>=7 && ipower3<=13){
			SetPenColor("gray");
			DrawBox(4.2-ipower3*0.3,.425,.13,0.65);
		}
		if(ipower3==13) ipower3=-1;
		ipower3++;
	}
 }
 
 void Power_Keyboard3(int key, int event){
 	switch(key)
	{
		/*按下空格，确定力度*/
		case VK_SPACE:{
			if(judgepower3==1)
				measurepower3=1;  //标志球已经确定力度，防止力度条再次滚动 
			/*赋予白球速度*/ 
			if(judgepower3==1&&measurepower3==1)    
			{
				double power=0;//打击力的大小 
				if(ipower3>=0 && ipower3<=6){
					power=3+ipower3;
				}
				else if(ipower3>=7 && ipower3<=13){
					power=16-ipower3;
				}
				startTimer(1,timegap3);       //开启球滚动的timer，赋予白球速度 
				Ball[0].vx=0.015*cos1*power;
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun3=1;                  //小球开始处于运动 
				cancelKeyboardEvent();
			}
			break;
		}
	}
 }
 
 /*力度条初始化*/                     //防止在返回后界面力度条和杆子显示出现问题 
void InitialPower3(){
    judgepower3=0;    
    ipower3=0;        
    measurepower3=0;  
    startrun3=0;     
} 

/*保存gamemode3中的暂时数据*/           
void SaveTxt3(){                                
	FILE *fp1;
    int i=0;
    if((fp1=fopen("Game3Temp.txt","w"))==NULL){  //判断能否正常打开Game3Temp.txt
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
 	fprintf(fp1,"%d\n",judgepower3);  //关于力度条数据 
 	fprintf(fp1,"%d\n",ipower3); 
 	fprintf(fp1,"%d\n",measurepower3); 
 	fprintf(fp1,"%d\n",startrun3);    //运动数据 
 	fprintf(fp1,"%lf\n",sin1);       //储存杆的方向 
 	fprintf(fp1,"%lf\n",cos1);  
 	if(fclose(fp1)){
   		exit(0); 
 	}                                                                                
}

/*模式3继续*/
void Playercontinue3()
{
 	measurepower3=0;            //力度条初始化 
	ipower3=0;                  //力度条变化初始化 
	if(Ball[0].state==2 ){
		Ball[0].state =0;
		Ball[0].x=2.6;
		Ball[0].y=4.25;
	}
	Newed3();                 //更新界面 
}

