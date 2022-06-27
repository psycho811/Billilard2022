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

/*游戏模式选择*/
void GameSelect(){                       
	registerMouseEvent(Menu_MouseEvent);
	DisplayClear();
	SetPenColor("myblue");                
	DrawBox(0.0,0.0,11.0,7.0);
	SetPenColor("darkblue");
	SetPointSize(30);       //设置笔的大小，避免出现回转之后出现问题
	SetPenSize(3);  
	SetStyle(2);
	/*继续上次*/ 
	DrawBox(4.75,2.5,1.5,0.6);          
	SetPenColor("yellow");
	DrawEmptyBox(4.75,2.5,1.5,0.6);
	MovePen(4.8,2.7);
	SetPenColor("white");
	DrawTextString("继续上次");
	/*单人限时*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,4.5,1.5,0.6);
	SetPenColor("yellow");
	DrawEmptyBox(4.75,4.5,1.5,0.6);
	MovePen(4.8,4.7);
	SetPenColor("white");
	DrawTextString("单人限时");  
	/*返回主界面*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,1.5,1.5,0.6); 
	SetPenColor("yellow");
	DrawEmptyBox(4.75,1.5,1.5,0.6);
	MovePen(4.8,1.7);
	SetPenColor("white");
	DrawTextString(" Return"); 
	/*双人对战模式*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,5.5,1.5,0.6);  
	SetPenColor("yellow");
	DrawEmptyBox(4.75,5.5,1.5,0.6);
	MovePen(4.8,5.7);
	SetPenColor("white");
	DrawTextString("双人对战");   
	/*单人练习模式*/ 
	SetPenColor("darkblue");              
	DrawBox(4.75,3.5,1.5,0.6);  
	SetPenColor("yellow");
	DrawEmptyBox(4.75,3.5,1.5,0.6);
	MovePen(4.8,3.7);
	SetPenColor("white");
	DrawTextString("单人练习");   
}
/*模式选择页面交互*/ 
void Menu_MouseEvent (int x, int y, int button, int event)
{
	double mox,moy;
	mox = ScaleXInches(x);
	moy = ScaleYInches(y);
	switch(event){
		/*移动到按钮状态*/ 
		case MOUSEMOVE:{
				SetPenSize(3);
				if(mox>=4.75 &&mox<=6.25&&moy>=5.5&&moy<=6.1){           //双人对战按钮 
					SetPenColor("black");
					DrawEmptyBox(4.75,5.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,5.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=4.5&&moy<=5.1){            //单人限时按钮 
					SetPenColor("black");
					DrawEmptyBox(4.75,4.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,4.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=2.5&&moy<=3.1){            //继续上次按钮 
					SetPenColor("black");
					DrawEmptyBox(4.75,2.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,2.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=1.5&&moy<=2.1){            //return按钮
					SetPenColor("black");
					DrawEmptyBox(4.75,1.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,1.5,1.5,0.6);
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){            //单人练习按钮
					SetPenColor("black");
					DrawEmptyBox(4.75,3.5,1.5,0.6);
				}
				else{
					SetPenColor("yellow");
					DrawEmptyBox(4.75,3.5,1.5,0.6);
				}
			break;
		}
		/*按下按钮交互*/  
		case BUTTON_UP:{
			if(button==LEFT_BUTTON){
				if(mox>=4.75 &&mox<=6.25&&moy>=5.5&&moy<=6.1){          //双人对战
					cancelMouseEvent();
					DisplayClear();
					Gamemode1();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=4.5&&moy<=5.1){          //单人限时 
					cancelMouseEvent;
					ProGamemode2();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=3.5&&moy<=4.1){          //单人练习 
					cancelMouseEvent;
					Gamemode3();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=2.5&&moy<=3.1){          //继续上次 
					cancelMouseEvent();
					ContinueLastBasic();
				}
				if(mox>=4.75 &&mox<=6.25&&moy>=1.5&&moy<=2.1){         //回到主界面 
					cancelMouseEvent();
					Basic();                                              
				}
			}
			break;
		}	
	}	
}
