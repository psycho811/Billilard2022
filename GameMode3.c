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

/*ģʽ3�е�����*/ 
extern int gamemode;      
double distance3[20][20];
double u3=9;         //Ħ�������ļ��ٶȣ����٣� 
double timegap3=0.006;//ʱ���� 
int ptime3=60;      //�������仯��ʱ�� 
int judgepower3=0;   //�ж��������Ƿ��� 
int ipower3=0;       //��¼���ȴ�С 
int measurepower3=0; //�ж��Ƿ��°������������Ƿ�ֹͣ�˶� 
int startrun3=0;     //�ж�С���Ƿ����˶� 
double cos1,sin1;   //ȷ��С���˶��ķ��� 

ball Ball[16];
 
void SaveTxt3();    //��ʱ����mode3�е����� 
void InitialPower3();              //��ʼ�������� 

void UnderDisplay3();              //����Ĳ˵��� 
void MouseEventProcess3(int x,int y,int button,int event);
void TimerEventProcess3(int timerID);         //�ж��ٶȣ�����������λ�õ�timer 
void PowerDisplay3();                        //��������ɫ�ı�
void Power_Keyboard3(int key, int event);     //�������ȴ�С�ļ����¼� 
void Gamemode3();            //��Ϸģʽ3 
void Newed3();               //ˢ�½��� 
void MonitorSwitch3();       //�ٶȼ��ת��ײ����΢������
void VilocityMonitor3();     //�ٶȼ�� 
void PotMonitor3(int i);     //������� 
void Playercontinue3();      //�������� 

extern void BorderMonitor(int i);  //�߽���ײ 
extern void CollisionMonitor(int i,int j);//��ײ���
extern void DrawBall();      //���� 
extern void Adjustlen(int i,int j);//С���ص�΢��
extern void Table();               //���� 
extern void InitialBall();         //��ʼ�����λ�� 
extern void Playerchange();        //������� 
extern void InitialPower();        //��ʼ��������
extern void BallNumberRecord();    //����¼��������� 
extern int Runningjudge();        //�ж��Ƿ���С�����
extern double DistanceBall();    //��������֮��ľ��� 
extern void DrawBox();
extern void DrawEmptyBox();

/*ģʽ3*/
void Gamemode3()     
{
	InitialBall();  //�������ʼ�� 
	InitialPower3(); //��������ʼ�� 
	gamemode=3;     //ת��Ϊ��Ϸģʽ3 
	int i,j;
	for(i=0;i<=15;i++)
		for(j=0;j<=15;j++)
		    distance3[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));    //������ĳ�ʼ���� 
		    
	registerTimerEvent(TimerEventProcess3);
    registerMouseEvent(MouseEventProcess3); 
	startTimer(2,ptime3);
	startTimer(3,1000);
	Newed3();
}

/*�ٶȼ��*/ 
void VilocityMonitor3()
{
	int i,j;
	double sin,cos,x,y,vx,vy,v,s,vn;
	
	for(i=0;i<=15;i++)
	{
		if(Ball[i].state!=2&&Ball[i].vx==0&&Ball[i].vy==0) Ball[i].state=0;
		if(Ball[i].state!=2&&(Ball[i].vx!=0&&Ball[i].vy!=0)) Ball[i].state=1;
		BorderMonitor(i);   //�߽���ײ��� 
		PotMonitor3(i);     //������� 
		
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
		
		if(Ball[i].state!=2)        //�ж�С��״̬ 
			if(Ball[i].vx==0&&Ball[i].vy==0)
			{
				Ball[i].state=0;
			}
			else Ball[i].state=1;
		
		MonitorSwitch3();   //����Ƿ�����ײ 
	}
}

/*�ٶȼ��ת��ײ����΢������*/
/*�ⲿ��ԭ���ǣ����������С�������뾶ʱ����¼��ʱ����ľ���tempdistance*/
/*��tempdistanceС��ԭ���ľ���Ball[i].distance[j]ʱ���������ڿ�����������ײģ�͸ı������ٶ�*/
/*��֮����ʾ������ײ���������໥Զ�룬����΢��ģ�ͼ��ٴ�ģ����*/ 
void MonitorSwitch3() 
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
				if(tempdistance<distance3[i][j])      //�����ڿ�����������ײģ�͸ı������ٶ�
				{
					CollisionMonitor(i,j);  //��ײ 
					distance3[i][j]=tempdistance;   //����ʱ���������������������distance�� 
					distance3[j][i]=tempdistance;
				}
				else                     //������ײ���������໥Զ�룬����΢��ģ�ͼ��ٴ�ģ����
				{
					if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2)))   //΢��������ֻ��Ball[i]����΢����ѡ���ٶ�ƫС����һ������ΪBall[i]
						Adjustlen(i,j);  //΢�� 
					  else Adjustlen(j,i);
				}
			}
			else
			 {
					distance3[i][j]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2));     //���ľ�����������뾶����������ײ��ֱ�ӽ���ʱ���������������������distance��
					distance3[j][i]=sqrt(pow(Ball[j].y-Ball[i].y,2)+pow(Ball[j].x-Ball[i].x,2)); 
				}
			}
		for(j=i+1;j<=15;j++)        //ͬ�ϣ������ֺ���������i�������������򣨳��䱾������Ƿ���ײ 
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

/*�������*/
void PotMonitor3(int i)   //�����ľ���ĳ������Բ�ĵľ���С�ڵ��ڴ��ڰ뾶ʱ���ж�����������ٶȺ����궼���ã�״̬��Ϊ2��������
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

/*�ж��Ƿ������ڶ�*/ 
int Runningjudge3(){     //1��ʾ�����ڶ���0��ʾȫ����ֹ 
	int i,t=1;
	for(i=0;i<=15;i++){
		if(Ball[i].state ==1 ){
			t=0;
			return 1;
		}
	}
	Playercontinue3();   //�������� 
	return 0;
}

/*��ʱ���ص�����*/ 
void TimerEventProcess3(int timerID)
{
 	int i,j;
	 if(timerID==1){  //��Ķ�ʱ�� 
 		VilocityMonitor3();
 		Newed3();
 		startrun3=Runningjudge3();
 		if(startrun3==0){
 			judgepower3=0;
 			for(i=1;i<=15;i++)
 			{
 				if(Ball[i].state==2)     //���������ˣ�������������ã����س�ʼλ�� 
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
					 for(j=0;j<=i-1;j++)    //������ʱ���ܻ�ͱ�����ص�������΢�� 
					{
	  	  				if(sqrt(pow(Ball[i].x-Ball[j].x,2)+pow(Ball[i].y-Ball[j].y,2))<=0.4)
					 	if((pow(Ball[i].vx,2)+pow(Ball[i].vy,2))<(pow(Ball[j].vx,2)+pow(Ball[j].vy,2))) 
							Adjustlen(i,j);
					  	else Adjustlen(j,i);
					}
			    }
			}
			Newed3();
 			cancelTimer(1);     //�رռ�ʱ��1������Ӱ����������ʾ 
		}
 	}
 	if(timerID==2)  //��������ʱ�� 
 		PowerDisplay3();
}

void MouseEventProcess3(int x,int y,int button,int event)   
{
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	/*�����̨������*/ 
	if(mox>=0&&mox<=11&&moy>=1.5&&moy<=7){
		switch(event){
			case MOUSEMOVE:{
				if(judgepower3==0){
					if(mox>=0.6&&mox<=10.4&&moy>=2.1&&moy<=6.4){    //���������ߺ���� 
						Newed3();  //ˢ�½��� 
		
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
					judgepower3=1;
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
					SaveTxt3();
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
void Newed3()   
{
	int i,j;
	DisplayClear();
	Table();
	UnderDisplay3();
	for(i=1;i<=15;i++){
 		if(Ball[i].state==2)     //���������ˣ�������������ã����س�ʼλ�� 
 		{
			for(j=0;j<=i-1;j++)    //������ʱ���ܻ�ͱ�����ص�������΢�� 
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

/*�²������˵���*/ 
void UnderDisplay3(){
	SetPenColor("lightorange");
	SetPenSize(7);
	DrawEmptyBox(0.03,0.03,10.94,1.45);
	MovePen(2.5,1.45);
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
	DrawTextString("POWER SELECT");
	
	/*������ʼ��*/
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
	DrawTextString(" Return");         //����������
	MovePen(10.44,.105);
	DrawTextString(" Exit");           //�˳�
	MovePen(10.44,.385);
	DrawTextString(" Help");           //���� 
 }
 
  /*�������仯*/ 
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
		/*���¿ո�ȷ������*/
		case VK_SPACE:{
			if(judgepower3==1)
				measurepower3=1;  //��־���Ѿ�ȷ�����ȣ���ֹ�������ٴι��� 
			/*��������ٶ�*/ 
			if(judgepower3==1&&measurepower3==1)    
			{
				double power=0;//������Ĵ�С 
				if(ipower3>=0 && ipower3<=6){
					power=3+ipower3;
				}
				else if(ipower3>=7 && ipower3<=13){
					power=16-ipower3;
				}
				startTimer(1,timegap3);       //�����������timer����������ٶ� 
				Ball[0].vx=0.015*cos1*power;
				Ball[0].vy=0.015*sin1*power;
				Ball[0].state=1; 
				startrun3=1;                  //С��ʼ�����˶� 
				cancelKeyboardEvent();
			}
			break;
		}
	}
 }
 
 /*��������ʼ��*/                     //��ֹ�ڷ��غ�����������͸�����ʾ�������� 
void InitialPower3(){
    judgepower3=0;    
    ipower3=0;        
    measurepower3=0;  
    startrun3=0;     
} 

/*����gamemode3�е���ʱ����*/           
void SaveTxt3(){                                
	FILE *fp1;
    int i=0;
    if((fp1=fopen("Game3Temp.txt","w"))==NULL){  //�ж��ܷ�������Game3Temp.txt
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
 	fprintf(fp1,"%d\n",judgepower3);  //�������������� 
 	fprintf(fp1,"%d\n",ipower3); 
 	fprintf(fp1,"%d\n",measurepower3); 
 	fprintf(fp1,"%d\n",startrun3);    //�˶����� 
 	fprintf(fp1,"%lf\n",sin1);       //����˵ķ��� 
 	fprintf(fp1,"%lf\n",cos1);  
 	if(fclose(fp1)){
   		exit(0); 
 	}                                                                                
}

/*ģʽ3����*/
void Playercontinue3()
{
 	measurepower3=0;            //��������ʼ�� 
	ipower3=0;                  //�������仯��ʼ�� 
	if(Ball[0].state==2 ){
		Ball[0].state =0;
		Ball[0].x=2.6;
		Ball[0].y=4.25;
	}
	Newed3();                 //���½��� 
}

