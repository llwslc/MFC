//define.h 
#ifndef CHESSMANRULES_H
#define CHESSMANRULES_H

#define START      1//开始
#define OVER       0//结束

#define BLACKCHESS 1//黑方
#define REDCHESS   2//红方

//--------棋子--------
#define NOCHESS    0 //没有棋子

#define B_GENERAL  1 //黑将
#define B_CHARIOT  2 //黑车
#define B_HORSE	   3 //黑马
#define B_CANON	   4 //黑炮
#define B_ADVISOR  5 //黑士
#define B_ELEPHANT 6 //黑相
#define B_SOLDIER  7 //黑兵

#define R_GENERAL  8 //红帅
#define R_CHARIOT  9 //红车
#define R_HORSE    10//红马
#define R_CANON    11//红炮
#define R_ADVISOR  12//红士
#define R_ELEPHANT 13//红象
#define R_SOLDIER  14//红卒

//初始化棋子位置
const BYTE InitChessMan[10][9] =
{
	{B_CHARIOT,B_HORSE,B_ELEPHANT,B_ADVISOR,B_GENERAL,B_ADVISOR,B_ELEPHANT,B_HORSE,B_CHARIOT},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{NOCHESS,B_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,B_CANON,NOCHESS},
	{B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER,NOCHESS,B_SOLDIER},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	//楚河                                            汉界//
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER,NOCHESS,R_SOLDIER},
	{NOCHESS,R_CANON,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,R_CANON,NOCHESS},
	{NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS,NOCHESS},
	{R_CHARIOT,R_HORSE,R_ELEPHANT,R_ADVISOR,R_GENERAL,R_ADVISOR,R_ELEPHANT,R_HORSE,R_CHARIOT}
};
//持子颜色

//判断棋子颜色
#define IsBlack(x) (x>=B_GENERAL && x<=B_SOLDIER)
#define IsRed(x)   (x>=R_GENERAL && x<=R_SOLDIER)

//判断两个棋子是不是同色
#define IsSameSide(x,y) ((IsBlack(x) && IsBlack(y)) || (IsRed(x) && IsRed(y)))

//棋子位置
struct ChessManPos
{
	BYTE x;
	BYTE y;
};

//棋子移动
struct ChessManMove
{
	BYTE ChessManID;
	POINT MovePoint;
};

//棋子走法
struct ChessManRule
{
	BYTE ChessManID;  //表明是什么棋子
	ChessManPos From;//起始位置
	ChessManPos To;  //走到什么位置
};

//悔棋时需要的数据结构
struct ChessManUndoMove
{
	ChessManMove cmChessMove;
	BYTE ChessManID;//被吃掉的棋子
};

#endif
