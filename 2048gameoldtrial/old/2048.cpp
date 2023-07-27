//��ɽ��ţ��ֱ����ָ��������飺*(c+(i*7)+j) 
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#define WINVALUE 2048
using namespace std;
bool graphic(int cb[7][7]);
int spawn(int *c); 
int key();
bool move(int *c,int in);
bool moveblocked(int *c,int in);
bool referee(int *c);
int classicgame();
int main(){
	char key='\0';
	while(key!='q'&&key!='Q'){
		system("cls");
		printf("��ӭ����ʷ�����ѵ�2048��\n����㲻�뱻��ô�ѵ���Ϸ���������������Ծ���ģʽ��\n1.����ģʽ  2.�������ѵ�2048 Q.�˳���Ϸ\n"); 
		key=getch();
		switch(key){
			case '1':classicgame();break;
			default:;
		}	
	}
}
/*
����ȱ�������������˾Ͱ�����ɾ�ˣ�
	�ƶ����ˣ� �����һ���������ƶ����˵�����һ���Ҫ�ƶ����ǳ����ʲôҲ�������ȴ������һ��������
	���ڵ������������ҷ�Ҫ�ƶ����������Ȼ�����ƶ������ǻ���ˢ����һ��Ԫ�� 
	�䣨Ԥ�ڵ�winֵ=2����Ŀǰ���㷨�����Ƕ����ڼ��㶯��������cb�����������жϳ��������еĸ��Ӷ������ġ� 
	��һ��ÿ��Ԫ�ؼ�������������ڵ��ĸ�Ԫ���ǲ�����ͬ��������е�Ԫ�ض�������ͬ�ľ����ˣ���Ϊ������ 
	λ���ظ������⣺������ǰ��������С��һ�룬��ôѡȡ��һ��Ԫ�س���λ�õ�ʱ�������������
	�ͻ�����һ������ӣ����ε�λ�þͻ�һ�������������˫���������ÿ������+1�ķ�������� 
*/
int classicgame(){
	int cb[7][7]={0};//չʾ����ҵ�����
	int i,j,k;//ѭ������������ 
	int in;//�洢��ֵ�� 
	bool gameover=false,win=false;//��Ӯ�ı���	
	bool quit=false;
	//���������еĵ�һ��Ԫ��spawn(*cb); 
	//����Ӧ׼������,��������Ϊ������ 
	while(!gameover){
		spawn(*cb);graphic(cb);//ˢ�½��� 
		in=key();//�������ָ��
		//if(!moveblocked(*cb,in)){}//������������һ����Ԫ��	
		//cout<<in;
		win=move(*cb,in);
		if(in==-1){
			quit=true;
		}
		if(quit){
			break;
		}
		if(win){//�������ֵ��0����ܻ������������1����һ������ƶ���2����Ҳ������ƶ������ˡ����Խ�����Ҫд��һ�������ж��ܲ����ƶ��� 
			break;
		}
		//gameover=referee(*cb);//��������ɨ��һ�����̣�������ˣ���gameover,������û���˵�䣬bug��Դ 
	}
	if(win){
		graphic(cb);
		printf("��Ӯ�ˣ���������˳���"); 
		getch();
	}else if(gameover){
		graphic(cb);
		printf("�����ˣ�Ҫ����һ����");
		getch();
	}else if(quit){
	}else{
		printf("���������һ�����󡣴����룺%d %d",win,gameover,quit); 
	}
}
bool move(int *c,int in){
	int i,j,k;
	bool win=false;
	int cb[7][7];
	for(i=0;i<=6;i++){
		for(j=0;j<=6;j++){
			cb[i][j]=*(c+(i*7)+j);
		}
	}
	switch(in){//������������ָ����ƶ����� 
		case 1:{
			for(k=1;k<=4;k++){//k������ 
				for(i=2;i<=4;i++){//ɨ��Ԫ�� 
					if(cb[i][k]!=0){//����Ԫ�� 
						for(j=i-1;j>=1;j--){//graphic(cb);//�������ϰ������ͬԪ�� 
							if(cb[j][k]!=0){//�ҵ��ϰ��� 
								if(cb[j][k]==cb[i][k]){//�ϰ�������ͬԪ�� 
									cb[j][k]=cb[j][k]*2;//��������ͬ��Ԫ�غϲ������ｫ�ӷ�����*2����ʽ 
									cb[i][k]=0;//��ԭ���ĵط�����
									 
								}else{//�ϰ����ǲ�ͬԪ�� 
									cb[j+1][k]=cb[i][k];
									if(j+1!=i){
										cb[i][k]=0;
									}							
								}
								break;							
							}
							else if(cb[j][k]==0&&j==1){//û�ҵ��ϰ�������Ѿ������� 
								cb[1][k]=cb[i][k];
								cb[i][k]=0;
							}						
						}
					} 
				}	
			}
			break;
		}
		case 2:{
			for(k=1;k<=4;k++){
				for(i=2;i<=4;i++){//ɨ��Ԫ�� 
					if(cb[k][i]!=0){//����Ԫ�� 
						for(j=i-1;j>=1;j--){//graphic(cb);//�������ϰ������ͬԪ�� 
							if(cb[k][j]!=0){//�ҵ��ϰ��� 
								if(cb[k][j]==cb[k][i]){//�ϰ�������ͬԪ�� 
									cb[k][j]=cb[k][j]*2;//��������ͬ��Ԫ�غϲ������ｫ�ӷ�����*2����ʽ 
									cb[k][i]=0;//��ԭ���ĵط�����
									 
								}else{//�ϰ����ǲ�ͬԪ�� 
									cb[k][j+1]=cb[k][i];
									if(j+1!=i){//�ƶ��ɹ� 
										cb[k][i]=0;
									}				
								}
								break;							
							}
							else if(cb[k][j]==0&&j==1){//û�ҵ��ϰ�������Ѿ������� 
								cb[k][1]=cb[k][i];
								cb[k][i]=0;
							}						
						}
					} 
				}	
			}				
			break;
		}
		case 3:{
			for(k=4;k>=1;k--){
				for(i=3;i>=1;i--){//ɨ��Ԫ�� 
					if(cb[i][k]!=0){//����Ԫ�� 
						for(j=i+1;j<=4;j++){//graphic(cb);//�������ϰ������ͬԪ�� 
							if(cb[j][k]!=0){//�ҵ��ϰ��� 
								if(cb[j][k]==cb[i][k]){//�ϰ�������ͬԪ�� 
									cb[j][k]=cb[j][k]*2;//��������ͬ��Ԫ�غϲ������ｫ�ӷ�����*2����ʽ 
									cb[i][k]=0;//��ԭ���ĵط�����
									 
								}else{//�ϰ����ǲ�ͬԪ�� 
									cb[j-1][k]=cb[i][k];
									if(j-1!=i){
										cb[i][k]=0;
									}							
								}
								break;							
							}
							else if(cb[j][k]==0&&j==4){//û�ҵ��ϰ�������Ѿ������� 
								cb[4][k]=cb[i][k];
								cb[i][k]=0;
							}						
						}
					} 
				}	
			}				
			break;
		}
		case 4:{
			for(k=4;k>=1;k--){
				for(i=3;i>=1;i--){//ɨ��Ԫ�� 
					if(cb[k][i]!=0){//����Ԫ�� 
						for(j=i+1;j<=4;j++){//graphic(cb);//�������ϰ������ͬԪ�� 
							if(cb[k][j]!=0){//�ҵ��ϰ��� 
								if(cb[k][j]==cb[k][i]){//�ϰ�������ͬԪ�� 
									cb[k][j]=cb[k][j]*2;//��������ͬ��Ԫ�غϲ������ｫ�ӷ�����*2����ʽ 
									cb[k][i]=0;//��ԭ���ĵط�����
									 
								}else{//�ϰ����ǲ�ͬԪ�� 
									cb[k][j-1]=cb[k][i];
									if(j-1!=i){
										cb[k][i]=0;
									}					
								}
								break;							
							}
							else if(cb[k][j]==0&&j==4){//û�ҵ��ϰ�������Ѿ������� 
									cb[k][4]=cb[k][i];
									cb[k][i]=0;
							}						
						}
					} 
				}	
			}
			break;
		}		
	}
	for(i=0;i<=6;i++){
		for(j=0;j<=6;j++){
			*(c+(i*7)+j)=cb[i][j];
		}
	}
	for(i=1;i<=4;i++){//Ӯ
		for(j=1;j<=4;j++){
			if(cb[i][j]==WINVALUE){//Ӯ��������ֵ 
				win=true;
			}
		}
	}
	if(win){
		return true;
	}else{
		return false;
	}
}
bool moveblocked(int *c,int in){
	int i,j,k;
	int cb[7][7];
	for(i=0;i<=6;i++){
		for(j=0;j<=6;j++){
			cb[i][j]=*(c+(i*7)+j);
		}
	}
	switch(in){
		case 1:{//�ӵ�һ��ɨ������������������ǿյĶ���һ��λ���У�����������������һ�����ֺ�������ͬ��֤�������ƶ� 
			for(k=1;k<=4;k++){//k������ 
				for(i=1;i<=3;i++){//�ӵײ���һ��������ɨ�赽3�����ĸ�������ɨ�� 
					if(cb[i][k]==0){
						if(cb[i+1][k]!=0){
							return true;break;
						}
					}else{
						if(cb[i+1][k]==cb[i][k]){
							return true;break;
						}else{
							return false;
						}
					}
				}
			}
			break;
		}
	}
}
bool referee(int *c){//�ж��Ƿ�����Ŀǰ��������bug 
	int i,j;
	bool gameover=true;
	int cbo[7][7];
	int cbw[7][7];
	int cba[7][7];
	int cbs[7][7];
	int cbd[7][7];
	for(i=0;i<=6;i++){//��ԭʼ���ݸ���һ�ݵ��ĸ����鵱�� 
		for(j=0;j<=6;j++){
			cbw[i][j]=*(c+(i*7)+j);
			cba[i][j]=*(c+(i*7)+j);
			cbs[i][j]=*(c+(i*7)+j);
			cbd[i][j]=*(c+(i*7)+j);
			cbo[i][j]=*(c+(i*7)+j);
		}
	}
	move(*cbw,1);
	move(*cba,2);
	move(*cbs,3);
	move(*cbd,4);
	for(i=0;i<=6;i++){
		for(j=0;j<=6;j++){
			if(!((cbo[i][j]==cbw[i][j])&&(cbo[i][j]==cbw[i][j])&&(cbo[i][j]==cbw[i][j])&&(cbo[i][j]==cbw[i][j]))){
				/*���cbo��ֵ������һ������ȣ�֤����һ�������Ϸ������ƶ� 
				��ô��Ϸ�����ܽ�����������gameoverΪfalse*/
				gameover=false;
			}
		}
	}
	return gameover;
}
int key(){
	char key;
	key=getch();
	switch(key){//û���Ǵ�д
		case 'w':return 1;break;
		case 'a':return 2;break;
		case 's':return 3;break;
		case 'd':return 4;break;
		case '\n':return 5;break;
		case 'q':return -1;break;
		default:return 0;
	}
}
bool graphic(int cb[7][7]){
	string c[7][7];
	int i,j,p;
	for (i=0;i<=6;i++){
		for (j=0;j<=6;j++){
			switch(cb[i][j]){
				case 0:c[i][j]="    ";break;
				case 2:c[i][j]="   2";break;
				case 4:c[i][j]="   4";break;
				case 8:c[i][j]="   8";break;
			}
		}
	}
	//system("cls");
	for(i=1;i<=4;i++){
		printf("+----+----+----+----+\n");
		for(j=1;j<=4;j++){
			printf("|");
			if(cb[i][j]==0){
				printf("    ");
			}
			else{
				printf("%4d",cb[i][j]);
			}
		}
		printf("|\n");
	}
	printf("+----+----+----+----+\n");
	return true;
}
int spawn(int *c){//�������֮�������Ȼ�յ���������ͻ����� 
	int position[7][7]={0};
	int i,j,p,n=0,x,y;
	for (i=1;i<=4;i++){
		for (j=1;j<=4;j++){
			if(*(c+(i*7)+j)==0){
				n++;
				position[i][j]=n;				
			}
		}
	}
	//graphic(position);
	n++;
	srand((unsigned)time(NULL));
	p=rand()%n+1;
	for (i=1;i<=4;i++){
		for (j=1;j<=4;j++){
			if(position[i][j]==p){//bug�ǰ����еĿո��Ӹ�����2    ��֤ʵ 
				*(c+(i*7)+j)=2;cout<<p;
			}
		}
	}
}
