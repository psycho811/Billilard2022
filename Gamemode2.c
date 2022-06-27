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

/*ģʽ2�е�����*/
double distance2[20][20];
int score=0;                //�÷� 
char a[10]="\0";                 //�������ַ���ʽ��¼�÷ַ�����ʾ 
int ContinueShooting=0;     //�����÷ּ�¼ 
double timegap2=0.006;      //ʱ���� 
int ptime2=100;             //�������仯��ʱ�� 
int judgepower2=0;          //�ж��������Ƿ��� 
int ipower2=0;              //��¼���ȴ�С 
int measurepower2=0;        //�ж��Ƿ��°������������Ƿ�ֹͣ�˶� 
int startrun2=0;            //�ж�С���Ƿ����˶� 
int timerecord=0;           //��¼��Ϸ�Ѿ����е�ʱ�� 
int rulereturn2;            //�ж��Ƿ�ӹ�����淵�� 

int lastflag=1;      //�жϼ�ʱ���Ƿ��ǵ�һ����ʾ   
double cos1,sin1;   //ȷ��С���˶��ķ��� 
char lasttime[10];   //����ʱ 
int lasttimesize;    //����ʱ�ַ����ĳ��� 
ball Ball[16];

extern int gamemode;     //��Ϸģʽ 

extern void Table();                //���� 
void MonitorSwitch2();
extern void InitialBall();         //��ʼ�����λ��
extern void DrawBall();            //���� 
extern void BorderMonitor(int i);  //�߽���ײ 
extern void CollisionMonitor(int i,int j);//��ײ���
extern void Adjustlen(int i,int j);//С���ص�΢�� 

void UnderDisplay2();         //����Ĳ˵��� 
void MouseEventProcess2(int x,int y,int button,int event);
void TimerEventProcess2(int timerID);         //�ж��ٶȣ�����������λ�õ�timer 
void PowerDisplay2();                        //��������ɫ�ı�
void Power_Keyboard2(int key, int event);      //�������ȴ�С�ļ����¼� 
void Gamemode2();            //��Ϸģʽ2 
void Newed2();               //ˢ�½���
void InitialTimer();         //��ʼ������ʱ������return���ٴδ�ʱ��������� 
void VilocityMonitor2();     //�ٶȼ�� 
void PotMonitor2(int i);     //�������
void ContinuePot(int i);          //���������� 
void InitialPower2();        //��ʼ��������
void WinnerJudge2();             //�ж���Ϸʤ�� 
void ScoreRecord();         //������ҵ÷� 
void SaveTxt2();            //�����ı��ļ�
void SaveBinary2();         //����������ļ� 

extern int Runningjudge();        //�ж��Ƿ���С�����
extern double DistanceBall();    //��������֮��ľ���
extern void Outcome2();      //������� 
extern void DrawBox();
extern void DrawEmptyBox();


void CountDown();                  //����ʱ���� 
void Playercontinue();             //�������� 


/*ģʽ2*/
void Gamemode2()     
{
	int i,j;
	/*������ʱ���ʼ��*/ 
	sprintf(a,"%d",0); 
	score=0;
	ContinueShooting=0;      
	DisplayClear();
	InitialBall();
	InitialPower2();
	InitialTimer();
	
	gamemode=2;               //ת��gamemode2ģʽ 
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance2[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //������ĳ�ʼ���� 
		    
	registerTimerEvent(TimerEventProcess2);
    registerMouseEvent(MouseEventProcess2);
	startTimer(2,ptime2);
	startTimer(3,1000);
	Newed2();
}

/*��ʼ��ʱ��*/                      //��ֹ�ڷ��غ󵹼�ʱ��ʾ�������� 
void InitialTimer(){
	timerecord=0;
	lastflag=1;
	sprintf(lasttime,"%c",'\0');
} 

/*��������ʼ��*/                     //��ֹ�ڷ��غ�����������͸�����ʾ�������� 
void InitialPower2(){
	judgepower2=0;    
	ipower2=0;        
    measurepower2=0;  
    startrun2=0;     
} 

/*�ٶȼ��*/ 
void VilocityMonitor2()
{
	int i,j; 
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);       //�߽���ײ��� 
		PotMonitor2(i);         //������� 
		
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
		 /*�ж�С��״̬*/
		if(Ball[i].state!=2)        
			if(Ball[i].vx==0&&Ball[i].vy==0)
			{
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		MonitorSwitch2();      //��ײ��� 
	}
}

/*�ٶȼ��ת��ײ����΢������*/
/*�ⲿ��ԭ���ǣ����������С�������뾶ʱ����¼��ʱ����ľ���tempdistance*/
/*��tempdistanceС��ԭ���ľ���Ball[i].distance[j]ʱ���������ڿ�����������ײģ�͸ı������ٶ�*/
/*��֮����ʾ������ײ���������໥Զ�룬����΢��ģ�ͼ��ٴ�ģ����*/ 
void MonitorSwitch2() 
{
	int i,j; 
	double tempdistance;
	for(i=0;i<=14;i++)
	{
		for(j=0;j<=i-1;j++)
		{
	  	  if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)   //���ľ���С�ڵ��������뾶��������Ӵ���������ײ����ײ�պý���
	   	 	{
	    		tempdistance=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));  //��ʱ�����������ľ���
				if(tempdistance<distance2[i][j])   //��������С����ʾ�����ڿ�����������ײ���� 
				{
					CollisionMonitor(i,j);
					distance2[i][j]=tempdistance;     //��¼��ǰ������� 
					distance2[j][i]=tempdistance;
				}
				else
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))     //����������������Զ�룬����΢�� 
						Adjustlen(i,j);
					  else Adjustlen(j,i);
				}
			}
			else {
					distance2[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));     //δ��ײ��ֱ�Ӽ�¼������� 
					distance2[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
			}
		for(j=i+1;j<=15;j++)   //ͬ�� 
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

/*�������*/
void PotMonitor2(int i)    //�����ľ���ĳ������Բ�ĵľ���С�ڵ��ڴ��ڰ뾶ʱ���ж�����������ٶȺ����궼���ã�״̬��Ϊ2�������� 
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

/*����������*/     //����һ�˽�����Ķ���� 
void ContinuePot(int i)      
{
	int j=0;
	if(i!=0)
	{
		j++;                     
		if(j>1) ContinueShooting=ContinueShooting+pow(2,j-2);   //���ڼ����������� 
	}
}

/*�ж��Ƿ������ڶ�*/     //����ֵ�������˶�Ϊ1��ȫ����ֹΪ0 
int Runningjudge2(){
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	if(t==1){ 
		Playercontinue();      //���Ƽ�������ĺ��� 
	}
	return 0;
}

/*��ʱ���ص�����*/ 
void TimerEventProcess2(int timerID)
{
 	if(timerID==1){  //��Ķ�ʱ�� 
 		VilocityMonitor2();   //ģ���ٶ� 
 		Newed2();    //ˢ��ÿһ֡���� 
 		startrun2=Runningjudge2();
 		if(startrun2==0){
 			judgepower2=0;
 			cancelTimer(1);     //�رռ�ʱ��1������Ӱ����������ʾ 
		}
 	}
 	if(timerID==2)  //��������ʱ�� 
 		PowerDisplay2();
 	if(timerID==3)  //�����ļ�ʱ��
	{
	 	timerecord++;    //��¼ʱ���������� 
	 	CountDown();     //����ʵʱ����ʱ��ʾ
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
/*���ص�����*/
void MouseEventProcess2(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*�����̨������*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower2==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //���������ߺ���� 
						Newed2();  //ˢ�½��� 
		
						if(Ball[0].state==0){      //����ֹͣ����������                     
		   				 	/*����������*/ 
		   					SetPenColor("White");
			    			MovePen(mox,moy);
			    			DrawLine(Ball[0].x - mox,Ball[0].y - moy);
			    			MovePen(mox+0.2,moy);
		     				DrawArc(0.2,0,360);
		
			   			 	/*�������*/
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
					judgepower2=1;  //�ж��������Ƿ������Ĳ��� 
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
					cancelTimer(3);
					cancelTimer(2);
					cancelTimer(1);  //�رռ�ʱ��������Ӱ�������ʾ 
					SaveTxt2();      //�������� 
					DisplayClear();  //����
					GameSelect();    //������Ϸģʽѡ�� 
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.92&&moy<=1.14){   //�ƶ�������2����״̬ 
					cancelMouseEvent();
					cancelTimer(3);
					cancelTimer(2);
					cancelTimer(1);  //�رռ�ʱ��������Ӱ�������ʾ 
					SaveBinary2();   //�������� 
					DisplayClear();  //����
					GameSelect();    //������Ϸģʽѡ��
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.64&&moy<=0.86){   //�ƶ���RETURN����״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1); 
					cancelTimer(3);  //�رռ�ʱ��������Ӱ�������ʾ 
					DisplayClear();
					Basic();
				}
				if(mox>=10.42&&mox<=10.92&&moy>=0.36&&moy<=.58){   //�ƶ���HELP����״̬ 
					cancelMouseEvent();
					cancelTimer(2);
					cancelTimer(1); 
					cancelTimer(3);
					DisplayClear();
					rulereturn2=1;
					SaveTxt2();                    //������ʱС��λ�� 
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

/*ˢ��̨����*/ 
void Newed2()   
{
	DisplayClear();
	Table();
	UnderDisplay2(); 
	DrawBall();
	
	MovePen(5,0.6);   //������ 
	SetPointSize(30);
	DrawTextString(a);
}

/*�²������˵���*/ 
void UnderDisplay2(){
	/*����߿�*/ 
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
	MovePen(0.3,1.2);
	SetStyle(1);
	DrawTextString("POWER SELECT");
	
	/*������ʼ��*/
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
	DrawTextString("����txt");          //�����ı��ļ� 
	MovePen(10.44,.925);
	DrawTextString("����dat");          //����������ļ� 
	MovePen(10.44,.665);
	DrawTextString(" Return");         //����������
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //�˳�
	MovePen(10.44,.385);
	DrawTextString(" Help");           //���� 
	
	SetPenColor("Black");
	SetPointSize(30);
	SetPenSize(5);
	MovePen(3.5,0.6);
	DrawTextString("�÷֣�"); 
	MovePen(7.46,0.6);
	DrawTextString("ʱ�䣺"); 
	MovePen(9,0.6);
	if(lastflag==1) DrawTextString("120");   //��ֹ��ʼ��120�޷���ʾ 
	DrawTextString(lasttime);                //��ֹ���˶���ʱ�򵹼�ʱ������ 
}
 
 /*��¼���������򣺽���ּ����������*/
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
 
 /*����ʱ������,��ʱ��Ϊ120s*/ 
 void CountDown()
 {
 	int time=120-timerecord;
 	if(time<0) time=0;
 	lastflag=0;
 	sprintf(lasttime,"%d",time);
 	/*���µ���ʱ����*/ 
 	SetPenColor("white");               
 	DrawBox(9.0,0.3,0.5,0.6);
 	SetPenColor("black");
 	MovePen(9.0,0.6);
 	SetPointSize(30);
 	DrawTextString(lasttime);
 }
 
  /*�������仯*/ 
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

/*�������ļ����¼�*/ 
void Power_Keyboard2(int key, int event){
 	switch(key)
	{
		/*���¿ո�ȷ������*/
		case VK_SPACE:{
			if(judgepower2==1)
				measurepower2=1;  //��־���Ѿ�ȷ�����ȣ���ֹ�������ٴι��� 
			/*��������ٶ�*/ 
			if(judgepower2==1&&measurepower2==1)    
			{
				double power=0;//������Ĵ�С 
				if(ipower2>=0 && ipower2<=6){
					power=3+ipower2;
				}
				else if(ipower2>=7 && ipower2<=13){
					power=16-ipower2;
				}
				startTimer(1,timegap2);       //�����������timer����������ٶ� 
				Ball[0].vx=0.015*cos1*power;
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun2=1;                  //С��ʼ�����˶� 
				cancelKeyboardEvent();
			}
			break;
		}
	}
 }

/*ʵ����������*/ 
void Playercontinue()
{
	int j=0,i=0;
 	measurepower2=0;            //��������ʼ�� 
	ipower2=0;                  //�������仯��ʼ�� 
	if(Ball[0].state==2 ){
		Ball[0].state =0;
		Ball[0].x=2.6;
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
	ScoreRecord();            //���㵱ǰ�÷� 
	Newed2();                 //���½���
}

/*���ı�ģʽ����*/           
void SaveTxt2()
{                               
	FILE *fp1;
	int i=0;
	if(rulereturn2==0){
		if((fp1=fopen("SaveGameSingle1.txt","w"))==NULL)     //�ж��ܷ�������SaveGameSingle1.txt
			exit(0);
	}	
	else if(rulereturn2==1){
		if((fp1=fopen("Game2Temp.txt","w"))==NULL)     //�ж��ܷ�������SaveGameSingle1.txt
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
	fprintf(fp1,"%d\n",score);
	/*ת���ʼ������*/
	fprintf(fp1,"%d\n",judgepower2);  //�������������� 
	fprintf(fp1,"%d\n",ipower2); 
	fprintf(fp1,"%d\n",measurepower2); 
	fprintf(fp1,"%d\n",startrun2);    //�˶����� 
	fprintf(fp1,"%lf\n",sin1);       //����˵ķ��� 
	fprintf(fp1,"%lf\n",cos1);  
	/*ת��ʱ������*/
	fprintf(fp1,"%d\n",timerecord);  
	fprintf(fp1,"%s\n",lasttime);
	fprintf(fp1,"%d\n",lastflag); 
	if(fclose(fp1)){
		exit(0); 
	}                                                                                
}
/*�Զ�����ģʽ����*/ 
void SaveBinary2(){
	FILE *fp2;
	int i=0;
	if((fp2=fopen("SaveGameSingle2.dat","wb"))==NULL){           //�ж��ܷ�������SaveGameSingle2.dat
		exit(0);
	}
	/*ת��С������*/
	for(i=0;i<=15;i++){
		fwrite(&Ball[i].state,sizeof(int),1,fp2);               
		fwrite(&Ball[i].vx,sizeof(double),1,fp2);
		fwrite(&Ball[i].vy,sizeof(double),1,fp2);
		fwrite(&Ball[i].x,sizeof(double),1,fp2);
		fwrite(&Ball[i].y,sizeof(double),1,fp2);
	}
	fwrite(&score,sizeof(int),1,fp2);
	/*ת���ʼ������*/
	fwrite(&judgepower2,sizeof(int),1,fp2);                  //������������� 
	fwrite(&ipower2,sizeof(int),1,fp2);
	fwrite(&measurepower2,sizeof(int),1,fp2);
	fwrite(&startrun2,sizeof(int),1,fp2);                    //С���Ƿ����˶� 
	fwrite(&sin1,sizeof(double),1,fp2);                      //���ӵķ��� 
	fwrite(&cos1,sizeof(double),1,fp2);
	/*ת��ʱ������*/
	lasttimesize=strlen(lasttime);
	fwrite(&timerecord,sizeof(int),1,fp2);
	fwrite(&lasttime,sizeof(lasttime),lasttimesize,fp2);
	fwrite(&lastflag,sizeof(int),1,fp2);
	if(fclose(fp2)){
		exit(0); 
	}  
}

 
 

