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

 
/*ģʽ1�е�����*/ 
double distance[20][20];   //��֮��ľ���
double timegap=0.006;   //ʱ���� 
int ptime=100;      //�������仯��ʱ�� 
int judgepower=0;   //�ж��������Ƿ��� 
int ipower=0;       //��¼���ȴ�С 
int measurepower=0; //�ж��Ƿ��°������������Ƿ�ֹͣ�˶� 
int startrun=0;     //�ж�С���Ƿ����˶� 
int player=1;       //Ԥ�����Ϊ1 
int winner=1;       //Ԥ��ʤ��Ϊ1 
double cos1,sin1;   //ȷ��С���˶��ķ��� 
ball Ball[16];      //������
int rulereturn1;     //�ж��Ƿ�ӹ�����淵��  

extern int gamemode;     //��¼ģʽgamemode1 

void MonitorSwitch();        //�ٶȼ��ת��ײ����΢������
void Table();                //���� 
void UnderDisplay();         //����Ĳ˵��� 
void MouseEventProcess(int x,int y,int button,int event);
void TimerEventProcess(int timerID);         //�ж��ٶȣ�����������λ�õ�timer 
void PowerDisplay();                        //��������ɫ�ı�
void Power_Keyboard(int key, int event);      //�������ȴ�С�ļ����¼� 
void Gamemode1();           //˫�˶�սģʽ 
void Newed();               //ˢ�½��� 
void DrawBall();            //���� 
void InitialBall();         //��ʼ�����λ�� 
void VilocityMonitor();     //�ٶȼ�� 
void BorderMonitor(int i);  //�߽���ײ 
void CollisionMonitor(int i,int j);//��ײ���
void PotMonitor(int i);     //������� 
void Adjustlen(int i,int j);//С���ص�΢�� 
void Playerchange();        //������� 
void InitialPower();        //��ʼ��������
void BallNumberRecord();    //����¼��������� 
void WinnerJudge();             //�ж���Ϸʤ�� 
void Outcome(int winernum);      //������� 
void SaveBinary1();          //�洢�������ļ�
void SaveTxt1();             //�����ı��ļ� 

int Runningjudge();        //�ж��Ƿ���С�����
double DistanceBall();    //��������֮��ľ��� 

extern void DrawBox();
extern void DrawEmptyBox();
extern void Basic();
extern void Outcome1();


/*ģʽ1*/
void Gamemode1()     
{
	InitialBall();   //��ʼ������� 
	InitialPower();  //��ʼ�������� 
	int i,j; 
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //������ĳ�ʼ���� 
	registerTimerEvent(TimerEventProcess);
    registerMouseEvent(MouseEventProcess);
    gamemode=1;      //��¼ģʽΪģʽ1���浵ʱ��Ҫ�ã� 
	startTimer(2,ptime);
	Newed();
}

/*��������ʼ��*/                     //��ֹ�ڱ��淵�غ�����������͸�����ʾ�������� 
void InitialPower(){
	judgepower=0;    
	ipower=0;        
    measurepower=0;  
    startrun=0;     
} 

/*��ײ���*/
void CollisionMonitor(int i,int j)
{
	double sy,sx,tx,ty;  //s��ʾ�����������߷��򣩣�t��ʾ���� 
	
	sx=Ball[i].x-Ball[j].x;   // ���㷨���������� 
	sy=Ball[i].y-Ball[j].y;
	tx=-sy;
	ty=sx;

	double e;           
	e=sqrt(sx*sx+sy*sy); //�������ĵ�λ����
	sx=sx/e;      
	sy=sy/e;
	e=sqrt(tx*tx+ty*ty);  //�������ĵ�λ���� 
	tx=tx/e;
	ty=ty/e;

	double tvi,tvj,svi,svj;              //�������������ͷ�����ٶ� 
	svi=Ball[i].vx*sx+Ball[i].vy*sy;
	svj=Ball[j].vx*sx+Ball[j].vy*sy;
	tvi=Ball[i].vx*tx+Ball[i].vy*ty;
	tvj=Ball[j].vx*tx+Ball[j].vy*ty;

	double temp;        //������ײ����ٶȣ�ԭ�������൱�������������������������ٶȣ������ٶȲ��ı� 
	temp=svi;
	svi=svj;
	svj=temp;

	Ball[i].vx=svi*sx+tvi*tx;      //�����ײ������������ٶ� 
	Ball[i].vy=svi*sy+tvi*ty;
	Ball[j].vx=svj*sx+tvj*tx;
	Ball[j].vy=svj*sy+tvj*ty;
}

/*�ٶȼ��ת��ײ����΢������*/
/*�ⲿ��ԭ���ǣ����������С�������뾶ʱ����¼��ʱ����ľ���tempdistance*/
/*��tempdistanceС��ԭ���ľ���Ball[i].distance[j]ʱ���������ڿ�����������ײģ�͸ı������ٶ�*/
/*��֮����ʾ������ײ���������໥Զ�룬����΢��ģ�ͼ��ٴ�ģ����*/ 
void MonitorSwitch() 
{
	int i,j; 
	double tempdistance;   //��ʱ��¼������� 
	for(i=0;i<=14;i++)
	{
		for(j=0;j<=i-1;j++)
		{
	  	  if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)   //���ľ���С�ڵ��������뾶��������Ӵ���������ײ����ײ�պý��� 
	   	 	{
				tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));   //��ʱ�����������ľ��� 
				if(tempdistance<distance[i][j]&&(Ball[i].state==1||Ball[j].state==1))      //�����ڿ�����������ײģ�͸ı������ٶ�
				{
					CollisionMonitor(i,j);  //��ײ 
					distance[i][j]=tempdistance;   //����ʱ���������������������distance�� 
					distance[j][i]=tempdistance;
				}
				else                      //������ײ���������໥Զ�룬����΢��ģ�ͼ��ٴ�ģ����
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))  //΢��������ֻ��Ball[i]����΢����ѡ���ٶ�ƫС����һ������ΪBall[i] 
						Adjustlen(i,j);  //΢�� 
					  else Adjustlen(j,i);
				}
			}
			else {
					distance[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //���ľ�����������뾶����������ײ��ֱ�ӽ���ʱ���������������������distance��
					distance[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
		}
		for(j=i+1;j<=15;j++)        //ͬ�ϣ������ֺ���������i�������������򣨳��䱾������Ƿ���ײ 
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

/*�ٶȼ��*/ 
void VilocityMonitor()
{
	int i,j;
	int xf=0,yf=0;  //��¼vx��vy������ 
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);  //����Ƿ����߽練�� 
		PotMonitor(i);    //����Ƿ���� 
		
		sin=Ball[i].vy/(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)));
		cos=Ball[i].vx/(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)));
		
		Ball[i].x=Ball[i].x+Ball[i].vx;
  		Ball[i].y=Ball[i].y+Ball[i].vy;
  		Ball[i].vx=Ball[i].vx*0.99;  //�����˶��ٶȼ�С 
  		Ball[i].vy=Ball[i].vy*0.99; 
		
		/*�ж�С���ٶ�*/
		if (Ball[i].state!=2&&(sqrt(pow(Ball[i].vx,2)+pow(Ball[i].vy,2)))<=0.001){             //С��һ��֮��Ϊ0 
			Ball[i].vx=0;
			Ball[i].vy=0; 
			Ball[i].state=0;
		} 
		if(Ball[i].state!=2){        //�ж�С��״̬ ����ֹΪ0���˶�Ϊ1 
			if(Ball[i].vx==0&&Ball[i].vy==0){
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		}
		MonitorSwitch();   //����Ƿ�����ײ 
	}
}		


/*�����ص�ʱ΢�����λ��*/                 //���˶����ж��Ƿ���ײ 
void Adjustlen(int i,int j)
{
	double linelen;   //�������ľ���
	
	linelen=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));
	
	Ball[i].x=0.4/linelen*(Ball[i].x-Ball[j].x)+Ball[j].x;
	Ball[i].y=0.4/linelen*(Ball[i].y-Ball[j].y)+Ball[j].y;
}

/*�߽���ײ���*/
void BorderMonitor(int i)     //�߽���ײӦ�þ��淴��ԭ�� ����������߽�ʱy����x���䣻��������߽�ʱ��x����y���� 
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

/*�������*/
void PotMonitor(int i)   //�����ľ���ĳ������Բ�ĵľ���С�ڵ��ڴ��ڰ뾶ʱ���ж�����������ٶȺ����궼���ã�״̬��Ϊ2�������� 
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

/*��ʱ���ص�����*/ 
void TimerEventProcess(int timerID)
{
 	if(timerID==1){  //��Ķ�ʱ�� 
 		VilocityMonitor();     //�ٶȼ�⣬��������˶� 
 		Newed();      //ˢ��ÿһ֡ 
 		startrun=Runningjudge();     //�ж��������Ƿ񶼾�ֹ���������ֹ����startrun=0 
 		if(startrun==0){             //���������ֹ��judgepower=0���������������ȴ����� 
 			judgepower=0;
 			cancelTimer(1);     //�رռ�ʱ��1������Ӱ����������ʾ 
		}
 	}
 	if(timerID==2)  //��������ʱ�� 
 		PowerDisplay();    ///����̬������ 
}

/*�ж��Ƿ������ڶ�*/ 
int Runningjudge(){     //1��ʾ�����ڶ���0��ʾȫ����ֹ 
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	if(t==1){ 
		if(Ball[8].state==2)     //�жϺڣ�8���Ƿ���������ѽ���������Ϸ����������ʤ���ж� 
		{
			cancelTimer(1);
			cancelTimer(2);
			WinnerJudge();    //ʤ���ж� 
		}
		else Playerchange();           //�����ͣ�����ˣ��ͽ������ 
	}
	return 0;
}

/*��ҽ���*/
void Playerchange(){
	int i,j;
	measurepower=0;            //��������ʼ�� 
	ipower=0;                  //�������仯��ʼ�� 
	if(player==1) player=2;
	else player=1;            //�����˵���ɫ 
	/*��������Ļ����³�ʼ������λ��*/ 
	if(Ball[0].state==2 ){
		Ball[0].state =0;   //״̬��Ϊ��ֹ 
		Ball[0].x=2.6;     //��ʼ������ 
		Ball[0].y=4.25;
		for(i=0;i<=15;i++)   //����΢������ֹ������������ʱ���������ص� 
			for(j=0;j<=15;j++)
			{
	  	  		if(i!=j&&sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
				if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
					Adjustlen(i,j);
			    else Adjustlen(j,i);
			}
	}
	Newed();                 //���½��� 
} 

/*�ж���Ϸʤ��*/
void WinnerJudge()   //ԭ�����ڣ�8��������ǰ��ҵ������1��ɫ�����2�ǻ����Ѿ�ȫ�������������һ�ʤ������Ȼ��δ�����ĸ���ҵ�����������ֻ�ʤ 
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
	Outcome1(winner);   //���������� 
}



/*̨���������ص�����*/
void MouseEventProcess(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*�����̨������*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower==0){
				//if(measurepower==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //���������ߺ���� 
						Newed();  //ˢ�½��� 
		
						if(Ball[0].state==0){      //����ֹͣ����������                     
		   				 	/*����������*/ 
		   					SetPenColor("White");
			    			MovePen(mox,moy);
			    			DrawLine(Ball[0].x - mox,Ball[0].y - moy);
			    			MovePen(mox+0.2,moy);
		     				DrawArc(0.2,0,360);
		
			   			 	/*������ˣ����1�ظˣ����2�����*/
			    			int pen;
			    			double sin,cos;
			    			pen=GetPenSize();
			    			if(player==1){                      //���1������ɫΪ�� 
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
	        				if(player==2){                      //���2����Ϊ����ɫ 
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
					judgepower=1;  //�жϷ����Ƿ����� 
				}
				break;
			}
		}
	}
	/*����ڱ�ǩ��*/ 
	else if(moy>=0&&moy<=1.5){                   
		switch(event){
			case MOUSEMOVE:{
				SetPenColor("gray");
				if(mox>=10.42&&mox<=10.92&&moy>=1.20&&moy<=1.42){     //�ƶ�������1״̬ 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,1.20,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,1.20,0.5,0.22);
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //�ƶ�������2����״̬ 
					SetPenColor("gray"); 
					DrawEmptyBox(10.42,0.92,0.5,0.22);
				}
				else{
					SetPenColor("yaliyellow");
					DrawEmptyBox(10.42,0.92,0.5,0.22);
				}
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
				if(mox>=10.42&&mox<=10.92&&moy>=1.20&&moy<=1.42){     //�ƶ�������1״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //�رռ�ʱ��������Ӱ�������ʾ 
					SaveTxt1();
					DisplayClear();  //����
					GameSelect();    //������Ϸģʽѡ�� 
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //�ƶ�������2����״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //�رռ�ʱ��������Ӱ�������ʾ 
					SaveBinary1();
					DisplayClear();  //����
					GameSelect();    //������Ϸģʽѡ��
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //�ƶ���RETURN����״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //�رռ�ʱ��������Ӱ�������ʾ 
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //�ƶ���HELP����״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1);                                  //�رռ�ʱ��������Ӱ�������ʾ 
					rulereturn1=1;
					SaveTxt1();
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



/*������ĳ�ʼ״̬*/
void InitialBall()   
{
	int i;
	for(i=0;i<=15;i++)  //���ó��ٶ�Ϊ�㣬״̬Ϊ��ֹ 
	{
		Ball[i].state=0;
		Ball[i].vx=0;
		Ball[i].vy=0;
	}
	/*�����������ɫ����*/ 
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

/*ˢ��̨����*/ 
void Newed()   
{
	DisplayClear();  //���� 
	Table();      //�ػ����� 
	UnderDisplay();   //�ػ��ײ���Ŀ 
	DrawBall();   //������ǰ״̬�����е��� 
}
 

/*����*/ 
void DrawBall()  
{
	int i;
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2)
		{
			/*����ɫ*/
			SetPenSize(1);
			DefineColor("Temp",Ball[i].R,Ball[i].G,Ball[i].B);
			SetPenColor("Temp");
			MovePen(Ball[i].x+0.2,Ball[i].y);
			StartFilledRegion(1);
	        DrawArc(0.2,0,360);
	        EndFilledRegion();
	        
	        /*��9-15����İ׿�*/
			if(i>8)
	        {
	        	SetPenColor("White");
			    MovePen(Ball[i].x+0.12,Ball[i].y);
			    StartFilledRegion(1);
	            DrawArc(0.12,0.0,360.0);
	            EndFilledRegion();
			}
			
			/*�����Ե��*/
			SetPenSize(5);
			SetPenColor("Black");
			MovePen(Ball[i].x+0.2,Ball[i].y);
			DrawArc(0.2,0.0,360.0);
			SetPenSize(1);
			
			/*������*/
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

/*����*/ 
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

/*�²������˵���*/ 
void UnderDisplay(){
	/*����߿�*/ 
	DefineColor("lightorange",.95,.63,.0351);
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
	DrawLine(0.0,-1.45);
	MovePen(6.46,1.45);
	DrawLine(0.0,-1.45);
	
	/*����ʾ��*/
	SetStyle(0);
	SetPenColor("Red");
	SetPointSize(30);
	SetPenSize(5);
	if(player==1) MovePen(3.5,0.25);
	    else MovePen(7.6,0.25);
	DrawTextString("�ֵ�������ˣ�"); 
	
	/*����ʾ���������*/
	BallNumberRecord();
	
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
	MovePen(0.3,1.2);
	SetStyle(1);      //���ʼӴ� 
	DrawTextString("POWER SELECT");
	
	/*������ʼ��*/
	SetStyle(0);    //�ع�ԭ�� 
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
	DrawTextString("����txt");          //�����ı��ļ� 
	MovePen(10.44,.926);
	DrawTextString("����dat");          //����������ļ� 
	MovePen(10.44,.665);
	DrawTextString(" Return");         //����������
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //�˳�
	MovePen(10.44,.385);
	DrawTextString(" Help");           //���� 
	
	/*���ģ���ʼ��*/
	SetPointSize(20);
	MovePen(4.3,1.2);
	DrawTextString("Player1");
	MovePen(8.4,1.2);
	DrawTextString("Player2"); 
 }

/*����¼���������*/ 
void BallNumberRecord()      //���������Ϊ��ɫ��δ�����ı�Ϊ��ɫ 
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

 
 /*�������仯*/ 
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
		/*���¿ո�ȷ������*/
		case VK_SPACE:{
			if(judgepower==1)
				measurepower=1;  //��־���Ѿ�ȷ�����ȣ���ֹ�������ٴι��� 
			/*��������ٶ�*/ 
			if(judgepower==1&&measurepower==1)    
			{
				double power=0;  //������Ĵ�С 
				if(ipower>=0 && ipower<=6){
					power=3+ipower;
				}
				else if(ipower>=7 && ipower<=13){
					power=16-ipower;
				}
				startTimer(1,timegap);       //�����������timer����������ٶ� 
				Ball[0].vx=0.015*cos1*power;    
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun=1;                  //С��ʼ�����˶� 
				cancelKeyboardEvent();       //�رռ��̲������������˶��������󴥼��̸��������¸�����ٶ� 
			}
			break;
		}
	}
 }
 
 
/*���ı�ģʽ����*/           
void SaveTxt1(){                                
	FILE *fp1;
	int i=0;
	if(rulereturn1==1){
		if((fp1=fopen("Game1Temp.txt","w"))==NULL)           //�ж��ܷ�������Game1Temp.txt
			exit(0);
	}	
	else if(rulereturn1==0) {
		if((fp1=fopen("SaveGameDouble1.txt","w"))==NULL)      //�ж��ܷ�������SaveGameDouble1.txt
			exit(0);
	}
	/*ת��С������*/
	for(i=0;i<=15;i++){
		fprintf(fp1,"%d\n",Ball[i].state );       //����С��״̬ 
		fprintf(fp1,"%lf\n",Ball[i].vx);          //����С���ٶ�
		fprintf(fp1,"%lf\n",Ball[i].vy);
		fprintf(fp1,"%lf\n",Ball[i].x);           //����С��λ�� 
		fprintf(fp1,"%lf\n",Ball[i].y);
	}
	/*ת���ʼ������*/
	fprintf(fp1,"%d\n",player);
	fprintf(fp1,"%d\n",judgepower);  //�������������� 
	fprintf(fp1,"%d\n",ipower); 
	fprintf(fp1,"%d\n",measurepower); 
	fprintf(fp1,"%d\n",startrun);    //�˶����� 
	fprintf(fp1,"%lf\n",sin1);       //����˵ķ��� 
	fprintf(fp1,"%lf\n",cos1);  
	if(fclose(fp1)){
		exit(0); 
	}                                                                                
}

/*���浱ǰ��Ϸ���ݵ���SaveGameDouble2.dat ���������ļ�*/
void SaveBinary1()
{
	FILE *fp2;
	int i;
	if((fp2=fopen("SaveGameDouble2.dat","wb"))==NULL){    //�ж��ܷ�������Game1Temp.txt
		exit(0);
	}
	/*ת��С������*/
	for(i=0;i<=15;i++)                                       
	{
		fwrite(&Ball[i].state,sizeof(int),1,fp2);         //����С��״̬ 
		fwrite(&Ball[i].vx,sizeof(double),1,fp2);         //����С���ٶ� 
		fwrite(&Ball[i].vy,sizeof(double),1,fp2);        
		fwrite(&Ball[i].x,sizeof(double),1,fp2);          //����С��λ�� 
		fwrite(&Ball[i].y,sizeof(double),1,fp2);
	}
	/*ת���ʼ������*/
	fwrite(&player,sizeof(int),1,fp2);                      //������� 
	fwrite(&judgepower,sizeof(int),1,fp2);                  //������������� 
	fwrite(&ipower,sizeof(int),1,fp2);
	fwrite(&measurepower,sizeof(int),1,fp2);
	fwrite(&startrun,sizeof(int),1,fp2);                    //С���Ƿ����˶� 
	fwrite(&sin1,sizeof(double),1,fp2);                     //���ӵķ��� 
	fwrite(&cos1,sizeof(double),1,fp2);
	if(fclose(fp2)){                                       //�ر��ļ� 
		exit(0);
	}
}


