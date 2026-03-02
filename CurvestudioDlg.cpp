
// CurvestudioDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Curvestudio.h"
#include "CurvestudioDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCurvestudioDlg 대화 상자



CCurvestudioDlg::CCurvestudioDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CURVESTUDIO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCurvestudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_LIST, m_listPoints);
	DDX_Control(pDX, IDC_CANVAS, m_canvas);
}

BEGIN_MESSAGE_MAP(CCurvestudioDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_RAD_BEZIER, &CCurvestudioDlg::OnBnClickedRadBezier)
	ON_BN_CLICKED(IDC_RAD_NSPLINE, &CCurvestudioDlg::OnBnClickedRadNspline)
	ON_BN_CLICKED(IDC_RAD_BSPLINE, &CCurvestudioDlg::OnBnClickedRadBspline)
END_MESSAGE_MAP()


// CCurvestudioDlg 메시지 처리기

BOOL CCurvestudioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// ListCtrl을 그리드처럼 설정
	m_listPoints.SetExtendedStyle(
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
	);

	// 컬럼 추가
	m_listPoints.InsertColumn(0, L"Index", LVCFMT_LEFT, 60);
	m_listPoints.InsertColumn(1, L"X", LVCFMT_LEFT, 80);
	m_listPoints.InsertColumn(2, L"Y", LVCFMT_LEFT, 80);

	m_canvas.SetPoints(&m_points);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCurvestudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCurvestudioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CCurvestudioDlg::OnBnClickedRadBezier()
{
	m_curveType = CURVE_BEZIER;
	m_canvas.SetCurveType(CURVE_BEZIER);
	m_canvas.Invalidate();
}

void CCurvestudioDlg::OnBnClickedRadNspline()
{
	m_curveType = CURVE_NSPLINE;
	m_canvas.SetCurveType(CURVE_NSPLINE);
	m_canvas.Invalidate();
}

void CCurvestudioDlg::OnBnClickedRadBspline()
{
	m_curveType = CURVE_BSPLINE;
	m_canvas.SetCurveType(CURVE_BSPLINE);
	m_canvas.Invalidate();
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCurvestudioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCurvestudioDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcCanvas;
	m_canvas.GetWindowRect(&rcCanvas);
	ScreenToClient(&rcCanvas);

	if (rcCanvas.PtInRect(point))
	{
		int localX = point.x - rcCanvas.left;
		int localY = point.y - rcCanvas.top;

		m_points.push_back(PointD{ (double)localX, (double)localY });

		int idx = (int)m_points.size() - 1;

		CString s;
		int row = m_listPoints.InsertItem(idx, L"");

		s.Format(L"%d", idx);
		m_listPoints.SetItemText(row, 0, s);

		s.Format(L"%.0f", m_points[idx].x);
		m_listPoints.SetItemText(row, 1, s);

		s.Format(L"%.0f", m_points[idx].y);
		m_listPoints.SetItemText(row, 2, s);

		m_canvas.Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CCurvestudioDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		// 클릭이 캔버스에서 발생했는지 확인
		if (pMsg->hwnd == m_canvas.GetSafeHwnd())
		{
			// 캔버스 "클라이언트 좌표" (0,0이 캔버스 좌상단)
			CPoint pt(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam));

			// 점 저장
			m_points.push_back(PointD{ (double)pt.x, (double)pt.y });

			// 그리드 갱신
			int idx = (int)m_points.size() - 1;
			CString s;

			int row = m_listPoints.InsertItem(idx, L"");
			s.Format(L"%d", idx);
			m_listPoints.SetItemText(row, 0, s);

			s.Format(L"%.0f", m_points[idx].x);
			m_listPoints.SetItemText(row, 1, s);

			s.Format(L"%.0f", m_points[idx].y);
			m_listPoints.SetItemText(row, 2, s);
			
			//캔버스 다시그리기
			m_canvas.Invalidate();
			m_canvas.UpdateWindow();


			return TRUE; // 우리가 처리했으니 더 이상 전달 X
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
