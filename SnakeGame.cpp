#include<iostream>
#include<conio.h>
#include<cstdlib>
#include<ctime>
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
};
snake generateFood(){
	snake s;
	srand(time(0));
	s.row=rand()%10+1;
	srand(time(0));
	s.column=rand()%10+1;
	gotoxy(s.row,s.column);
	return s;
}
void pause(int &c,int copy, int lives,int n, snake s[]){
	while(true){
		sleep(1);
		gotoxy(20,50);
		cout<<"GAME HAS BEEN PAUSED.";
		if(kbhit()){
			if(getch()==27)
				gotoxy(20,50);
				cout<<"GAME HAS BEEN RESUMED.";
				sleep(2);
				break;
		}
	}
	c=copy;
}
void passValue(snake s[],int n){
	for(int i=n+100;i>0;i--){
		s[i]=s[i-1];
	}
}
bool gameOver(snake s[],int n,snake obstacles[]){
	if(s[0].row==0||s[0].column==0||s[0].row==39||s[0].column==39)
		return true;
	if(n>3){
		for(int i=3;i<n;i++){
			if((s[0].row==s[i].row)&&(s[0].column==s[i].column))
				return true;
		}
	}
	for(int i=0;i<38;i++){
		if(s[0].row==obstacles[i].row&&s[0].column==obstacles[i].column)
			return true;
	}
	return false;
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
void border(snake obstacles[]){
	int rec=0;
	for(int i=0;i<40;i++){
			gotoxy(i,0);
			cout<<"|";
			gotoxy(i,39);
			cout<<"|";
	}
	for(int i=0;i<40;i++){
			gotoxy(39,i);
			cout<<"_";
			gotoxy(0,i);
			cout<<"_";
	}
	for(int i=13;i<26;i++){
		gotoxy(11,i);
		obstacles[rec++]={11,i};
		cout<<"=";
		gotoxy(24,i);
		obstacles[rec++]={24,i};
		cout<<"=";
	}
	for(int i=12;i<15;i++){
		gotoxy(i,12);
		obstacles[rec++]={i,12};
		cout<<"|";
		gotoxy(i,26);
		obstacles[rec++]={i,26};
		cout<<"|";
	}
	for(int i=24;i>21;i--){
		gotoxy(i,12);
		obstacles[rec++]={i,12};
		cout<<"|";
		gotoxy(i,26);
		obstacles[rec++]={i,26};
		cout<<"|";
	}
}
void assignDirection(snake s[],int c,int &copy,char &print){
	switch(c){
				case KEY_UP:	s[0].row--;	copy=KEY_UP; break;
				case KEY_DOWN:  s[0].row++;	copy=KEY_DOWN; break;
				case KEY_LEFT:  s[0].column--;	copy=KEY_LEFT;	break;
				case KEY_RIGHT: s[0].column++;	copy=KEY_RIGHT;	break;
				default : if(c<223) {print=c;} c=copy; break;
			}
}
void drawFoodnBorder(snake food,snake s[],int n,char &print,snake obstacles[]){
	system("cls");
	gotoxy(food.row,food.column);
	cout<<"o";
	border(obstacles);
	for(int i=0;i<n;i++){
		gotoxy(s[i].row,s[i].column);
		cout<<print;
	}
}
int undo(snake s[],int &n,snake foodStore[],int &l,snake &food){
	if(s[0].row<2&&s[0].column<2)
		return 0;
	for(int i=0;i<n+200;i++){
		s[i]=s[i+1];
	}
	if(foodStore[l-2].row==s[0].row&&foodStore[l-2].column==s[0].column){
		n--;
		food=foodStore[l-2];
		l--;
	}
	return 0;
}
int main(void){
	int c,n=1,l=1,copy,lives=3;
	snake s[400],obstacles[38];
	char print='*';
	bool gamefinish;
	snake food=generateFood();
	snake foodStore[60]={{food}};
	while(lives>0){
		c=KEY_RIGHT;
		gamefinish=false;
		for(int i=0;i<n;i++)
			s[i]={1,n-i};
		gamefinish=false;
		while(!gamefinish){
			drawFoodnBorder(food,s,n,print,obstacles);
			if(s[0].row==food.row&&s[0].column==food.column){
				while(overlap(food,s,obstacles,n)){
					food=generateFood();
				}
				foodStore[l++]=food;
				n++;
			}
			if(kbhit()){
				c=getch();
				if(c==27){
					pause(c,copy,lives,n,s);
				}
				else if(c=='z'){
					while(c=='z'){
						undo(s,n,foodStore,l,food);
						drawFoodnBorder(food,s,n,print,obstacles);
						c=getch();
					}	
				}
				
			}
			if(((c==KEY_DOWN)&&(copy==KEY_UP))||((c==KEY_UP)&&(copy==KEY_DOWN))||((c==KEY_RIGHT)&&(copy==KEY_LEFT))||((c==KEY_LEFT)&&(copy==KEY_RIGHT))){
				passValue(s,n);
				assignDirection(s,copy,copy,print);
			}
			else{
				passValue(s,n);
			    assignDirection(s,c,copy,print);
			}
			gamefinish=gameOver(s,n,obstacles);
			sleep(1);
		}
		lives--;
		gotoxy(20,50);
		if(lives==1)
			cout<<"You have 1 life remaining.";
		else
			cout<<"You have "<<lives<<" lives remaining";
		sleep(5);
	}
}