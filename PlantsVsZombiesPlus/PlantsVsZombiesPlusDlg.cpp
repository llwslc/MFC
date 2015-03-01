
// PlantsVsZombiesPlusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlantsVsZombiesPlus.h"
#include "PlantsVsZombiesPlusDlg.h"
#include "afxdialogex.h"

#include <Windows.h>
#include <WinUser.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#pragma comment (lib, "Uxtheme.lib")
#pragma comment (lib, "dwmapi.lib")

#include <gdiplus.h>          // 导入gdiplus头文件
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;      // 使用Gdiplus命名空间，避免命名歧议
ULONG_PTR m_pGdiToken;

#define	unlimitedSunKey			 WM_USER+100
#define	unlimitedMoneyKey		 WM_USER+101
#define	noRechargeKey			 WM_USER+102
#define	unlimitedFertilizerKey	 WM_USER+103
#define	unlimitedSprayKey		 WM_USER+104
#define	unlimitedChocolateKey	 WM_USER+105 
#define	unlimitedTreeKey		 WM_USER+106
#define	bottleTransparentKey	 WM_USER+107


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//从资源加载图片
BOOL ImageFromIDResource(UINT resourceID, LPCTSTR resourceType, Image*& image)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(resourceID),resourceType);
	if (hRsrc == NULL)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (lpRsrc == NULL)
		return FALSE;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

	// load from stream
	image = Gdiplus::Image::FromStream(pstm);

	// free/release stuff
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	return TRUE;
}

//绘制发光字
void DrawGlowingText(HDC hDC, LPWSTR szText, RECT &rcArea,
	DWORD dwTextFlags = DT_LEFT | DT_VCENTER | DT_SINGLELINE, int iGlowSize = 10)
{
	//获取主题句柄
	HTHEME hThm = OpenThemeData(GetDesktopWindow(), L"TextStyle");
	//创建DIB
	HDC hMemDC = CreateCompatibleDC(hDC);
	BITMAPINFO bmpinfo = {0};
	bmpinfo.bmiHeader.biSize = sizeof(bmpinfo.bmiHeader);
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biWidth = rcArea.right - rcArea.left;
	bmpinfo.bmiHeader.biHeight = -(rcArea.bottom - rcArea.top);
	HBITMAP hBmp = CreateDIBSection(hMemDC, &bmpinfo, DIB_RGB_COLORS, 0, NULL, 0);
	if (hBmp == NULL) return;
	HGDIOBJ hBmpOld = SelectObject(hMemDC, hBmp);
	//绘制选项
	DTTOPTS dttopts = {0};
	dttopts.dwSize = sizeof(DTTOPTS);
	dttopts.dwFlags = DTT_GLOWSIZE | DTT_COMPOSITED;
	dttopts.iGlowSize = iGlowSize;  //发光的范围大小
	//绘制文本
	RECT rc = {0, 0, rcArea.right-rcArea.left, rcArea.bottom-rcArea.top};
	HRESULT hr = DrawThemeTextEx(hThm, hMemDC,
		TEXT_LABEL, 0, szText, -1, dwTextFlags , &rc, &dttopts);
	if(FAILED(hr)) return;
	BitBlt(hDC, rcArea.left, rcArea.top, rcArea.right - rcArea.left,
		rcArea.bottom - rcArea.top, hMemDC, 0, 0, SRCCOPY | CAPTUREBLT);
	//Clear
	SelectObject(hMemDC, hBmpOld);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
	CloseThemeData(hThm);
}


// CPlantsVsZombiesPlusDlg 对话框
CPlantsVsZombiesPlusDlg::CPlantsVsZombiesPlusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlantsVsZombiesPlusDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlantsVsZombiesPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlantsVsZombiesPlusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDCANCEL, &CPlantsVsZombiesPlusDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_BUTTONMISSION, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonMission)
	ON_BN_CLICKED(IDC_BUTTONPLANTS, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonPlants)
	ON_BN_CLICKED(IDC_BUTTONGAMEBACK, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonGameBack)
	ON_BN_CLICKED(IDC_BUTTONGLASSHOUSE, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonGlassHouse)
	ON_BN_CLICKED(IDC_BUTTONROUND, &CPlantsVsZombiesPlusDlg::OnBnClickedButtonRound)
END_MESSAGE_MAP()


// CPlantsVsZombiesPlusDlg 消息处理程序

BOOL CPlantsVsZombiesPlusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	//发光字透明代码
// 	DWM_BLURBEHIND dbb = {0};
// 	dbb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
// 	dbb.fEnable = true;
// 	dbb.hRgnBlur = CreateEllipticRgn(130,130,170,170);
// 	DwmEnableBlurBehindWindow(this->GetSafeHwnd(), &dbb);
	
	//设置透明色
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, 
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	SetLayeredWindowAttributes(0xEEEEE1, 255, LWA_ALPHA | LWA_COLORKEY);

	//GDI+的初始化
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	//游戏是否运行 static text 位置
	GameRunFlagTextRect.top = 125;
	GameRunFlagTextRect.left = 20;
	GameRunFlagTextRect.right = 170;
	GameRunFlagTextRect.bottom = 140;

	//设定基址的值
	ZW_BASE = (int*)0x6A9EC0;

	//设置外挂快捷键
	RegisterHotKey(this->GetSafeHwnd(), unlimitedSunKey, NULL, VK_NUMPAD1);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedMoneyKey, NULL, VK_NUMPAD2);
	RegisterHotKey(this->GetSafeHwnd(), noRechargeKey ,NULL, VK_NUMPAD3);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedFertilizerKey, NULL, VK_NUMPAD4);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedSprayKey, NULL, VK_NUMPAD5);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedChocolateKey, NULL, VK_NUMPAD6);
	RegisterHotKey(this->GetSafeHwnd(), unlimitedTreeKey, NULL, VK_NUMPAD7);
	RegisterHotKey(this->GetSafeHwnd(), bottleTransparentKey, NULL, VK_NUMPAD8);

	//初始化游戏进程,阳光无限,无冷却,罐子透明,跳关,花盆植物,游戏后台
	gameProcess = NULL;
	gameProcessFlag = FALSE;
	unlimitedSunFlag = FALSE;
	noRechargeFlag = FALSE;
	bottleTFlag = FALSE;

	initComboPlants();

	GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITMISSION)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONROUND)->EnableWindow(FALSE);

	SetTimer(1, 500, NULL);    //GameRunFlag位置
	SetTimer(2, 1000, NULL);   //GameRunFlag字符串
	SetTimer(3, 500, NULL);   //阳光无限
	SetTimer(4, 200, NULL);	  //无冷却
	SetTimer(5, 500, NULL);   //罐子透明

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPlantsVsZombiesPlusDlg::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	CRect PVZRect;
	GetClientRect(PVZRect);
	dc.FillSolidRect(PVZRect, 0xEEEEE1);  //窗口背景色

	//毛玻璃范围
	MARGINS mar={485,5,355,5};
	DwmExtendFrameIntoClientArea(this->GetSafeHwnd(), &mar);

	//发光字写入
//	RECT rcText1 = {30, 180, 400, 210};
//	DrawGlowingText(dc.m_hDC, L"这是 win7 的发光字诶~~能看出来吗？", rcText1, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 15);


	//绘制文字
	CFont GameRunFlagFont;
	GameRunFlagFont.CreateFont(40, 20, 0, 0, 400, FALSE, FALSE, FALSE, FALSE, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 3, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");

	CFont GameRunTextFont;
	GameRunTextFont.CreateFont(12, 6, 0, 0, 400, FALSE, FALSE, FALSE, TRUE, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, 3, DEFAULT_PITCH | FF_DONTCARE, L"宋体");

	CFont *OldFont;
	OldFont = dc.SelectObject(&GameRunFlagFont);

	CRect TextRect;

	TextRect.top = 90;
	TextRect.left = 0;
	TextRect.right = PVZRect.Width();
	TextRect.bottom = 120;
	DrawText(dc.m_hDC,
		L"------------------------------",
		-1,
		TextRect,
		DT_CENTER | DT_VCENTER);

	TextRect.top = 125;
	TextRect.left = 0;
	TextRect.right = PVZRect.Width();
	TextRect.bottom = 170;
	DrawText(dc.m_hDC,
		L"------------------------------",
		-1,
		TextRect,
		DT_CENTER | DT_VCENTER);

	dc.SelectObject(&GameRunTextFont);
	dc.SetTextColor(0x0000ff);
	DrawText(dc.m_hDC,
		GameRunFlagText,
		-1,
		GameRunFlagTextRect,
		DT_LEFT | DT_VCENTER);

// 	dc.SetTextColor(0x000000);
// 	TextRect.top = 295;
// 	TextRect.left = 250;
// 	TextRect.right = PVZRect.Width();
// 	TextRect.bottom = 450;
// 	DrawText(dc.m_hDC,
// 		L"    软件工程三级项目\n\n《植物大战僵尸》外挂\n\n",
// 		-1,
// 		TextRect,
// 		DT_LEFT | DT_VCENTER);

	dc.SelectObject(OldFont);

	//在面板上绘制图片
	Image *image;
	RectF imageRc;
	Graphics LogoGraphics(dc.m_hDC);

	ImageFromIDResource(IDR_PNG_PLANTS, L"PNG", image);
	imageRc.X = 15;
	imageRc.Y = 0;
	imageRc.Width = 85;
	imageRc.Height = 110;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_LOGO, L"PNG", image);
	imageRc.X = 110;
	imageRc.Y = 0;
	imageRc.Width = 425;
	imageRc.Height = 110;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_ZOMBIES, L"PNG", image);
	imageRc.X = 345;
	imageRc.Y = 165;
	imageRc.Width = 215;
	imageRc.Height = 255;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;

	ImageFromIDResource(IDR_PNG_UNLIMITED, L"PNG", image);
	imageRc.X = 15;
	imageRc.Y = 165;
	imageRc.Width = 220;
	imageRc.Height = 255;
	LogoGraphics.DrawImage(image,imageRc,0,0,imageRc.Width,imageRc.Height,UnitPixel);
	delete image;


	CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlantsVsZombiesPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlantsVsZombiesPlusDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	_CrtDumpMemoryLeaks();

	gameProcess = NULL;
	if (TRUE == gameProcessFlag)
		CloseHandle(gameProcess);

	UnregisterHotKey(this->GetSafeHwnd(), unlimitedSunKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedMoneyKey);
	UnregisterHotKey(this->GetSafeHwnd(), noRechargeKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedFertilizerKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedSprayKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedChocolateKey);
	UnregisterHotKey(this->GetSafeHwnd(), unlimitedTreeKey);
	UnregisterHotKey(this->GetSafeHwnd(), bottleTransparentKey);

	GdiplusShutdown(m_pGdiToken);

	CDialogEx::OnCancel();
}

void CPlantsVsZombiesPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		if (380 == GameRunFlagTextRect.left)
			GameRunFlagTextRectFlag = false;
		if (20 == GameRunFlagTextRect.left)
			GameRunFlagTextRectFlag = true;
		if (GameRunFlagTextRectFlag)
		{
			GameRunFlagTextRect.left += 10;
			GameRunFlagTextRect.right += 10;
		}
		else
		{
			GameRunFlagTextRect.left -= 10;
			GameRunFlagTextRect.right -= 10;
		}
		
		InvalidateRect(CRect(GameRunFlagTextRect.left-20,
							 GameRunFlagTextRect.top,
							 GameRunFlagTextRect.right+20,
							 GameRunFlagTextRect.bottom));
		break;

	case 2:
		GameHwnd = ::FindWindow(NULL,L"植物大战僵尸中文版");
		if (GameHwnd)
		{
			GameRunFlagText.Empty();
			GameRunFlagText = CString(L"The Game Is Running!");
			GetWindowThreadProcessId(GameHwnd, &GameProcessId);
			gameProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, GameProcessId);
			if (gameProcess)
				gameProcessFlag = TRUE;

			GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDITMISSION)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTONROUND)->EnableWindow(TRUE);
		}
		else
		{
			GameRunFlagText.Empty();
			GameRunFlagText = CString(L"The Game Is Not Running!");
			gameProcess = NULL;
			if (TRUE == gameProcessFlag)
				CloseHandle(gameProcess);

			GetDlgItem(IDC_BUTTONMISSION)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDITMISSION)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONPLANTS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBOPLANTS)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBOPLANTSRL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONGAMEBACK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONGLASSHOUSE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTONROUND)->EnableWindow(FALSE);
		}
		break;

	case 3:
		nSun = 9999;
		if (TRUE==unlimitedSunFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			for (int i=0; i<10; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x5560),&nSun,sizeof(int),0);
		}
		break;
	case 4:
		nRecharge = 1;
		if (TRUE==noRechargeFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x144),&nBase,sizeof(int),0);
			for (int i=0; i<10; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x50+i*0x50),&nRecharge,sizeof(int),0);
		}
		break;
	case 5:
		nBottleT = 100;
		if (TRUE==bottleTFlag && TRUE==gameProcessFlag)
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x768),&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x11C),&nBase,sizeof(int),0);
			for (int i=0; i<35; i++)
				WriteProcessMemory(gameProcess,(int *)(nBase+0x4C+i*0xEC),&nBottleT,sizeof(int),0);
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CPlantsVsZombiesPlusDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nHotKeyId)
	{
	case unlimitedSunKey:
		if (FALSE==unlimitedSunFlag && TRUE==gameProcessFlag)
			unlimitedSunFlag = TRUE;
		else if(TRUE==unlimitedSunFlag && TRUE==gameProcessFlag)
			unlimitedSunFlag = FALSE;
		break;
	case unlimitedMoneyKey:
		nMoney = 99999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x28),&nMoney,sizeof(int),0);
		break;
	case noRechargeKey:
		if (FALSE==noRechargeFlag && TRUE==gameProcessFlag)
			noRechargeFlag = TRUE;
		else if(TRUE==noRechargeFlag && TRUE==gameProcessFlag)
			noRechargeFlag = FALSE;
		break;
	case unlimitedFertilizerKey:
		nFertilizer = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x1F8),&nFertilizer,sizeof(int),0);
		break;
	case unlimitedSprayKey:
		nSpray = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x1FC),&nSpray,sizeof(int),0);
		break;
	case unlimitedChocolateKey:
		nChocolate = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x228),&nChocolate,sizeof(int),0);
		break;
	case unlimitedTreeKey:
		nTree = 10999;

		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x230),&nTree,sizeof(int),0);
		break;
	case bottleTransparentKey:
		if (FALSE==bottleTFlag && TRUE==gameProcessFlag)
			bottleTFlag = TRUE;
		else if(TRUE==bottleTFlag && TRUE==gameProcessFlag)
			bottleTFlag = FALSE;
		break;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CPlantsVsZombiesPlusDlg::initComboPlants()
{
	CString strTemp;
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->ResetContent();//消除现有所有内容

	strTemp.Format(L"00 豌豆射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"01 向日葵");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"02 樱桃炸弹");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"03 坚果");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"04 土豆雷");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"05 寒冰射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"06 大嘴花");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"07 双发射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"08 小喷菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"09 阳光菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"10 大喷菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"11 墓碑吞噬者");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"12 魅惑菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"13 胆小菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"14 寒冰菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"15 毁灭菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"16 睡莲");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"17 窝瓜");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"18 三线射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"19 缠绕海草");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"20 火爆辣椒");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"21 地刺");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"22 火炬树桩");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"23 高坚果");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"24 海蘑菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"25 路灯花");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"26 仙人掌");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"27 三叶草");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"28 裂荚射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"29 杨桃");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"30 南瓜头");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"31 磁力菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"32 卷心菜投手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"33 花盆");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"34 玉米投手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"35 咖啡豆");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"36 大蒜");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"37 叶子保护伞");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"38 金盏花");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"39 西瓜投手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"40 机枪射手");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"41 双子向日葵");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"42 忧郁蘑菇");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"43 香蒲");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"44 \"冰\"瓜");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"45 吸金磁");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"46 地刺王");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"47 玉米加农炮");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"48 模仿者");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"49 红坚果");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"50 巨坚果");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);
	strTemp.Format(L"51 花苗");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTS))->AddString(strTemp);

	strTemp.Format(L"向右");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTSRL))->AddString(strTemp);
	strTemp.Format(L"向左");
	((CComboBox*)GetDlgItem(IDC_COMBOPLANTSRL))->AddString(strTemp);

}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonMission()
{
	// TODO: 在此添加控件通知处理程序代码
	int nMission;
	CString sMission;

	GetDlgItem(IDC_EDITMISSION)->GetWindowTextW(sMission);
	nMission = _tstoi(LPCTSTR(sMission));	//非数字nMission赋值为0

	if (0==nMission || nMission>50)
		MessageBox(L"关数是从 1 到 50 的整数", L"Error", MB_ICONWARNING);
	else
	{
		if (IDOK == MessageBox(L"请确认当前游戏不在冒险模式中，否则会导致意外结束。\n如果进入冒险模式为之前未完成的关卡，请点击“重玩关卡”，即可！", L"跳关么", MB_OKCANCEL | MB_ICONWARNING))
		{
			ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
			WriteProcessMemory(gameProcess,(int *)(nBase+0x24),&nMission,sizeof(int),0);
		}
	}
}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonRound()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRound = 2;

	if (IDOK == MessageBox(L"请确认当前游戏不在冒险模式中，否则会导致意外结束。", L"跳入第二轮", MB_OKCANCEL | MB_ICONWARNING))
	{
		ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
		ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
		WriteProcessMemory(gameProcess,(int *)(nBase+0x2C),&nRound,sizeof(int),0);
	}
}

void CPlantsVsZombiesPlusDlg::OnBnClickedButtonPlants()
{
	// TODO: 在此添加控件通知处理程序代码
	int nAddPlantName = 0;
	CString sAddPlantName;

	GetDlgItem(IDC_COMBOPLANTS)->GetWindowTextW(sAddPlantName);
	nAddPlantName = _tstoi(LPCTSTR(sAddPlantName));

	if (nAddPlantName<0 || nAddPlantName>510)
	{
		MessageBox(L"没有这种植物", L"Error!", MB_ICONWARNING);
		return;
	}

	int nAddPlantRL = 0;
	CString sAddPlantRL;

	GetDlgItem(IDC_COMBOPLANTSRL)->GetWindowTextW(sAddPlantRL);
	if("向右" == sAddPlantRL)
		nAddPlantRL = 0;
	if("向左" == sAddPlantRL)
		nAddPlantRL = 1;

	int curPlantTotalNum = 0;
	BOOL curPlantPosition[4][8] = {{FALSE}};
	int addPlantPosition = 0;
	int addPlantLine = 0;
	int addPlantRow = 0;
	int addPlantColor = 0;
	int addPlantGrow = 3;


	ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);	//花盆个数
	for (int i=0; i<curPlantTotalNum; i++)
	{
		//花盆位置(0白天,1晚上,2小车,3水族馆)
		ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0xC+i*0x58),&addPlantPosition,sizeof(int),0);
		if (0 == addPlantPosition)
		{
			//花盆所在列,花盆所在行
			ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0x10+i*0x58),&addPlantLine,sizeof(int),0);
			ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0x14+i*0x58),&addPlantRow,sizeof(int),0);
			curPlantPosition[addPlantRow][addPlantLine] = TRUE;
		}
	}
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<8; j++)
		{
			if (FALSE == curPlantPosition[i][j])
			{
				addPlantRow = i;
				addPlantLine = j;

				i=5;
				break;
			}
			if (3==i && 7==j)
			{
				MessageBox(L"温室植物已满,请移动或卖掉一些植物!", L"失败", MB_ICONWARNING);
				return;
			}
		}
	}
	int addPlantTotalNum = curPlantTotalNum + 1;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350),&addPlantTotalNum,sizeof(int),0);	//花盆个数
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x08+curPlantTotalNum*0x58),&nAddPlantName,sizeof(int),0);	//花盆种类
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x10+curPlantTotalNum*0x58),&addPlantLine,sizeof(int),0);	//花盆所在列
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x14+curPlantTotalNum*0x58),&addPlantRow,sizeof(int),0);		//花盆所在行
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x18+curPlantTotalNum*0x58),&nAddPlantRL,sizeof(int),0);		//花盆方向(0正,1反)
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x2C+curPlantTotalNum*0x58),&addPlantGrow,sizeof(int),0);	//花盆状态(0幼苗,1小花,2中花,3大花)
	
	int tmp = 1324960763;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x20+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//未知
	tmp = 4;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x34+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//未知
	tmp = 1324974559;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x40+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//未知
	tmp = 1317389145;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x48+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//未知
	tmp = 1318828743;
	WriteProcessMemory(gameProcess,(int *)(nBase+0x350+0x50+curPlantTotalNum*0x58),&tmp,sizeof(int),0);	//巧克力

	CString successAddPlant = CString("成功添加\"") + sAddPlantName + CString("\",刷新花园后看到!");
	MessageBox(successAddPlant, L"成功");
}


void CPlantsVsZombiesPlusDlg::OnBnClickedButtonGameBack()
{
	// TODO: 在此添加控件通知处理程序代码
	// 将内存中 0054EBA8 - 74 2E - je 0054EBD8
	// 换成	   0054EBA8 - EB 00 - jmp 0054EBAA
	// 那么偏移就是0054EBAA-0054EBA8-2 (占两个字节)

	CString enableGameBack;
	GetDlgItem(IDC_BUTTONGAMEBACK)->GetWindowTextW(enableGameBack);

	byte changeA;
	byte changeB;

	if ("开启后台运行" == enableGameBack)
	{
		GetDlgItem(IDC_BUTTONGAMEBACK)->SetWindowTextW(L"禁用后台运行");

		changeA = 0xEB;
		changeB = 0x00;

		WriteProcessMemory(gameProcess,(int *)(0x0054EBA8),&changeA,sizeof(byte),0);
		WriteProcessMemory(gameProcess,(int *)(0x0054EBA9),&changeB,sizeof(byte),0);
	}
	if ("禁用后台运行" == enableGameBack)
	{
		GetDlgItem(IDC_BUTTONGAMEBACK)->SetWindowTextW(L"开启后台运行");

		changeA = 0x74;
		changeB = 0x2E;

		WriteProcessMemory(gameProcess,(int *)(0x0054EBA8),&changeA,sizeof(byte),0);
		WriteProcessMemory(gameProcess,(int *)(0x0054EBA9),&changeB,sizeof(byte),0);
	}
}


void CPlantsVsZombiesPlusDlg::OnBnClickedButtonGlassHouse()
{
	// TODO: 在此添加控件通知处理程序代码
	int curPlantTotalNum = 0;
	int plantPosition = 0;
	int glassHousePlantTotalNum = 0;

	ReadProcessMemory(gameProcess,ZW_BASE,&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x82C),&nBase,sizeof(int),0);
	ReadProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);	//花盆个数
	for (int i=0; i<curPlantTotalNum; i++)
	{
		//花盆位置(0白天,1晚上,2小车,3水族馆)
		ReadProcessMemory(gameProcess,(int *)(nBase+0x350+0xC+i*0x58),&plantPosition,sizeof(int),0);
		if (0 == plantPosition)
			glassHousePlantTotalNum++;

	}
	if (IDOK == MessageBox(L"此操作会清空你温室中的植物,确定?", L"删除么", MB_OKCANCEL | MB_ICONWARNING))
	{
		curPlantTotalNum = curPlantTotalNum - glassHousePlantTotalNum;
		WriteProcessMemory(gameProcess,(int *)(nBase+0x350),&curPlantTotalNum,sizeof(int),0);
		MessageBox(L"刷新花园后清空温室植物!", L"清空成功");
	}
}
