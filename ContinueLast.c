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
extern ball Ball[15];
extern gamemode;         //��¼��Ϸģʽ 
extern double u;         //Ħ�������ļ��ٶȣ����٣� 
extern double timegap;   //ʱ���� 
extern int ptime;        //�������仯��ʱ�� 
extern int judgepower;   //�ж��������Ƿ��� 
extern int ipower;       //��¼���ȴ�С 
extern int measurepower; //�ж��Ƿ��°������������Ƿ�ֹͣ�˶� 
extern int startrun;     //�ж�С���Ƿ����˶� 
extern int player;       //Ԥ�����Ϊ1     
extern double cos1,sin1;   //ȷ��С���˶��ķ��� 
extern int judgepower2;   //�ж�ģʽ2�������Ƿ���
extern int ipower2;       //��¼ģʽ2���ȴ�С
extern int measurepower2; //�ж�ģʽ2�Ƿ��°������������Ƿ�ֹͣ�˶�
extern int timerecord;    //��¼ʱ��
extern char lasttime;     //����ʱ���ַ�����ʽ 
extern int lastflag;      //�жϼ�ʱ���Ƿ��ǵ�һ����ʾ  
extern int score;         //��ҷ��� 
extern int startrun2;     //�ж�ģʽ2С���Ƿ����˶� 
extern int judgepower3;   //�ж�ģʽ3�������Ƿ���
extern int ipower3;       //��¼ģʽ3���ȴ�С
extern int measurepower3; //�ж�ģʽ3�Ƿ��°������������Ƿ�ֹͣ�˶�
extern int startrun3;     //�ж�ģʽ3С���Ƿ����˶�
extern int rulereturn1;   //�ж�ģʽ1�Ǵ��ĸ����淵����Ϸ 
extern int rulereturn2;   //�ж�ģʽ2�Ǵ��ĸ����淵����Ϸ 
extern int lasttimesize;  //��¼ģʽ2����ʱ�䳤�� 

void ContinueLastBasic();    //�����ϴεĻ������� 
void ContinueTxt1();    //����˫��ģʽtxt 
void ContinueDat1();    //����˫��ģʽdat
void ContinueTxt2();    //��������ģʽtxt
void ContinueDat2();    //��������ģʽdat
void ContinueTxt3();    //������ϰģʽ 
void Continue_MouseEvent (int x, int y, int button, int event);        //�����ϴν��������Ӧ���� 

extern Gamemode1(),Newed();
extern Gamemode2(),Newed2();
extern Gamemode3(),Newed3();

/*����������*/ 
void ContinueLastBasic(){
	DisplayClear();
	registerMouseEvent(Continue_MouseEvent); 
	SetPenSize(3); 
	SetPointSize(24);     
	SetPenColor("myblue");                
	DrawBox(0.0,0.0,11.0,7.0);
	
	
	/*����������ʱģʽtxt*/
	SetPenColor("darkblue");
	DrawBox(4.75,3.3,2.3,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,3.3,2.3,0.6);
	MovePen(4.8,3.5);
	SetPenColor("white");
	SetFont("Times");
	SetStyle(2);
	DrawTextString("����������ʱtxt");
	
	SetPenColor("darkblue");
	DrawBox(4.75,2.3,2.3,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,2.3,2.3,0.6);
	MovePen(4.8,2.5);
	SetPenColor("white");
	DrawTextString("����������ʱdat");
	
	/*����˫�˶�սtxt*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,5.3,2.3,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,5.3,2.3,0.6);
	MovePen(4.8,5.5);
	SetPenColor("white");
	DrawTextString("����˫�˶�սtxt");
	/*����˫�˶�սdat*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,4.3,2.3,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,4.3,2.3,0.6);
	MovePen(4.8,4.5);
	SetPenColor("white");
	DrawTextString("����˫�˶�սdat"); 
	/*����ģʽѡ��*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,1.3,2.3,0.6); 
	SetPenColor("yellow");
	DrawEmptyBox(4.75,1.3,2.3,0.6);
	MovePen(4.8,1.5);
	SetPenColor("white");
	DrawTextString("       Return"); 	
}

/*�����ϴν��������¼�*/
void Continue_MouseEvent(int x, int y, int button, int event){
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*�ƶ�����ť״̬*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=7.05&&moy>=5.3&&moy<=5.9){           //����˫�˶�սtxt 
					SetPenColor("black");
					DrawEmptyBox(4.75,5.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,5.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=4.3&&moy<=4.9){           //����˫�˶�սdat
					SetPenColor("black");
					DrawEmptyBox(4.75,4.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,4.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=3.3&&moy<=3.9){            //����������ʱtxt
					SetPenColor("black");
					DrawEmptyBox(4.75,3.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,3.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=2.3&&moy<=2.9){            //����������ʱdat
					SetPenColor("black");
					DrawEmptyBox(4.75,2.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,2.3,2.3,0.6);
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=1.3&&moy<=1.9){            //return ��ť 
					SetPenColor("black");
					DrawEmptyBox(4.75,1.3,2.3,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,1.3,2.3,0.6);
				}
			break;
		}
		/*���°�ť����*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=7.05&&moy>=5.3&&moy<=5.9){          //˫�˶�սtxt
					cancelMouseEvent();
					DisplayClear();
					ContinueTxt1();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=4.3&&moy<=4.9){          //˫�˶�սdat
					cancelMouseEvent();
					DisplayClear();
					ContinueDat1();
				}
				
				if(mox>=4.75 &&mox<=7.05&&moy>=3.3&&moy<=3.9){          //������ʱtxt 
					cancelMouseEvent();
					DisplayClear();
					ContinueTxt2();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=2.3&&moy<=2.9){          //������ʱdat 
					cancelMouseEvent();
					DisplayClear();
					ContinueDat2();
				}
				if(mox>=4.75 &&mox<=7.05&&moy>=1.3&&moy<=1.9){          //������һ�� 
					cancelMouseEvent();
					DisplayClear();
					GameSelect();
				}
			}
			break;
		}	
	}
}

/*��ȡ˫��ģʽ�ı��ļ�*/ 
void ContinueTxt1(){
	FILE *fp1;
	int i=0;                                                     //i��Ϊѭ��������t��Ϊ���
	if(rulereturn1==1){                                          //�ж��Ƿ��Ǵ�help���淵�� 
		if((fp1=fopen("Game1Temp.txt","r"))==NULL)               //�ж��Ƿ����������ļ� 
			exit(0);
	}
	else if(rulereturn1==0){
		if((fp1=fopen("SaveGameDouble1.txt","r"))==NULL)         //�ж��Ƿ����������ļ�     
			exit(0);
	} 	
	Gamemode1();
	/*ת��С������*/ 
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );     //С��״̬
		fscanf(fp1,"%lf",&Ball[i].vx);        //С���ٶ�
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);         //С��λ�� 
		fscanf(fp1,"%lf",&Ball[i].y);  
	}
	/*ת����ʼ������*/
	fscanf(fp1,"%d",&player);
	fscanf(fp1,"%d",&judgepower);             //����������
	fscanf(fp1,"%d",&ipower);
	fscanf(fp1,"%d",&measurepower);
	fscanf(fp1,"%d",&startrun);               //�˶����� 
	fscanf(fp1,"%lf",&cos1);                  //����˵ķ��� 
	fscanf(fp1,"%lf",&cos1);
	/*��ֹ����С�����ٶȣ�����������״��*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	if(fclose(fp1)){
		exit(0); 
	}
	Newed();  //����ԭ������ 
} 

/*��ȡ˫�˶�ս�������ļ�*/ 
void ContinueDat1()
{
	Gamemode1();
	int i=0,t=0;
	FILE *fp2;
	if((fp2=fopen("SaveGameDouble2.dat","rb"))==NULL){           //�ж��ܷ�������"SaveGame2.dat"
		exit(0);
	}
	for(i=0;i<=15;i++){
		fread(&Ball[i].state,sizeof(int),1,fp2);
		fread(&Ball[i].vx,sizeof(double),1,fp2);
		fread(&Ball[i].vy,sizeof(double),1,fp2);
		fread(&Ball[i].x,sizeof(double),1,fp2);
		fread(&Ball[i].y,sizeof(double),1,fp2);
	}
	fread(&player,sizeof(int),1,fp2);                      //������� 
	fread(&judgepower,sizeof(int),1,fp2);                  //������������� 
	fread(&ipower,sizeof(int),1,fp2);
	fread(&measurepower,sizeof(int),1,fp2);
	fread(&startrun,sizeof(int),1,fp2);
	fread(&sin1,sizeof(double),1,fp2);
	fread(&cos1,sizeof(double),1,fp2);
	/*�ر��ļ�*/
	if(fclose(fp2)){
		exit(0);
	}
	/*��ֹ����С�����ٶȣ�����������״��*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	Newed();  //����ģʽ2���� 
}

/*��ȡ���˶�սtxt�ļ�*/ 
void ContinueTxt2()
{                          
	FILE *fp1;
	int i=0;
	if(rulereturn2==1){                                             //�ж��Ƿ��Ǵ�help���淵��                                         
		if((fp1=fopen("Game2Temp.txt","r"))==NULL)                 //�ж��ܷ�������
			exit(0);
	}
	else if(rulereturn2==0){
		if((fp1=fopen("SaveGameSingle1.txt","r"))==NULL)           //�ж��ܷ�������
			exit(0);
	}
	Gamemode2();
	/*ת��С������*/
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );       //����С��״̬ 
		fscanf(fp1,"%lf",&Ball[i].vx);          //����С���ٶ�
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);           //����С��λ�� 
		fscanf(fp1,"%lf",&Ball[i].y);
	}
	fscanf(fp1,"%d",&score);
	/*ת���ʼ������*/
	fscanf(fp1,"%d",&judgepower2);  //�������������� 
	fscanf(fp1,"%d",&ipower2); 
	fscanf(fp1,"%d",&measurepower2); 
	fscanf(fp1,"%d",&startrun2);    //�˶����� 
	fscanf(fp1,"%lf",&sin1);       //����˵ķ��� 
	fscanf(fp1,"%lf",&cos1);  
	/*ת��ʱ������*/
	fscanf(fp1,"%d",&timerecord);  
	fscanf(fp1,"%s",&lasttime);
	fscanf(fp1,"%d",&lastflag); 
	if(fclose(fp1))
		exit(0); 
	/*��ֹ����С�����ٶȣ�����������״��*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	Newed2();  //����ģʽ2����                                                                               
}

/*��ȡ���˶�սdat�ļ�*/
void ContinueDat2(){
	FILE *fp2;
	int i=0;
	if((fp2=fopen("SaveGameSingle2.dat","rb"))==NULL){           //�ж��ܷ�������SaveGameSingle2.dat
		exit(0);
	}
	Gamemode2();
	/*ת��С������*/
	for(i=0;i<=15;i++){
		fread(&Ball[i].state,sizeof(int),1,fp2);               
		fread(&Ball[i].vx,sizeof(double),1,fp2);
		fread(&Ball[i].vy,sizeof(double),1,fp2);
		fread(&Ball[i].x,sizeof(double),1,fp2);
		fread(&Ball[i].y,sizeof(double),1,fp2);
	}
	fread(&score,sizeof(int),1,fp2);
	/*ת���ʼ������*/
	fread(&judgepower2,sizeof(int),1,fp2);                  //������������� 
	fread(&ipower2,sizeof(int),1,fp2);
	fread(&measurepower2,sizeof(int),1,fp2);
	fread(&startrun2,sizeof(int),1,fp2);                    //С���Ƿ����˶� 
	fread(&sin1,sizeof(double),1,fp2);                     //���ӵķ��� 
	fread(&cos1,sizeof(double),1,fp2);
	/*ת��ʱ������*/
	fread(&timerecord,sizeof(int),1,fp2);
	fread(&lasttime,sizeof(lasttime),lasttimesize,fp2);
	fread(&lastflag,sizeof(int),1,fp2);
	/*��ֹ����С�����ٶȣ�����������״��*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	}
	if(fclose(fp2)){
		exit(0); 
	} 
	Newed2(); //����ģʽ2���� 
}

/*����ģʽ3�е�*/
void ContinueTxt3(){
	Gamemode3();                            
	FILE *fp1;
	int i=0;
	if((fp1=fopen("Game3Temp.txt","r"))==NULL)
		exit(0);
	/*ת��С������*/
	for(i=0;i<=15;i++){
		fscanf(fp1,"%d",&Ball[i].state );       //����С��״̬ 
		fscanf(fp1,"%lf",&Ball[i].vx);          //����С���ٶ�
		fscanf(fp1,"%lf",&Ball[i].vy);
		fscanf(fp1,"%lf",&Ball[i].x);           //����С��λ�� 
		fscanf(fp1,"%lf",&Ball[i].y);
	}
	/*ת���ʼ������*/
	fscanf(fp1,"%d",&judgepower3);  //�������������� 
	fscanf(fp1,"%d",&ipower3); 
	fscanf(fp1,"%d",&measurepower3); 
	fscanf(fp1,"%d",&startrun3);    //�˶����� 
	fscanf(fp1,"%lf",&sin1);       //����˵ķ��� 
	fscanf(fp1,"%lf",&cos1);  
	if(fclose(fp1)){
		exit(0); 
	}
	/*��ֹ����С�����ٶȣ�����������״��*/ 
	for(i=0;i<=15;i++){   
		if(Ball[i].vx !=0 || Ball[i].vy !=0)
			startTimer(1,timegap);
	} 
	Newed3(); //����ģʽ3����                 
} 



