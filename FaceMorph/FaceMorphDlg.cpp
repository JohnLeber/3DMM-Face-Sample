
// FaceMorphDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FaceMorph.h"
#include "FaceMorphDlg.h"
#include "DxDlg.h"
#include "afxdialogex.h"
  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float g_fFOV = 12.5936f;// used to be 45.5f;
const float g_fAspect = 1.0f;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------//
CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}
//---------------------------------------------------------------//
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
//---------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFaceMorphDlg dialog


//---------------------------------------------------------------//
CFaceMorphDlg::CFaceMorphDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FACEMORPH_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, k, B, n, beta, C, n);
}
//---------------------------------------------------------------//
CFaceMorphDlg::~CFaceMorphDlg()
{
 
}
//---------------------------------------------------------------//
void CFaceMorphDlg::DoDataExchange(CDataExchange* pDX)
{
	for (int h = 0; h < NUM_SLIDERS; h++)
	{
		DDX_Control(pDX, IDC_SLIDER1 + h, m_Slider[h]);
	}
	CDialog::DoDataExchange(pDX);
}
//---------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFaceMorphDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_HSCROLL() 
	ON_WM_QUERYDRAGICON() 
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RESET, &CFaceMorphDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_CHECK1, &CFaceMorphDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()
//---------------------------------------------------------------//
// CFaceMorphDlg message handlers
BOOL CFaceMorphDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
 
	CRect rtFrame;
	GetDlgItem(IDC_FRAME)->GetWindowRect(rtFrame);
	ScreenToClient(rtFrame);

	for (int h = 0; h < NUM_SLIDERS; h++)
	{
		m_Slider[h].SetRange(0, 2 * SLIDER_CENTER);
		m_Slider[h].SetPos(SLIDER_CENTER);
	}
	m_pRenderWnd = new CDxWnd();
 
	BOOL b2 = m_pRenderWnd->Create(NULL, _T("DxWnd"), WS_VISIBLE, rtFrame, this, 1);
	 
	// Draw the background gradient.
	m_pRenderWnd->Initialize(this);
	m_pRenderWnd->MoveWindow(rtFrame);
	SetTimer(1, 50, NULL);
	 
	
 
	/*dgesvd("All", "All", &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork,
		&info);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}  
//---------------------------------------------------------------//
void CFaceMorphDlg::OnTimer(UINT_PTR nIdEvent)
{
	if (nIdEvent == 1)
	{
		if(m_pRenderWnd) m_pRenderWnd->Render();
	}
} 
//---------------------------------------------------------------//
void CFaceMorphDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//---------------------------------------------------------------//
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CFaceMorphDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	for (int h = 0; h < 20; h++)
	{
		m_Slider[h].SetRange(0, 2 * SLIDER_CENTER);
		m_Slider[h].SetPos(SLIDER_CENTER);
	}	
}
//---------------------------------------------------------------//
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFaceMorphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
} 
//---------------------------------------------------------------//
void CFaceMorphDlg::OnSlider()
{
	if (!m_pRenderWnd) return;
	for (int h = 0; h < NUM_SLIDERS; h++)
	{
		m_Scalars.m_Next[h] = (float)m_Slider[h].GetPos() - SLIDER_CENTER;
	}
	m_pRenderWnd->RecalcMesh(m_Scalars);
}
//---------------------------------------------------------------//
void CFaceMorphDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	for (int h = 0; h < NUM_SLIDERS; h++)
	{
		if (pSlider == &m_Slider[h])
		{
			OnSlider();
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//---------------------------------------------------------------//
void CFaceMorphDlg::OnClose()
{
	if (m_pRenderWnd)
	{
		m_pRenderWnd->ShutDown();
		m_pRenderWnd->DestroyWindow();
		delete m_pRenderWnd;
		m_pRenderWnd = 0;
	}

	CDialog::OnClose();
}
//---------------------------------------------------------------//
void CFaceMorphDlg::OnBnClickedReset()
{
	for (int h = 0; h < NUM_SLIDERS; h++)
	{
		m_Slider[h].SetPos(SLIDER_CENTER);
	};
	OnSlider();
}
//---------------------------------------------------------------//

void CFaceMorphDlg::OnBnClickedCheck1()
{
	if (!m_pRenderWnd) return;
	m_pRenderWnd->m_bWireframe = !m_pRenderWnd->m_bWireframe;
}
