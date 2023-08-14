#include<iostream>
#include<chrono>
#include<thread>
#include<conio.h>
#include<cstdlib>
#include<ctime>
#include<stack>
#include<windows.h>
using namespace std;
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80

void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = y;
  coord.Y = x;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
struct snake{
	public:
		unsigned int row;
		unsigned int column;
		char pr;
};
snake generateFood(){
	snake s;
	srand(time(0));
	s.row=rand()%31+1;
	srand(time(0));
	s.column=rand()%38+1;
	gotoxy(s.row,s.column);
	return s;
}
void pause(int &dir,int copy){
	while(true){
		sleep(1);
		gotoxy(20,9);
		cout<<"GAME HAS BEEN PAUSED.";
		if(kbhit()){
			if(getch()==27){
				gotoxy(20,9);
				cout<<"GAME HAS BEEN RESUMED.";
				sleep(2);
				gotoxy(20,7);
				cout<<"                               ";
				break;
			}	
			else
				continue;
		}
	}
	dir=copy;
}
void passValue(snake s[],int n){
	for(int i=n;i>0;i--){
		s[i]=s[i-1];
	}
}
bool gameOver(snake s[],int n,snake obstacles[],int b,int v){
	if(s[0].row==0||s[0].column==0||(s[0].row==b-1)||(s[0].column==v))
		return true;
	if(n>3){
		for(int i=3;i<n;i++){
			if((s[0].row==s[i].row)&&(s[0].column==s[i].column))
				return true;
		}
	}
	for(int i=0;i<50;i++){
		if(s[0].row==obstacles[i].row&&s[0].column==obstacles[i].column)
			return true;
	}
	return false;
}
void border(snake obstacles[],int &b,int &v){
	int rec=0;
	for(int i=0;i<b;i++){
			gotoxy(i,0);
			cout<<"|";
			gotoxy(i,b-1);
			cout<<"|";
	}
	for(int i=0;i<v;i++){
			gotoxy(0,i);
			cout<<"_";
			gotoxy(v-1,i);
			cout<<"_";
	}
	for(int i=12;i<15;i++){
		gotoxy(i,12);
		obstacles[rec++]={i,12};
		cout<<"|";
		gotoxy(i,b-14);
		obstacles[rec++]={i,b-14};
		cout<<"|";
	}
	for(int i=v-16;i>v-20;i--){
		gotoxy(i,12);
		obstacles[rec++]={i,12};
		cout<<"|";
		gotoxy(i,b-14);
		obstacles[rec++]={i,b-14};
		cout<<"|";
	}
	for(int i=12;i<b-13;i++){
		gotoxy(11,i);
		obstacles[rec++]={11,i};
		cout<<"=";
		gotoxy(v-16,i);
		obstacles[rec++]={v-16,i};
		cout<<"=";
	}
}
bool overlap(snake food, snake s[],snake obstacles[],int n){
	for(int i=0;i<38;i++){
		if(food.row==obstacles[i].row&&food.column==obstacles[i].column)
			return true;
	}
	for(int i=0;i<n;i++){
		if(food.row==s[i].row&&food.column==s[i].column)
			return true;
	}
	return false;
}
void foodEaten(snake s[],snake &food,int &l,int &n,snake obstacles[],snake foodStore[]){
	if(s[0].row==food.row&&s[0].column==food.column){
		while(overlap(food,s,obstacles,n)){
			food=generateFood();
		}
		foodStore[l++]=food;
		n++;
	}
}
void assignDirection(snake s[],int &dir,int &copy){
	switch(dir){
				case KEY_UP:	s[0].row--;	copy=KEY_UP; break;
				case KEY_DOWN:  s[0].row++;	copy=KEY_DOWN; break;
				case KEY_LEFT:  s[0].column--;	copy=KEY_LEFT;	break;
				case KEY_RIGHT: s[0].column++;	copy=KEY_RIGHT;	break;
				default : if(dir<223) {s[0].pr=dir;} dir=copy; break;
			}
}
void drawFoodnBody(snake &food,snake s[],int n,char print,snake obstacles[],stack <snake> &undo){
	gotoxy(food.row,food.column);
	cout<<"o";
	for(int i=0;i<n;i++){
		gotoxy(s[i].row,s[i].column);
		cout<<s[0].pr;
	}
	if(s[0].row>0&&s[0].column>0){
		gotoxy(s[n].row,s[n].column);
			cout<<" ";
	}
}
void undoStep(snake s[],int &n,snake foodStore[],int &l,snake &food,stack <snake> &redo,stack <snake> &undo){
//	for(int i=0;i<n;i++){
//			undo.pop();
//		}
	if(!undo.empty()){
		gotoxy(s[0].row,s[0].column);
		cout<<" ";
		redo.push(s[0]);
		for(int i=0;i<n;i++){
			s[i]=s[i+1];
		}
		s[n-1]=undo.top();
		undo.pop();
		if(foodStore[l-2].row==s[0].row&&foodStore[l-2].column==s[0].column){
			gotoxy(foodStore[l-1].row,foodStore[l-1].column);
			cout<<" ";
			food=foodStore[l-2];
			n--;
			l--;
		}		
	}
}
void redoStep(snake s[],stack <snake> &redo,int &l,int &n,snake &food,snake foodStore[],stack <snake> &undo){
	if(!redo.empty()){
		undo.push(s[0]);
		passValue(s,n);
		gotoxy(s[0].row,s[0].column);
		cout<<" ";
		s[0]=redo.top();
		redo.pop();
		if(s[0].row==food.row&&s[0].column==food.column){
			food=foodStore[l];
			l++;
			n++;
		}
	}
}
void initialize(int &dir,bool &gamefinish,snake s[],int &n,int &copy,stack <snake> &undo){
	dir=KEY_RIGHT;
	copy=dir;
	for(int i=0;i<n;i++)
		s[i]={1,n-i};
	undo.push(s[0]);
	gamefinish=false;
}
void extraChances(int &lives){
	lives--;
	gotoxy(20,8);
	if(lives==1)
		cout<<"You have 1 life remaining.";
	else
		cout<<"You have "<<lives<<" lives remaining";
	sleep(5);
	system("cls");
}
void keyboard(int &dir,int &w,int copy,snake s[],snake &food, int &n,char print,snake obstacles[],snake foodStore[],int l,stack <snake> redo,int &b, int &v,stack <snake> &undo){
	dir=getch();
	if(dir==27){
		pause(dir,copy);
	}
	while(dir=='z'){
		undoStep(s,n,foodStore,l,food,redo,undo);
		drawFoodnBody(food,s,n,print,obstacles,undo);
		dir=getch();
	}
	if(dir=='w'){
		if(w<20)
			w++;
		else
			w=1;
	}
	while(dir=='r'){
		redoStep(s,redo,l,n,food,foodStore,undo);
		drawFoodnBody(food,s,n,print,obstacles,undo);
		dir=getch();
	}
	if(dir=='b'||dir=='v'){
		if(dir=='b'){
			b++;
			v++;
		}
		else{
			b--;
			v--;
		}
		system("cls");
		border(obstacles,b,v);
	}
}
int main(void){
	int dir,n=1,l=1,copy,lives=3,w=1,b=40,v=40;
	snake s[400],obstacles[50];
	bool gamefinish;char print;
	snake food=generateFood(),foodStore[60]={{food}};
	stack <snake> redo,undo;
	while(lives>0){
		border(obstacles,b,v);
		initialize(dir,gamefinish,s,n,copy,undo);
		while(!gamefinish){
			s[0].pr=w;
			undo.push(s[0]);
			drawFoodnBody(food,s,n,s[0].pr,obstacles,undo);
			foodEaten(s,food,l,n,obstacles,foodStore);
			if(kbhit()){
				keyboard(dir,w,copy,s,food,n,s[0].pr,obstacles,foodStore,l,redo,b,v,undo);
			}
			if(((dir==KEY_DOWN)&&(copy==KEY_UP))||((dir==KEY_UP)&&(copy==KEY_DOWN))||((dir==KEY_RIGHT)&&(copy==KEY_LEFT))||((dir==KEY_LEFT)&&(copy==KEY_RIGHT))){
				dir=copy;
			}
			passValue(s,n);
			if(dir!='r')
				assignDirection(s,dir,copy);
			this_thread::sleep_for(chrono::milliseconds(250));
			gamefinish=gameOver(s,n,obstacles,b,v);
		}
		extraChances(lives);
	}
	cout<<"Score = "<<n-1;
	return 0;
}