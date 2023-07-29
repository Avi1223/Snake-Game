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
	unsigned int row;
	unsigned int column;
};
snake generateFood(){
	snake s;
	srand(time(0));
	s.row=rand()%20;
	srand(time(0));
	s.column=rand()%30;
	gotoxy(s.row,s.column);
	return s;
}

void passValue(snake s[],int n){
	for(int i=n-1;i>0;i--){
		snake temp=s[i+1];
		s[i]=s[i-1];
		
	}
}

bool gameOver(snake s[],int n){
	if(s[0].row==0||s[0].column==0||s[0].row==39||s[0].column==39)
		return true;
//	for(int i=1;i<n;i++){
//		if((s[0].row==s[i].row)&&(s[0].column==s[i].column))
//			return true;
//	}
	return false;
}

void border(int i,int j){
	gotoxy(i,j);
	cout<<"+";
}
int main(void){
	int c=KEY_RIGHT;
	snake s[20]={{1,1}};
	int n=1;
	snake food=generateFood();
	bool gamefinish=false;
	while(!gamefinish){
			gotoxy(food.row,food.column);
			cout<<"o";
			for(int i=0;i<40;i++){
				border(0,i);
				border(i,0);
				border(39,i);
				border(i,39);
			}
			for(int j=0;j<n;j++){
				gotoxy(s[j].row,s[j].column);
				cout<<"*";
			}
			if(s[0].row==food.row&&s[0].column==food.column){
				food=generateFood();
				n++;
			}
			if(kbhit()){
				c=getch();
			}
			passValue(s,n);
			switch(c){
				case KEY_UP:	s[0].row--;	break;
				case KEY_DOWN:  s[0].row++;	break;
				case KEY_LEFT:  s[0].column--;	break;
				case KEY_RIGHT: s[0].column++;	break;
			}
			gamefinish=gameOver(s,n);
			sleep(1);
			gotoxy(300,50);
			cout<<"SCORE = "<<n;
			system("cls");
		}
		cout<<"GAME OVER!";         
}
	
