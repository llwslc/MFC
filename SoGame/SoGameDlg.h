
// SoGameDlg.h : 头文件
//

#pragma once


// CSoGameDlg 对话框
class CSoGameDlg : public CDialogEx
{
// 构造
public:
	CSoGameDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOGAME_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	int Player1StateOld;
	int Player1StateCur;
	int Player2StateOld;
	int Player2StateCur;
	int PlayerHP[2];
	double ButtleSpeedH;  //子弹水平速度
	double ButtleSpeedV;  //子弹竖直速度
	double ButtleX;  //子弹初始X坐标
	double ButtleY;  //子弹初始Y坐标
	BOOL ButtleFlag; //子弹玩家标志
	BOOL ButtleSpeedFlag; //子弹速度标志
	BOOL ButtleFireFlag; //子弹发射标志
	int ButtleFireTime;  //子弹发射时间
	int WindSpeed; //风速
	int WindSpeedTmp; //随机风速
	int GameGravity;  //重力
	

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void BulletPathWay();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void ButtleEdge(int x, int y);
};
