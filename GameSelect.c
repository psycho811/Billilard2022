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

extern void DrawBox();
extern void Clean();
void Menu_MouseEvent (int x, int y, int button, int event);

/*��Ϸģʽѡ��*/
void GameSelect(){                       
	registerMouseEvent(Menu_MouseEvent);
	DisplayClear();
	SetPenColor("myblue");                
	DrawBox(0.0,0.0,11.0,7.0);
	SetPenColor("darkblue");
	SetPointSize(30);       //���ñʵĴ�С��������ֻ�ת֮���������
	SetPenSize(3);  
	SetStyle(2);
	/*�����ϴ�*/ 
	DrawBox(4.75,2.5,1.5,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,2.5,1.5,0.6);
	MovePen(4.8,2.7);
	SetPenColor("white");
	DrawTextString("�����ϴ�");
	/*������ʱ*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,4.5,1.5,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,4.5,1.5,0.6);
	MovePen(4.8,4.7);
	SetPenColor("white");
	DrawTextString("������ʱ");  
	/*����������*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,1.5,1.5,0.6); 
	SetPenColor("yellow");
	DrawEmptyBox(4.75,1.5,1.5,0.6);
	MovePen(4.8,1.7);
	SetPenColor("white");
	DrawTextString(" Return"); 
	/*˫�˶�սģʽ*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,5.5,1.5,0.6);  
	SetPenColor("yellow");
	DrawEmptyBox(4.75,5.5,1.5,0.6);
	MovePen(4.8,5.7);
	SetPenColor("white");
	DrawTextString("˫�˶�ս");   
	/*������ϰģʽ*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,3.5,1.5,0.6);  
	SetPenColor("yellow");
	DrawEmptyBox(4.75,3.5,1.5,0.6);
	MovePen(4.8,3.7);
	SetPenColor("white");
	DrawTextString("������ϰ");   
}
/*ģʽѡ��ҳ�潻��*/ 
void Menu_MouseEvent (int x, int y, int button, int event)
{
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*�ƶ�����ť״̬*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=5.5&&moy<=6.1){           //˫�˶�ս��ť 
					SetPenColor("black");
					DrawEmptyBox(4.75,5.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,5.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=4.5&&moy<=5.1){            //������ʱ��ť 
					SetPenColor("black");
					DrawEmptyBox(4.75,4.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,4.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=2.5&&moy<=3.1){            //�����ϴΰ�ť 
					SetPenColor("black");
					DrawEmptyBox(4.75,2.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,2.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=1.5&&moy<=2.1){            //return��ť
					SetPenColor("black");
					DrawEmptyBox(4.75,1.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,1.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){            //������ϰ��ť
					SetPenColor("black");
					DrawEmptyBox(4.75,3.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,3.5,1.5,0.6);
				}
			break;
		}
		/*���°�ť����*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=5.5&&moy<=6.1){          //˫�˶�ս
					cancelMouseEvent();
					DisplayClear();
					Gamemode1();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=4.5&&moy<=5.1){          //������ʱ 
					cancelMouseEvent;
					ProGamemode2();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){          //������ϰ 
					cancelMouseEvent;
					Gamemode3();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=2.5&&moy<=3.1){          //�����ϴ� 
					cancelMouseEvent();
					ContinueLastBasic();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=1.5&&moy<=2.1){         //�ص������� 
					cancelMouseEvent();
					Basic();                                              
				}
			}
			break;
		}	
	}	
}
