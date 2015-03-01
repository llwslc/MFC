
// PlantsVsZombiesPlusDlg.h : 头文件
//

#pragma once


// CPlantsVsZombiesPlusDlg 对话框
class CPlantsVsZombiesPlusDlg : public CDialogEx
{
// 构造
public:
	CPlantsVsZombiesPlusDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLANTSVSZOMBIESPLUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	HWND GameHwnd;      //取得游戏句柄
	DWORD GameProcessId;  //取得游戏进程ID
	HANDLE gameProcess;   //读取游戏进程
	BOOL gameProcessFlag;  //判断是否开启成功

	CString GameRunFlagText;
	CRect GameRunFlagTextRect;
	bool GameRunFlagTextRectFlag;

	int *ZW_BASE;       //基址
	int nBase;		  //存储地址
	int nSun;			//阳光值
	int nMoney;			//金币
	int nRecharge;		//冷却
	int nFertilizer;	//肥料
	int nSpray;			//杀虫剂
	int nChocolate;		//巧克力
	int nTree;			//大树食物
	int nBottleT;		//罐子透明

	BOOL unlimitedSunFlag;    //判断是否开始阳光无限
	BOOL noRechargeFlag;      //判断是否开始没有冷却
	BOOL bottleTFlag;         //判断是否开始罐子透明


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	void initComboPlants();
	afx_msg void OnBnClickedButtonMission();
	afx_msg void OnBnClickedButtonPlants();
	afx_msg void OnBnClickedButtonGameBack();
	afx_msg void OnBnClickedButtonGlassHouse();
	afx_msg void OnBnClickedButtonRound();
};