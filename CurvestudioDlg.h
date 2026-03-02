
// CurvestudioDlg.h: 헤더 파일
//
#include <vector>
#pragma once
#include "CanvasStatic.h"


struct PointD
{
	double x;
	double y;
};

enum CurveType
{
	CURVE_NONE = 0,
	CURVE_BEZIER,
	CURVE_NSPLINE,
	CURVE_BSPLINE
};

// CCurvestudioDlg 대화 상자
class CCurvestudioDlg : public CDialogEx
{
// 생성입니다.
public:
	CCurvestudioDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
private:
	std::vector<PointD> m_points;  // 점 목록
	CurveType m_curveType = CURVE_NONE;
	bool m_dragging = false;
	int m_dragIndex = -1;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CURVESTUDIO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadNspline();
	afx_msg void OnBnClickedRadBezier();
	afx_msg void OnBnClickedRadBspline();
	CListCtrl m_listPoints;
	CCanvasStatic m_canvas;
};
