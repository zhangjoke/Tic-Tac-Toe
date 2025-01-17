#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void drawBoard(char *board) //绘制棋盘
{
	printf("%c|%c|%c\n", board[7], board[8], board[9]);
	puts("-+-+-");
	printf("%c|%c|%c\n", board[4], board[5], board[6]);
	puts("-+-+-");
	printf("%c|%c|%c\n", board[1], board[2], board[3]);
	puts("-+-+-");
}

char inputPlayerLetter() //玩家选择棋子
{
	char letter;
	puts("你想用X还是O?");
	do{
		letter = toupper(getchar());
		if (letter == '\n')
			continue;
		if (letter != 'X' && letter != 'O')
			puts("你想用X还是O?");
	}while (letter != 'X' && letter != 'O');
	
	return letter;

}

void makeMove(char *board,char letter,int move) //落子
{
	board[move] = letter;
}

_Bool isWinner(char *bo, char le) //判定是否获胜
{
	return ((bo[7] == le && bo[8] == le && bo[9] == le) ||
		(bo[4] == le && bo[5] == le && bo[6] == le) ||
		(bo[1] == le && bo[2] == le && bo[3] == le) ||
		(bo[7] == le && bo[4] == le && bo[1] == le) ||
		(bo[8] == le && bo[5] == le && bo[2] == le) ||
		(bo[9] == le && bo[6] == le && bo[3] == le) ||
		(bo[7] == le && bo[5] == le && bo[3] == le) ||
		(bo[9] == le && bo[5] == le && bo[1] == le));
}

const char getBoardCopy(char *board) //复制棋盘，让电脑预判可能出现的情况
{
	char boardCopy[10];
	for (int i = 1; i < 10; i++)
		boardCopy[i] = board[i];
	return *boardCopy;
}

_Bool isSpaceFree(char *board,int move) //判断棋盘上是否为空
{
	return board[move] == ' ';
}

int getPlayerMove(char *board) //读取玩家棋子移动
{
	puts("你下一步走哪里？（1-9）");
	int move;
	do {
		move = getchar() - '0';
		if (move == '\n' - '0')
			continue;
		if (move < 1 || move > 9 || !isSpaceFree(board, move))
			puts("你下一步走哪里？（1-9）");
	}while (move < 1 || move > 9 || !isSpaceFree(board, move));
	return move;
}

int chooseRandomMoveFromList(char *board,char *movelist, int n) //随机读取计算机可移动的位置
{
	int possibleMove[4]; //每轮选择最多只有四个
	int j = 0;
	for (int i = 0; i < n; i++)
		if (isSpaceFree(board, movelist[i] - '0'))
			possibleMove[j++] = movelist[i] - '0';

	if (j != 0)
		return possibleMove[rand()%j];
	else
		return 0;
}

int getComputerMove(char board[], char computerLetter) //获得计算机的移动
{
	char playerLetter;
	char boardCopy[10];
	if (computerLetter == 'X') //根据计算机的棋子，判断玩家棋子
		playerLetter = 'O';
	else
		playerLetter = 'X';

	for (int i = 1; i < 10; i++){ //如果下一步可获胜，下那一步
		strcpy(boardCopy, board);
		if (isSpaceFree(boardCopy, i)) {
			makeMove(boardCopy, computerLetter,i);
				if (isWinner(boardCopy, computerLetter))
					return i;
		}
	}

	for (int i = 1; i < 10; i++){ //如果下一步玩家会获胜，占那个位置
		strcpy(boardCopy,board);
		if (isSpaceFree(boardCopy, i)) {
			makeMove(boardCopy, playerLetter,i);
				if (isWinner(boardCopy, playerLetter))
					return i;
		}
	}

	int move; //如果下一步不是决胜步
	move = chooseRandomMoveFromList(board, "1379", 4); //四个角优先
	if (move != 0)
		return move;

	move = chooseRandomMoveFromList(board, "5", 1); //中间
	if (move != 0)
		return move;
	
	return chooseRandomMoveFromList(board, "2468", 4); //剩下的位置
}

_Bool isBoardFull(char *board) //判断棋盘是否满了
{
	for (int i = 1; i < 10; i++)
		if (isSpaceFree(board, i))
			return 0;
	return 1;
}

_Bool isAgain() //再来一局
{
	char again;
	do{
		again = tolower(getchar());
		if (again == '\n')
			continue;
		if (again != 'n' && again != 'y')
			puts("请输入y或n。");
	}while (again != 'n' && again != 'y');
	
	if (again == 'y')
		return 1;
	else 
		return 0;
}


int main()
{
	puts("欢迎来玩井字棋！") ;

	while(1) { //游戏
		char theBoard[10];
		for (int i = 1; i < 10; i++) //将棋盘设为空白
			theBoard[i] = ' ';
		char playerLetter = inputPlayerLetter(); //获得玩家所选的棋子
		char computerLetter = (playerLetter == 'X')?'O': 'X' ;
		//获得计算机的棋子
		_Bool isTurnPlayer; //设定是否是玩家回合
		int move;
		srand((unsigned)time(NULL)); //随机先后手
		if (rand() % 2) {
			isTurnPlayer = 0;
			puts("电脑先走。");
		}else {
		       	isTurnPlayer = 1;
			puts("玩家先走。");
		}
		_Bool gameIsPlaying = 1; //设定游戏是否进行

		while (gameIsPlaying) {

			if (isTurnPlayer) { //如果是玩家回合
				drawBoard(theBoard);
				move = getPlayerMove(theBoard);
				makeMove(theBoard, playerLetter, move);

				if (isWinner(theBoard, playerLetter)) { //如果获胜
					drawBoard(theBoard);
					puts("太棒了！你获胜了！");
					gameIsPlaying = 0;
				}
			
				else { //如果平局
					if (isBoardFull(theBoard)) {
						drawBoard(theBoard);
						puts("平局了！");
						break;
					}
					else //设定轮到计算机
						isTurnPlayer = 0;
				}
			}

			else { //轮到计算机了
				move = getComputerMove(theBoard, computerLetter);
				makeMove(theBoard, computerLetter, move);
				if (isWinner(theBoard, computerLetter)) { //如果获胜
					drawBoard(theBoard);
					puts("电脑打败了你！你输了。");
					gameIsPlaying = 0;
				} else { //平局
					if (isBoardFull(theBoard)) {
						drawBoard(theBoard);
						puts("平局了！");
						break;
					} else // 设定轮到玩家
						isTurnPlayer = 1;
				}
			}
		}
		puts("再来一局？（yes或no）");
		if (!isAgain())
			break;
	}
	return 0;
}
