
// ChineseChessMFCDlg.h : 头文件
//

#include "ChessManRules.h"
#pragma once

struct ChessBoard
{
	int ChessBoardX, ChessBoardY, ChessBoardWide, 
		ChessBoardFocusXFlag, ChessBoardFocusYFlag,
		ChessBoardFocusLong, ChessBoardFocusDistance;

	CImageList ChessMan;       //棋子图片加载
	CBitmap ChessManBmp;   //加载位图

	BYTE ShowChessMan[10][9];    //移动棋子后

	ChessManMove CMMove;

	ChessManRule CMRule;
	BYTE nFlag;
	BYTE nChessMan; //间隔的棋子数

	ChessBoard()
	{
		ChessBoardX = 30;
		ChessBoardY = 50;
		ChessBoardWide = 50;
		ChessBoardFocusXFlag = 1;
		ChessBoardFocusYFlag = 1;
		ChessBoardFocusLong = 15;
		ChessBoardFocusDistance = 5;

		//创建一个包含14个24位色40x40图片的ImageList,ILC_MASK的意思是同时创建一个 mask,这样在下面指定了背景颜色以后ImageList就可以画透明图像了  
		ChessMan.Create(40, 40, ILC_COLOR24|ILC_MASK, 14, 14);
		ChessManBmp.LoadBitmap(IDB_CHESSMAN);
		//将图片加到ImageList中
		ChessMan.Add(&ChessManBmp, RGB(0,255,0));

		memcpy(ShowChessMan, InitChessMan, 90);//初始化棋盘

		CMMove.ChessManID = NOCHESS;
		CMRule.ChessManID = NOCHESS;
		nFlag = FALSE;
		nChessMan = 0;

	}

	//绘制棋盘
	void DrawChessBoard(CPaintDC* dc)
	{
		CFont RiverFont;
		RiverFont.CreatePointFont(250,L"隶书");  

		CFont *OldFont;  
		OldFont = dc->SelectObject(&RiverFont);
		
		int n, i,j;
		//棋盘上半部分
		for (i=0; i<9; i++)
		{
			dc->MoveTo(ChessBoardX+i*ChessBoardWide, ChessBoardY);
			dc->LineTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+4*ChessBoardWide);
		}
		for (j=0; j<5; j++)
		{
			dc->MoveTo(ChessBoardX, ChessBoardY+j*ChessBoardWide);
			dc->LineTo(ChessBoardX+8*ChessBoardWide, ChessBoardY+j*ChessBoardWide);
		}
		//楚河汉界部分
		dc->SelectObject(RiverFont);
		dc->TextOut(ChessBoardX+5,ChessBoardY+4*ChessBoardWide+10,L"楚 河             汉 界");
		dc->SelectObject(OldFont);
		for (i=0; i<2; i++)
		{
			dc->MoveTo(ChessBoardX+i*8*ChessBoardWide, ChessBoardY+4*ChessBoardWide);
			dc->LineTo(ChessBoardX+i*8*ChessBoardWide, ChessBoardY+5*ChessBoardWide);
		}
		//棋盘下半部分
		for (i=0; i<9; i++)
		{
			dc->MoveTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+5*ChessBoardWide);
			dc->LineTo(ChessBoardX+i*ChessBoardWide, ChessBoardY+9*ChessBoardWide);
		}
		for (j=0; j<5; j++)
		{
			dc->MoveTo(ChessBoardX, ChessBoardY+(j+5)*ChessBoardWide);
			dc->LineTo(ChessBoardX+8*ChessBoardWide, ChessBoardY+(j+5)*ChessBoardWide);
		}
		//将帅田字格
		for (i=0; i<2; i++)
		{
			for (j=0; j<2; j++)
			{
				dc->MoveTo(ChessBoardX+(3+j*2)*ChessBoardWide, ChessBoardY+i*7*ChessBoardWide);
				dc->LineTo(ChessBoardX+(5-j*2)*ChessBoardWide, ChessBoardY+(i*7+2)*ChessBoardWide);
			}
		}
		//炮台焦点
		for (i=0; i<2; i++)
		{
			for (n=0; n<2; n++)
			{
				for (j=0; j<4; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = 1;
						break;
					case 2:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = -1;
						break;
					case 3:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+(i*6+1)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*5+2)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
			}
		}
		//兵卒焦点
		for (i=0; i<4; i++)
		{
			for (n=0; n<2; n++)
			{
				for (j=0; j<2; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = -1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+(i*2+2)*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
				for (j=0; j<2; j++)
				{
					switch (j)
					{
					case 0:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = -1;
						break;
					case 1:
						ChessBoardFocusXFlag = 1;
						ChessBoardFocusYFlag = 1;
						break;
					}
					dc->MoveTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance+ChessBoardFocusYFlag*ChessBoardFocusLong);
					dc->LineTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
					dc->LineTo(ChessBoardX+i*2*ChessBoardWide+ChessBoardFocusXFlag*ChessBoardFocusDistance+ChessBoardFocusXFlag*ChessBoardFocusLong, ChessBoardY+(n*3+3)*ChessBoardWide+ChessBoardFocusYFlag*ChessBoardFocusDistance);
				}
			}
		}
	}
	//绘制棋子
	void DrawChessMan(CPaintDC* dc)
	{
		int i, j;
		POINT pt;

		for (i=0; i<10; i++)
		{
			for (j=0; j<9; j++)
			{
				if(ShowChessMan[i][j]==NOCHESS)
					continue;

				pt.x = 10+j*ChessBoardWide;
				pt.y = 30+i*ChessBoardWide;
				ChessMan.Draw(dc, ShowChessMan[i][j]-1, pt, ILD_TRANSPARENT);
			}
		}

		//绘制用户正在拖动的棋子
		if(CMMove.ChessManID != NOCHESS)
			ChessMan.Draw(dc, CMMove.ChessManID-1, CMMove.MovePoint, ILD_TRANSPARENT);
	}

	//验证棋子移动的合法性
	BYTE ValidMove()
	{
		int x, y;
		switch (CMRule.ChessManID)
		{			      
		case B_GENERAL:     //黑将
			//遍历竖线上的棋子
			for (y=0; y<10; y++)
			{
				//没有棋子,继续遍历
				if (ShowChessMan[y][CMRule.From.x] == NOCHESS)
					continue;
				//有棋子,分为红帅和其他棋子
				else
					//如果是红帅
					if (ShowChessMan[y][CMRule.From.x] == R_GENERAL)
						if ((CMRule.To.x==CMRule.From.x && CMRule.To.y==y) //可以吃掉红帅,或者在黑将田字格内移动
							|| (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
							&& ((CMRule.To.x==CMRule.From.x && abs(CMRule.To.y-CMRule.From.y)==1)
							|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1))))
							nFlag = TRUE;
						else
							nFlag = FALSE;
					else
						//竖线上有其他棋子,只能在黑将田字格内移动
						if (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
							&& ((CMRule.To.x==CMRule.From.x && abs(CMRule.To.y-CMRule.From.y)==1)
							|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1)))
							nFlag = TRUE;
						else
							nFlag = FALSE;
			}
			break;
 		case B_CHARIOT:     //黑车
			if (CMRule.To.x==CMRule.From.x || CMRule.To.y==CMRule.From.y)
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_HORSE:       //黑马
			if ((abs(CMRule.To.x-CMRule.From.x)==1 && abs(CMRule.To.y-CMRule.From.y)==2)
				|| (abs(CMRule.To.x-CMRule.From.x)==2 && abs(CMRule.To.y-CMRule.From.y)==1))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_CANON:       //黑炮
			if (CMRule.To.x==CMRule.From.x || CMRule.To.y==CMRule.From.y)
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
		case B_ADVISOR:     //黑士
			if (CMRule.To.x>2 && CMRule.To.x<6 && CMRule.To.y > -1 && CMRule.To.y<3
				&& (abs(CMRule.To.x-CMRule.From.x)==1 && abs(CMRule.To.y-CMRule.From.y)==1))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
		case B_ELEPHANT:    //黑相
			if (CMRule.To.x>-1 && CMRule.To.x<9 && CMRule.To.y > -1 && CMRule.To.y<5
				&& (abs(CMRule.To.x-CMRule.From.x)==2 && abs(CMRule.To.y-CMRule.From.y)==2))
				nFlag = TRUE;
			else
				nFlag = FALSE;
			break;
 		case B_SOLDIER:     //黑兵
			if (CMRule.From.y>2 && CMRule.From.y<5)
				if (CMRule.To.x==CMRule.From.x && CMRule.To.y-CMRule.From.y==1)
					nFlag = TRUE;
				else
					nFlag = FALSE;
			else
				if ((CMRule.To.x==CMRule.From.x && CMRule.To.y-CMRule.From.y==1)
					|| (CMRule.To.y==CMRule.From.y && abs(CMRule.To.x-CMRule.From.x)==1))
					nFlag = TRUE;
				else
					nFlag = FALSE;
			break;


// 		case R_GENERAL:     //红帅
// 		case R_CHARIOT:     //红车
// 		case R_HORSE:       //红马
// 		case R_CANON:       //红炮
// 		case R_ADVISOR:     //红士
// 		case R_ELEPHANT:    //红象
// 		case R_SOLDIER:     //红卒
			break;
		}

		return nFlag;
	}
};

// CChineseChessMFCDlg 对话框
class CChineseChessMFCDlg : public CDialogEx
{
	// 构造
public:
	CChineseChessMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_CHINESECHESSMFC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	ChessBoard DrawCB;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};