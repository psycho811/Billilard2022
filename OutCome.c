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

char mode1date[80] = "\0";                 //��¼ģʽ1����ʱ�� 
char mode2date[80] = "\0";                 //��¼ģʽ2����ʱ�� 
char WinnerName[80] = "Winner Name";       //��¼ģʽ1Ӯ������ 
char LoserName[80] = "Loser Name";         //��¼ģʽ1������� 
char mode2name[80] ="Name";                //��¼ģʽ2������� 
double winwidth=11.0, winheight=7.0;      // ���ڳߴ�                          

void Outcome1();                                             //ģʽ1����ֹ����
void Outcome2();                                             //ģʽ2����ֹ���� 
void DrawBackButton();                                       //���ذ�ť�Ļ��� 
void BackGame(int x,int y,int button,int event);             //�ص���������İ�ť
void OutDisplay();                                           //��������ˢ�� 
void drawEditText1();                                        //ģʽ1���ս�����ı��༭ 
void drawEditText2();                                        //ģʽ2���ս�����ı��༭ 
void Result1Input();                                               //ģʽ1�����ս��
void Score2Input();                                                //ģʽ1�����ս�� 
void Gettime();                                                    //��ȡ��Ϸ����ʱ�� 
 
/*ʹ���ı�����Ҫ����*/ 
void OutCharEventProcess(char ch);                           //�ַ����� 
void OutKeyboardEventProcess(int key, int event);                    
void OutMouseEventProcess(int x, int y, int button, int event); 
       
extern int winner;                   //����winner
extern int gamemode;                 //����gamemode 
extern int score;                    //�������
extern void Basic();                 //��ʼ����
 
/*OUTCOME�����ַ��¼���Ӧ����*/ 
void OutCharEventProcess(char ch){
	uiGetChar(ch);    // GUI�ַ�����
	OutDisplay();     //ˢ����ʾ
}

/*OUTCOME��������¼���Ӧ����*/
void OutKeyboardEventProcess(int key, int event){
	uiGetKeyboard(key,event); // GUI��ȡ����
	OutDisplay();             // ˢ����ʾ
}

/*ˢ�½���*/ 
void OutDisplay(){
	DisplayClear(); 	// ����
	if(gamemode==1) {   // ���ƽ���
		Outcome1(winner);	
	}   
	else if(gamemode==2) {
		Outcome2(); 
	}   
}

/*˫�˶�ս�Ľ����¼*/
void Result1Input(){
	FILE *fp;
	int i=0;
	if((fp=fopen("FinalResult1.txt","a"))==NULL){           //�ж��ܷ�������FinalResult1.txt 
		exit(0);
	}
	Gettime();                                               //��ȡ��Ϸ����ʱ�� 
	fprintf(fp,"%s\n",mode1date);                            //����ʱ�� 
	fprintf(fp,"%s\n",WinnerName);                           //Ӯ�ҵ�����
	fprintf(fp,"%s\n",LoserName);                            //��ҵ�����
	if(fclose(fp)){
		exit(0); 
	}  
}

/*��ȡ��Ϸ����ʱ��*/ 
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

/*���˶�ս�ķ�����¼*/ 
void Score2Input(){
	FILE *fp;
	if((fp=fopen("FinalScore2.txt","a"))==NULL){           //��FinalScore2.txt
		exit(0);
	}
	Gettime();
	fprintf(fp,"%s\n",mode2date);   //��Ϸ��ʱ�� 
	fprintf(fp,"%s\n",mode2name);    //��ҵ�����
	fprintf(fp,"%d\n",score);       //��ҵķ��� 
	if(fclose(fp)){
		exit(0); 
	}
}

/*ģʽ2���ս�����ı��༭*/ 
void drawEditText2(){
	SetPointSize(20);
	double fH = GetFontHeight();
	double h = fH*2;         // �ؼ��߶�
	double w = winwidth/4.3; // �ؼ����
	double x = winwidth*3/6.7;
	double y = winheight/1.7-h;
	SetPenColor("graygreen");
	setTextBoxColors("white", "black", "listyellow", "blue", 1);
	drawLabel(x-fH/1.5-TextStringWidth("WinnName     "), (y-=h*2.7)+fH*0.7, "Type the Name:");
	textbox(GenUIID(0), x+0.3, y, w, h, mode2name, sizeof(mode2name)); 
}

/*ģʽ1���ս�����ı��༭*/ 
void drawEditText1(){
	SetPointSize(20);
	double fH = GetFontHeight();
	double h = fH*2; // �ؼ��߶�
	double w = winwidth/4.3; // �ؼ����
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

/*�����ؽ���İ�ť*/ 
void DrawBackButton(){
	SetPenSize(4);
	SetPointSize(17);
	/*�ص���ʼ����*/ 
	SetPenColor("myorange");
	DrawBox(4.3,1.7,0.9,0.35);
	MovePen(4.42,1.8);
	SetPenColor("graygreen");
	DrawTextString("RETURN");                      
	SetPenColor("yellow");
	DrawEmptyBox(4.3,1.7,0.9,0.35);

	/*�˳���ť*/  
	SetPenColor("myorange");
	DrawBox(5.8,1.7,0.9,0.35);
	MovePen(5.92,1.8);
	SetPenColor("graygreen");
	DrawTextString("   EXIT");                                
	SetPenColor("yellow");
	DrawEmptyBox(5.8,1.7,0.9,0.35);
}

/*ģʽ2��������*/ 
void Outcome2(){
	char score1[100]="\0";                          //��¼�ɼ���λ�� 
	registerCharEvent(OutCharEventProcess);         // �ַ��¼� 
	registerKeyboardEvent(OutKeyboardEventProcess); // �����¼� 
	registerMouseEvent(BackGame);                   // ���س�ʼ���� 
	/*����*/
	DisplayClear();
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);
	SetStyle(2);
	/*��ʾ�ɼ�*/ 
	MovePen(1.8,4.8);
	SetPenColor("myyellow");
	SetPointSize(90);
	sprintf(score1,"%d",score);
	DrawTextString("Your Final Score is ");
	MovePen(5.3,3.45);
	SetPointSize(120);
	SetPenColor("myred");
	DrawTextString(score1);   
	/*�����ذ�ť*/ 
	DrawBackButton();	
	/*ģʽ2���ı�����*/ 
	drawEditText2();
}

/*ģʽ1�����������*/
void Outcome1(int winnernum)
{
	registerCharEvent(OutCharEventProcess);        // �ַ��¼� 
	registerKeyboardEvent(OutKeyboardEventProcess);// �����¼� 
	registerMouseEvent(BackGame);                  // ���س�ʼ���� 
	/*����*/
	DisplayClear();
	SetPenColor("myblue1");              
	DrawBox(0.0,0.0,11.0,7.0);
	SetStyle(2);
	if(winnernum==1){                             //������һ��ʤ 
		MovePen(3.0,4.5);
		SetPenColor("myyellow");
		SetPointSize(100);
		DrawTextString("Player1 Win !");        		 
	}
	else if (winnernum==2){                      //�����Ҷ���ʤ 
		MovePen(3.0,4.5);
		SetPenColor("myyellow");
		SetPointSize(100);
		DrawTextString("Player2 Win !"); 
	}
	/*�����ذ�ť*/ 
	DrawBackButton();
	/*���ı���*/	
	drawEditText1();                            
}

/*�ص���������İ�ť����*/ 
void BackGame(int x,int y,int button,int event){
	uiGetMouse(x,y,button,event); //GUI��ȡ���
	OutDisplay();                 // ˢ����ʾ	
	double mox,moy;
	mox = ScaleXInches(x), moy = ScaleYInches(y);
	switch(event){
		case MOUSEMOVE:{
			/*�ƶ������ذ�ť*/ 
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
			/*�ƶ����˳���ť*/ 
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
					if(gamemode==1) Result1Input();   //˫�˶�ս�����¼ 
					if(gamemode==2) Score2Input();    //����ģʽ�����¼ 
					Basic();
				}
				if(mox>=5.8&&mox<=6.7&&moy>=1.7&&moy<=2.05){
					cancelMouseEvent();
					cancelKeyboardEvent();
					cancelCharEvent();
					if(gamemode==1) Result1Input();   //˫��ģʽ�����¼ 
					if(gamemode==2) Score2Input();    //����ģʽ�����¼ 
					ExitGraphics(); 
				}
			}
			break;
		}
	}
}






