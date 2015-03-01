
// ChineseChessMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChineseChessMFC.h"
#include "ChineseChessMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//判断开始
BYTE StartFlag = START;
BYTE WhichSide = REDCHESS;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChineseChessMFCDlg 对话框




CChineseChessMFCDlg::CChineseChessMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChineseChessMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChineseChessMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CChineseChessMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CChineseChessMFCDlg 消息处理程序

BOOL CChineseChessMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, 
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	HINSTANCE hInstUser32 = LoadLibrary(L"user32.dll"); 
	if(hInstUser32) 
	{ 
		typedef BOOL (WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);
		SLWA SLWAFunc= NULL;
		//取得SetLayeredWindowAttributes函数指针 
		SLWAFunc=(SLWA)GetProcAddress(hInstUser32, "SetLayeredWindowAttributes");
		if(SLWAFunc)SLWAFunc(this->GetSafeHwnd(), 0xABCDEF, 255, LWA_ALPHA | LWA_COLORKEY);
		FreeLibrary(hInstUser32);
	}

	HINSTANCE hInstDwmapi = LoadLibrary(L"dwmapi.dll");
	MARGINS mar={-1};
	if(hInstDwmapi) 
	{ 
		typedef BOOL (WINAPI *DEFICA)(HWND,MARGINS*);
		DEFICA DEFICAFunc= NULL;
		//取得DwmExtendFrameIntoClientArea函数指针 
		DEFICAFunc=(DEFICA)GetProcAddress(hInstDwmapi, "DwmExtendFrameIntoClientArea");
		if(DEFICAFunc)DEFICAFunc(this->GetSafeHwnd(),&mar);
		FreeLibrary(hInstDwmapi);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChineseChessMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChineseChessMFCDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	CRect rcGlassArea;
	GetClientRect ( rcGlassArea );
	dc.FillSolidRect(rcGlassArea, 0xABCDEF);  //窗口背景色

	DrawCB.DrawChessBoard(&dc);
	DrawCB.DrawChessMan(&dc);

	CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChineseChessMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CChineseChessMFCDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	StartFlag = START;

	int x, y;
	//将坐标换算成棋盘上的格子
	x = (point.x-10)/DrawCB.ChessBoardWide;
	y = (point.y-30)/DrawCB.ChessBoardWide;
	//判断鼠标是否在棋盘内，并且点中了用户棋子
	if(x>=0 && x<9 && y>=0 && y<10 )
	{
		//将当前棋子的信息装入，记录移动棋子的结构中
		DrawCB.CMMove.ChessManID = DrawCB.ShowChessMan[y][x];

		//将当前棋子的信息装入,记录合法移动棋子的结构中
		DrawCB.CMRule.From.x = x;
		DrawCB.CMRule.From.y = y;
		DrawCB.CMRule.ChessManID = DrawCB.ShowChessMan[y][x];

		//将该棋子原位置棋子去掉
		DrawCB.ShowChessMan[y][x] = NOCHESS;

		//让棋子中点坐标位于鼠标所在点
		point.x -= 20;
		point.y -= 20;
		DrawCB.CMMove.MovePoint = point;
		//重绘屏幕
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		SetCapture();//独占鼠标焦点
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CChineseChessMFCDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x, y;
	//将坐标换算成棋盘上的格子
	x = (point.x-10)/DrawCB.ChessBoardWide;
	y = (point.y-30)/DrawCB.ChessBoardWide;

	//将移动后的位置装入,记录合法移动棋子的结构体
	DrawCB.CMRule.To.x = x;
	DrawCB.CMRule.To.y = y;

	//判断是否有移动棋子，并且该棋子的移动是一个合法走法
	if(DrawCB.CMMove.ChessManID && DrawCB.ValidMove())
	{
		DrawCB.ShowChessMan[y][x] = DrawCB.CMMove.ChessManID;
	}
	else
	{
		DrawCB.ShowChessMan[DrawCB.CMRule.From.y][DrawCB.CMRule.From.x] = DrawCB.CMMove.ChessManID;
	}

	DrawCB.CMMove.ChessManID = NOCHESS;//将移动的棋子清空

	//重绘屏幕  
	InvalidateRect(NULL,FALSE);
	UpdateWindow();	
	ReleaseCapture();//释放鼠标焦点

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CChineseChessMFCDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(DrawCB.CMMove.ChessManID)
	{
		//防止将棋子拖出棋盘
		if(point.x < 30)//左边
			point.x = 30;
		if(point.y < 50)//上边
			point.y = 50;
		if(point.x > 430)//右边
			point.x = 430;
		if(point.y > 500)//下边
			point.y = 500;

		//让棋子中心位于鼠标所在处
		point.x -= 20;
		point.y -= 20;

		DrawCB.CMMove.MovePoint = point;//保存移动棋子的坐标	

		InvalidateRect(NULL,FALSE);//刷新窗口
		UpdateWindow();//立即执行刷新
	}
	CDialogEx::OnMouseMove(nFlags, point);
}
