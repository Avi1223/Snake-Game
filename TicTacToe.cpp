#include<iostream>
#include<stdlib.h>
using namespace std;

bool check(int a[]){
	if((a[0]==a[4]&&a[0]==a[8])||(a[2]==a[4]&&a[2]==a[6]))
		return true;
	if((a[0]==a[3]&&a[0]==a[6])||(a[1]==a[4]&&a[1]==a[7])||(a[2]==a[5]&&a[2]==a[8]))
		return true;
	for(int i=0;i<9;i=i+3){
		if(a[i]==a[i+1]&&a[i]==a[i+2])
			return true;
	}
	return false;
}

bool draw(int a[]){
	int count=0;
	for(int i=0;i<9;i++){
		if(a[i]=='X'||a[i]=='O'){
			count++;
		}
	}
	if(count==8)
		return true;
	else
		return false;
}

void display(int a[]){
	char b;
	for(int i=0;i<9;i++){
		if(i%3==0){
			cout<<endl;
		}
		if(a[i]==79||a[i]==88){
			b=a[i];
			cout<<b<<" ";
		}	
		else{
			cout<<(a[i])<<" ";
		}
	}
	cout<<endl;
}
class player{
	private:
		int playerNo;
		char element;
	public:
		player(int playerNo, char element){
			this->playerNo=playerNo;
			this->element=element;
		}
		void input(int a[]){
			int in;
			int i=0;
			cout<<"Player "<<playerNo<<"'s turn - ";
			cin>>in;
			while(a[in-1]=='X'||a[in-1]=='O'||in>9||in<1){
				cout<<"Invalid Input! Enter Again - ";
				cin>>in;
			}	
			while(i<9){
				if(i==(in-1)){
					a[i]=element;
					break;
				}
				else{
					i++;
				}
			
			}
			if(check(a)){
				display(a);
				cout<<"Player "<<playerNo<<" Won!";
				exit(0);
			}
		}
};


 int main(void){
 	player p1(1,'X'),p2(2,'O');
 	int a[9];
 	int n=1,count=0;
	for(int i=0;i<9;i++){
			a[i]=n;
			n++;
	}
	while(true){
		if(draw(a))
			break;
		display(a);	
		p1.input(a);
		system("cls");
		display(a);
	    p2.input(a);
	    system("cls");
	}	
	cout<<"The Game is Tied.";
 	return 0;
 }