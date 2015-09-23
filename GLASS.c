/**
*  The glass UI framework is meant for some handy ui components
*  such as Warning Dialog , Input String dialog , Title bar etc.
*  To use this in your C++ program simply copy all the code in
*  this file( except the main function ) and call the init()
*  function  at the start of your main() function.
*  For all colored output use the cprintf function.
*  @author Siddhesh Rane
*/

#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<string.h>

#include<curses.h>
#include<unistd.h>

 void cls(int,int,int,int,char);
 void init();
 void warn(char* , char*);
 void animate(char* , int);
 void fancy();
 void rightpane(int);
 char* input(char*,char*);
 void fc(char,char);
 void title(char*);

//screen width and height
int w=80 , h=25 , lastX=1, lastY = 1;
//window positions for dialogs (min must be 1 )
int x1 , y1 , x2 , y2;
//buffer for screen shot
char *buf;


void main(){
  init();
  cls(1,1,w,h,BLUE);
  textbackground(BLACK);
  textcolor(WHITE);
  for(int i=0 ; i<h ; i++)cprintf("This is a demo background ");
  fc(GREEN);
  title("TITLE DEMO");
  getch();
  title("INPUT DIALOG DEMO");
  char *name = input("this is the title","This is the body.Type something");
  title("WARNING DIALOG DEMO");
  warn("Warning Title" , "This is the body of the warning message.");
  warn("The User Entered", name);
  getch();
}


void fancy(){
 int y=0;
 while(!kbhit()){
  gotoxy(2,2);
  for(int i=0 ; i<20 ; i++){
   if( (i+y)%3 != 0){
   textcolor(GREEN);
   textbackground(BLACK);

   cprintf("-");}
   else {
    textcolor(GREEN);
    textbackground(BLACK);
    cprintf("<");
   }
  }
  y++;
  if(y>20)y=0;
  delay(90);
  delline();

 }
}

//Utility method for foreground and background color
void fc(char f , char b){
  textcolor(f);
  textbackground(b);
}

/*
  Creates a title bar at the top using the current colors.
  The title bar will overwrite on the first line. Cursor
  position is then restored.
*/
void title(char *ti){
 //title
  lastX = wherex() ; lastY = wherey();
 int len = strlen(ti);
 len = (w+len)/2;
 gotoxy(1,1);
 cprintf("%*s",len,ti);
 cprintf("%*c",w-len ,' ');
 gotoxy(lastX,lastY);
}
/*
 Shows an input dialog with a title and a short
 message with a text input box. Program is blocked
 till user presses enter key. Function returns
 the user entered text.
*/
char* input(char *title , char *msg){
 lastX = wherex() ; lastY = wherey();
 gettext(1,1,w,h,buf);
 char *file = new char[20];
 cls(x1,y1,x2,y2,GREEN);
 fc(GREEN , BLACK);
 animate(title,x2-x1);
 fc(BLACK,GREEN);
 cprintf("\n\r%s",msg);
 gotoxy(2,y2-y1);
 fc(MAGENTA);
 cprintf("%*s",x2-x1-2," ");
 gotoxy(3,y2-y1);
 gets(file);
 window(1,1,w,h);
 puttext(1,1,w,h,buf);
 gotoxy(lastX , lastY);
 return file;
}
/*
  Displays a warning dialog that shows a title and
  an assocated message. Program is blocked till user
  presses any key.
*/
void warn(char *title , char *msg){
  lastX=wherex(); lastY = wherey();
  _setcursortype(_NOCURSOR);
  gettext(1,1,w,h,buf);
  cls(x1,y1,x2,y2,WHITE);
  //Title
  textcolor(WHITE+BLINK);
  textbackground(RED);
    animate(title , x2-x1);
  //body
  textcolor(BLACK);
  textbackground(WHITE);
  cprintf("\n\r");
  animate(msg ,x2-x1);
  getch();
  clrscr();
  window(1,1,w,h);
  puttext(1,1,w,h,buf);
  gotoxy(lastX , lastY);
  _setcursortype(_NORMALCURSOR);
}

/*
 Meant for animating the text.
 On some systems animation is not smooth.
 You can disable the feature by replacing the whole
 body with the comment given.
 */
 void animate(char * text , int pad){
  int len = strlen(text);
  int SLEEP = 10; //milliseconds to sleep
   for(int i=0 ; i<=pad ; i++){
    if(i<=len)cprintf("%c", text[i]);
    else cprintf(" ");

    delay(SLEEP);

    if(kbhit()){ SLEEP = 0; getch(); }
    }
   //decomment the following to disable animation
   //cprintf("%-*s",pad ,text);

 }

/*creates a right pane
 no=1 : show pane
 no=0 : hide pane
 no=-1: lose focus
*/
void rightpane(int no){
 if(no == 1)
 {
  gettext(1,1,w,h,buf);
  cls(w*.8 , 1 , w , h , MAGENTA);
 }
 else if(no == 0 || no ==-1)
 {//1st lose focus
  window(1,1,w*.8,h);
  if(no==0)puttext(1,1,w,h,buf);
 }
}

/*
  Creates a window with coords x1,y1,x2,y2 filled with the
  given background color
*/
void cls(int x1 , int y1 , int x2 , int y2 , char color){
  window(x1,y1,x2,y2);
  textbackground(color);
  int w = x2-x1 ;
  int h = y2-y1 ;
  for(int i=0 ; i<h ; i++)
    cprintf("%*s\n\r" , w , " ");
  clrscr();
}
/*
  DO NOT ALTER THIS FUNCTION
  To be called once at the start of main()
*/
void init(){
  struct text_info ti;
  gettextinfo(&ti);
  w = ti.screenwidth;
  h = ti.screenheight;
  buf = new char[w*h*2];
  x1 = w*0.2 ;//20%
  x2 = w*0.8 ;//80%
  y1 = h*0.3 ;//30%
  y2 = h*0.7 ;//70%
  //  _setcursortype(_NOCURSOR);
}
