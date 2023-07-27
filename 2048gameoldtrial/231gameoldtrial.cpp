// 231gameoldtrial.cpp: 定义应用程序的入口点。
//
//隔山打牛法直接用指针调用数组：*(c+(i*7)+j) 
#include <iostream>
#include <stdlib.h>
#include <string.h>
//#include <conio.h>
#include <time.h>
#include <string.h>
#define WINVALUE 2048
#include "231gameoldtrial.h"

using namespace std;

bool graphic(int cb[7][7]);
void spawn(int* c);
int key();
bool move(int* c, int in);
bool moveblocked(int* c, int in);
bool referee(int* c);
void classicgame();
char input();

int main()
{
	char key = '\0';
	while (key != 'q' && key != 'Q') {
		system("cls");
		printf("欢迎来到史上最难的2048！\n如果你不想被那么难的游戏打击到，你可以试试经典模式！\n1.经典模式  2.世上最难的2048 Q.退出游戏\n");
		key = input();
		switch (key) {
			case '1':classicgame(); break;
			default:;
		}
	}
	return 0;
}
/*
程序还缺三个功能做完了就把这条删了：
	移动不了： 如果朝一个方向本来移动不了但是玩家还非要移动，那程序就什么也不做，等待玩家下一个操作。
	现在的问题是如果玩家非要移动，程序就虽然不能移动，但是会再刷出来一个元素
	输（预期的win值=2）：目前的算法决定是对用于计算动作的数组cb迭代，首先判断出棋盘所有的格子都是满的。
	下一步每个元素检测上下左右相邻的四个元素是不是相同，如果所有的元素都不是相同的就输了，因为动不了
	位置重复的问题：如果玩家前后操作间隔小于一秒，那么选取下一个元素出现位置的时候随机数的种子
	就还是上一秒的种子，两次的位置就会一样。这个决定用双层随机数和每次种子+1的方法解决。
*/
void classicgame() {
	int cb[7][7] = { 0 };//展示给玩家的棋盘
	int i, j, k;//循环变量必须用 
	int in;//存储键值用 
	bool gameover = false, win = false;//输赢的变量	
	bool quit = false;
	//生成棋盘中的第一个元素spawn(*cb); 
	//这里应准备棋盘,这条仅作为测试用 
	while (!gameover) {
		spawn(*cb); graphic(cb);//刷新界面 
		in = key();//玩家做出指令
		if(!moveblocked(*cb,in)){}//在棋盘中生成一个新元素	
		//cout<<in;
		win = move(*cb, in);
		if (in == -1) {
			quit = true;
		}
		if (quit) {
			break;
		}
		if (win) {//如果返回值是0则可能还有两种情况：1，玩家还可以移动。2，玩家不可以移动，死了。所以接下来要写另一个程序判断能不能移动。 
			break;
		}
		gameover=referee(*cb);//在这里在扫描一次棋盘，如果死了，就gameover,这里总没输就说输，bug根源 
	}
	if (win) {
		graphic(cb);
		printf("你赢了！按任意键退出。");
		input();
	}
	else if (gameover) {
		graphic(cb);
		printf("你输了！要再来一局吗？");
		input();
	}
	else if (quit) {
	}
	else {
		printf("程序出现了一个错误。错误码：%d %d", win, gameover, quit);
	}
}
bool move(int* c, int in) {
	int i, j, k;
	bool win = false;
	int cb[7][7];
	for (i = 0; i <= 6; i++) {
		for (j = 0; j <= 6; j++) {
			cb[i][j] = *(c + (i * 7) + j);
		}
	}
	switch (in) {//辨别玩家做出的指令，并移动棋盘 
	case 1: {
		for (k = 1; k <= 4; k++) {//k是列数 
			for (i = 2; i <= 4; i++) {//扫描元素 
				if (cb[i][k] != 0) {//发现元素 
					for (j = i - 1; j >= 1; j--) {//graphic(cb);//往回找障碍物或相同元素 
						if (cb[j][k] != 0) {//找到障碍物 
							if (cb[j][k] == cb[i][k]) {//障碍物是相同元素 
								cb[j][k] = cb[j][k] * 2;//将两个相同的元素合并，这里将加法换成*2的形式 
								cb[i][k] = 0;//将原来的地方清零

							}
							else {//障碍物是不同元素 
								cb[j + 1][k] = cb[i][k];
								if (j + 1 != i) {
									cb[i][k] = 0;
								}
							}
							break;
						}
						else if (cb[j][k] == 0 && j == 1) {//没找到障碍物，但是已经到底了 
							cb[1][k] = cb[i][k];
							cb[i][k] = 0;
						}
					}
				}
			}
		}
		break;
	}
	case 2: {
		for (k = 1; k <= 4; k++) {
			for (i = 2; i <= 4; i++) {//扫描元素 
				if (cb[k][i] != 0) {//发现元素 
					for (j = i - 1; j >= 1; j--) {//graphic(cb);//往回找障碍物或相同元素 
						if (cb[k][j] != 0) {//找到障碍物 
							if (cb[k][j] == cb[k][i]) {//障碍物是相同元素 
								cb[k][j] = cb[k][j] * 2;//将两个相同的元素合并，这里将加法换成*2的形式 
								cb[k][i] = 0;//将原来的地方清零

							}
							else {//障碍物是不同元素 
								cb[k][j + 1] = cb[k][i];
								if (j + 1 != i) {//移动成功 
									cb[k][i] = 0;
								}
							}
							break;
						}
						else if (cb[k][j] == 0 && j == 1) {//没找到障碍物，但是已经到底了 
							cb[k][1] = cb[k][i];
							cb[k][i] = 0;
						}
					}
				}
			}
		}
		break;
	}
	case 3: {
		for (k = 4; k >= 1; k--) {
			for (i = 3; i >= 1; i--) {//扫描元素 
				if (cb[i][k] != 0) {//发现元素 
					for (j = i + 1; j <= 4; j++) {//graphic(cb);//往回找障碍物或相同元素 
						if (cb[j][k] != 0) {//找到障碍物 
							if (cb[j][k] == cb[i][k]) {//障碍物是相同元素 
								cb[j][k] = cb[j][k] * 2;//将两个相同的元素合并，这里将加法换成*2的形式 
								cb[i][k] = 0;//将原来的地方清零

							}
							else {//障碍物是不同元素 
								cb[j - 1][k] = cb[i][k];
								if (j - 1 != i) {
									cb[i][k] = 0;
								}
							}
							break;
						}
						else if (cb[j][k] == 0 && j == 4) {//没找到障碍物，但是已经到底了 
							cb[4][k] = cb[i][k];
							cb[i][k] = 0;
						}
					}
				}
			}
		}
		break;
	}
	case 4: {
		for (k = 4; k >= 1; k--) {
			for (i = 3; i >= 1; i--) {//扫描元素 
				if (cb[k][i] != 0) {//发现元素 
					for (j = i + 1; j <= 4; j++) {//graphic(cb);//往回找障碍物或相同元素 
						if (cb[k][j] != 0) {//找到障碍物 
							if (cb[k][j] == cb[k][i]) {//障碍物是相同元素 
								cb[k][j] = cb[k][j] * 2;//将两个相同的元素合并，这里将加法换成*2的形式 
								cb[k][i] = 0;//将原来的地方清零

							}
							else {//障碍物是不同元素 
								cb[k][j - 1] = cb[k][i];
								if (j - 1 != i) {
									cb[k][i] = 0;
								}
							}
							break;
						}
						else if (cb[k][j] == 0 && j == 4) {//没找到障碍物，但是已经到底了 
							cb[k][4] = cb[k][i];
							cb[k][i] = 0;
						}
					}
				}
			}
		}
		break;
	}
	}
	for (i = 0; i <= 6; i++) {
		for (j = 0; j <= 6; j++) {
			*(c + (i * 7) + j) = cb[i][j];
		}
	}
	for (i = 1; i <= 4; i++) {//赢
		for (j = 1; j <= 4; j++) {
			if (cb[i][j] == WINVALUE) {//赢的最终数值 
				win = true;
			}
		}
	}
	if (win) {
		return true;
	}
	else {
		return false;
	}
}
bool moveblocked(int* c, int in) {
	int i, j, k;
	int cb[7][7];
	for (i = 0; i <= 6; i++) {
		for (j = 0; j <= 6; j++) {
			cb[i][j] = *(c + (i * 7) + j);
		}
	}
	switch (in) {
	case 1: {//从第一个扫到第三个，如果这里是空的而下一个位置有，或者这里有数字下一个数字和这里相同，证明可以移动 
		for (k = 1; k <= 4; k++) {//k是列数 
			for (i = 1; i <= 3; i++) {//从底部第一个数向上扫描到3，第四个数不用扫描 
				if (cb[i][k] == 0) {
					if (cb[i + 1][k] != 0) {
						return true; break;
					}
				}
				else {
					if (cb[i + 1][k] == cb[i][k]) {
						return true; break;
					}
					else {
						return false;
					}
				}
			}
		}
		break;
	}
	}
}
bool referee(int* c) {//判断是否卡死，目前会有严重bug 
	int i, j;
	bool gameover = true;
	int cbo[7][7];
	int cbw[7][7];
	int cba[7][7];
	int cbs[7][7];
	int cbd[7][7];
	for (i = 0; i <= 6; i++) {//将原始数据复制一份到四个数组当中 
		for (j = 0; j <= 6; j++) {
			cbw[i][j] = *(c + (i * 7) + j);
			cba[i][j] = *(c + (i * 7) + j);
			cbs[i][j] = *(c + (i * 7) + j);
			cbd[i][j] = *(c + (i * 7) + j);
			cbo[i][j] = *(c + (i * 7) + j);
		}
	}
	move(*cbw, 1);
	move(*cba, 2);
	move(*cbs, 3);
	move(*cbd, 4);
	for (i = 0; i <= 6; i++) {
		for (j = 0; j <= 6; j++) {
			if (!((cbo[i][j] == cbw[i][j]) && (cbo[i][j] == cbw[i][j]) && (cbo[i][j] == cbw[i][j]) && (cbo[i][j] == cbw[i][j]))) {
				/*如果cbo的值与其中一个不相等，证明有一个方向上发生了移动
				那么游戏还不能结束，于是让gameover为false*/
				gameover = false;
			}
		}
	}
	return gameover;
}
int key() {
	//key = input();
	switch (input()) {//没考虑大写
	case 'w':return 1; break;
	case 'a':return 2; break;
	case 's':return 3; break;
	case 'd':return 4; break;
	case '\n':return 5; break;
	case 'q':return -1; break;
	default:return 0;
	}
}

bool graphic(int cb[7][7]) {
	string c[7][7];
	int i, j, p;
	for (i = 0; i <= 6; i++) {
		for (j = 0; j <= 6; j++) {
			switch (cb[i][j]) {
			case 0:c[i][j] = "    "; break;
			case 2:c[i][j] = "   2"; break;
			case 4:c[i][j] = "   4"; break;
			case 8:c[i][j] = "   8"; break;
			}
		}
	}
	system("cls");
	for (i = 1; i <= 4; i++) {
		printf("+----+----+----+----+\n");
		for (j = 1; j <= 4; j++) {
			printf("|");
			if (cb[i][j] == 0) {
				printf("    ");
			}
			else {
				printf("%4d", cb[i][j]);
			}
		}
		printf("|\n");
	}
	printf("+----+----+----+----+\n");
	return true;
}

void spawn(int* c) {//如果死了之后程序仍然收到大量输入就会闪退 
	int position[7][7] = { 0 };
	int i, j, p, n = 0, x, y;
	for (i = 1; i <= 4; i++) {
		for (j = 1; j <= 4; j++) {
			if (*(c + (i * 7) + j) == 0) {
				n++;
				position[i][j] = n;
			}
		}
	}
	graphic(position);
	n++;
	srand((unsigned)time(NULL));
	p = rand() % n + 1;
	for (i = 1; i <= 4; i++) {
		for (j = 1; j <= 4; j++) {
			if (position[i][j] == p) {//bug是把所有的空格子赋成了2    已证实 
				*(c + (i * 7) + j) = 2; cout << p;
			}
		}
	}
}
char input(){
	char get=getchar();
	getchar();
	return get;
}