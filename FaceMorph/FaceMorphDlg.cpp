
// FaceMorphDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "FaceMorph.h"
#include "FaceMorphDlg.h"
#include "FileHandle.h"
#include "DxDlg.h"
#include "afxdialogex.h"
#include <Eigen/Core>
#include <Eigen/SVD>
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
const int LINE_BUFF_SIZE = 4096;
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
	ON_BN_CLICKED(IDC_EXPORT, &CFaceMorphDlg::OnBnClickedExport)
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
 
	SetDlgItemText(IDC_EXPORT_PATH, L"C:\\_3DMMExport");

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
	 
	//calculate the eigenvalues here and pass them to CDxDlg
	//first we need to find the Meshess folder//
	//This will be at a higher level. Recursively search until we find it
	CString strMeshPath;
	TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
	DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 0, MAX_PATH);
	strMeshPath = CString(szFilePath);

	long nRight = strMeshPath.ReverseFind(_T('\\'));//remove exe name
	strMeshPath = strMeshPath.Left(nRight);//this will ne the same folder as the EXe
	bool bFoundMeshFolder = false;
	int nNumAttempts = 0;
	while (!bFoundMeshFolder && nNumAttempts < 3)
	{
		nRight = strMeshPath.ReverseFind(_T('\\'));//move one folder up
		strMeshPath = strMeshPath.Left(nRight);//this will ne the same folder as 
		nNumAttempts++;
		if (DoesDirExist(strMeshPath + L"\\Meshes")) {
			bFoundMeshFolder = true;
			strMeshPath.Append(L"\\Meshes\\");
		}		 
		
	}
	if (bFoundMeshFolder)
	{
		CalcEigenValues(strMeshPath);
	}
	else
	{
		AfxMessageBox(L"Failed to find Mesh Folder");
	}

	// Draw the background gradient.
	m_pRenderWnd->Initialize(this);
	m_pRenderWnd->MoveWindow(rtFrame);
	SetTimer(1, 50, NULL);
	 
 

	/*dgesvd("All", "All", &m, &n, a, &lda, s, u, &ldu, vt, &ldvt, work, &lwork,
		&info);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}  
//---------------------------------------------------------------//
bool CFaceMorphDlg::CalcEigenValues(CString strMeshPath)
{
	if (!m_pRenderWnd)
	{
		AfxMessageBox(L"m_pRenderWnd not created");
		return false;
	}
	std::vector<std::vector<float> > vMeshes;

	 
	int nNumVertices = -1;
	int nI1 = 0;
	int nI2 = 0;
	int nI3 = 0;

	////////////////////////////////////////////////////////////////////////////////////////
	//STEP ONE: load the files
	////////////////////////////////////////////////////////////////////////////////////////
	CFileFind finder;
	//count the number of files we will have to load
	int nNumFiles = 0;
	BOOL bWorking = finder.FindFile(strMeshPath + _T("*.mesh"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		nNumFiles++;
	}
	finder.Close(); 

	int nFileCounter = 0;
	bWorking = finder.FindFile(strMeshPath + _T("*.mesh"));
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!bWorking) {
			long ert = 0;
		}
		CString strPath = finder.GetFilePath();  
		int nCounter = 0; 
		CFileHandle file(strPath, TEXT("r"));
		if (file.GetFile() != 0)
		{
			//cound the vertices in advance and make sure they fit the vector
			while (!feof(file.GetFile()))
			{
				CHAR buffer[LINE_BUFF_SIZE];
				fgets(buffer, LINE_BUFF_SIZE, file.GetFile());
				if (buffer[0] == 'v')
				{
					nCounter++;
				}
			}
			if (nNumVertices == -1)
			{
				nNumVertices = nCounter;
			}
			else if (nNumVertices != nCounter)
			{
				finder.Close();
				AfxMessageBox(L"wrong size");
				return false;
			}
			if (nFileCounter == 0)
			{
				m_pRenderWnd->m_nNumVertices = nNumVertices;
				//we have enough info now to preallocate memory to avoid performance penalties caused by dynamic memory allocations...
				vMeshes.reserve(nNumFiles);
				vMeshes.resize(nNumFiles);
				int nNumVerticesXYZ = nNumVertices * 3;
				for (int h = 0; h < nNumFiles; h++)
				{
					vMeshes[h].reserve(nNumVerticesXYZ);
					vMeshes[h].resize(nNumVerticesXYZ);
				}

				m_pRenderWnd->m_AveFace.reserve(nNumVerticesXYZ);
				m_pRenderWnd->m_AveFace.resize(nNumVerticesXYZ);
				m_pRenderWnd->m_Mesh.reserve(nNumVerticesXYZ);
				m_pRenderWnd->m_Mesh.resize(nNumVerticesXYZ);
			}
			rewind(file.GetFile());
			int nIndex = 0;
			float x = 0;
			float y = 0;
			float z = 0;
			while (!feof(file.GetFile()))
			{
				CHAR buffer[LINE_BUFF_SIZE];
				fgets(buffer, LINE_BUFF_SIZE, file.GetFile());
				if (0 == strncmp("v ", buffer, 2))
				{
					//load vertices into the vectors
					sscanf_s(buffer + 1, "%f %f %f", &x, &y, &z);
					m_pRenderWnd->m_AveFace[nIndex] += x;
					m_pRenderWnd->m_AveFace[nIndex + nNumVertices] += y;
					m_pRenderWnd->m_AveFace[nIndex + 2 * nNumVertices] += z;
					vMeshes[nFileCounter][nIndex] = x;
					vMeshes[nFileCounter][nIndex + nNumVertices] = y;
					vMeshes[nFileCounter][nIndex + 2 * nNumVertices] = z;
					nIndex++;
				}
				else if (nFileCounter == 0 && 0 == strncmp("f ", buffer, 2))
				{
					//load face information (list of three points forming a triangle). Do it for the first mesh only as the rest should be the same
					sscanf_s(buffer + 1, "%d %d %d", &nI1, &nI2, &nI3);
					m_pRenderWnd->m_vIndices.push_back(nI1 - 1);
					m_pRenderWnd->m_vIndices.push_back(nI2 - 1);
					m_pRenderWnd->m_vIndices.push_back(nI3 - 1);
				}
			}
			nFileCounter++; 
		}
	}
	//finder.Close(); //note, CFileFinder calls Close on destructor, so we don't need to call this here

	////////////////////////////////////////////////////////////////////////////////////////
	//STEP TWO: calculate the mean/average face mesh
	////////////////////////////////////////////////////////////////////////////////////////
	for_each(m_pRenderWnd->m_AveFace.begin(), m_pRenderWnd->m_AveFace.end(), [nNumFiles](float& x) {x = x / nNumFiles; });

	////////////////////////////////////////////////////////////////////////////////////////
	//STEP THREE: subtract the mean from each face mesh
	////////////////////////////////////////////////////////////////////////////////////////
	auto t = m_pRenderWnd->m_AveFace;
	for (auto& m : vMeshes)
	{
		int j = 0;
		for_each(m.begin(), m.end(), [&j, &t](float& x) {x = x - t[j]; j++; });
	}
	/*for (int h = 0; h < nNumFiles; h++)//older style for loop - but is it easier to read??
	{
		for (int j = 0; j < nNumVertices * 3; j++){
			vMeshes[h][j] = vMeshes[h][j] - m_pRenderWnd->m_AveFace[j];
		}
	}*/
	////////////////////////////////////////////////////////////////////////////////////////
	//STEP FOUR: calculate the eigenvectors
	////////////////////////////////////////////////////////////////////////////////////////
	int nNumVerticesXYZ = nNumVertices * 3;
	Eigen::MatrixXf X(nNumVerticesXYZ, nNumFiles);
	int h = 0;
	for (auto& m : vMeshes)
	{
		int j = 0;
		for_each(m.begin(), m.end(), [&j, h, &X](float& x) {X(j, h) = x; j++; });
		h++;
	}
	/*for (int h = 0; h < nNumFiles; h++)//older style for loop - is this easier to read??
	{
		for (int j = 0; j < nNumVerticesXYZ; j++) {
			X(j, h) = vMeshes[h][j];
		}
	}*/

	//Use Principle Component Analysis. SVD routine in the Eigen library to do this
	Eigen::JacobiSVD<Eigen::MatrixXf> svd(X, Eigen::ComputeThinU | Eigen::ComputeThinV);
	Eigen::MatrixXf U = svd.matrixU();
	Eigen::MatrixXf V = svd.matrixV();
	Eigen::VectorXf s = svd.singularValues();
 
	if (s.rows() < NUM_EIGENS)
	{
		CString strMsg;
		strMsg.Format(L"Not enough eigenvectors found. We need at least %d. Please increase the number of meshes and restart the application.", NUM_EIGENS);
		AfxMessageBox(strMsg);
		return false;
	}

	//There will be nNumFiles eigenvectors, but only take the first 20 (NUM_EIGENS)
	m_pRenderWnd->m_Eigen.reserve(NUM_EIGENS);
	m_pRenderWnd->m_Eigen.resize(NUM_EIGENS);
	for (int h = 0; h < NUM_EIGENS; h++)
	{
		m_pRenderWnd->m_Eigen[h].reserve(nNumVerticesXYZ);
		m_pRenderWnd->m_Eigen[h].resize(nNumVerticesXYZ);
		int j = 0;
		for_each(m_pRenderWnd->m_Eigen[h].begin(), m_pRenderWnd->m_Eigen[h].end(), [&j, h, &U](float& x) {x = U(j, h); j++; });
		/*for (int j = 0; j < nNumVerticesXYZ; j++) 	{//older style, again, isn't it easier to read?
			m_pRenderWnd->m_Eigen[h][j] = U(j, h);
		}*/
	}
	//We now have the eigenvectors. The mesh that is rendered will be the average/mean mesh plus a linear combination of the 
	//the eignevector meshes (m_Eigen). That is each eigenvector is multipled by the scalar corresponding to each slider in the UI.
	return true;
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
//-------------------------------------------------------------------------------------//
bool CFaceMorphDlg::DoesDirExist(CString str)
{
	DWORD dwRetVal = GetFileAttributes(str);
	if (dwRetVal == 0xFFFFFFFF)
	{
		return false;
	}
	else if (dwRetVal & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------//
void CFaceMorphDlg::OnBnClickedExport()
{ 
	CString strLine;
	CString strExportPath;
	CWaitCursor wait;

	GetDlgItemText(IDC_EXPORT_PATH, strExportPath);
	if (!DoesDirExist(strExportPath))
	{
		AfxMessageBox(L"Folder doesn't exist!");
		return;
	}
	//export eigenvalues 
	CStdioFile f;
	int nNumVerticesXYZ = m_pRenderWnd->m_nNumVertices * 3;
	if (f.Open(strExportPath + L"\\Eigen.dat", CFile::modeCreate | CFile::modeWrite))
	{		
		for (int j = 0; j < nNumVerticesXYZ; j++)
		{
			for (int h = 0; h < NUM_EIGENS; h++)
			{
				strLine.Format(L"%f", m_pRenderWnd->m_Eigen[h][j]);
				f.WriteString(strLine);
			}
			f.WriteString(L"\n");
		}
		f.Close();
	}
	else
	{
		AfxMessageBox(L"Could not save EigenVector file");
		return;
	}

	if (f.Open(strExportPath + L"\\MeanFace.dat", CFile::modeCreate | CFile::modeWrite))
	{
		for (int j = 0; j < nNumVerticesXYZ; j++)
		{
			strLine.Format(L"%f\n", m_pRenderWnd->m_AveFace[j]);
			f.WriteString(strLine);
		}
		f.Close();
	}
	else
	{
		AfxMessageBox(L"Could not save file");
		return;
	}

	if (f.Open(strExportPath + L"\\MeanFace.obj", CFile::modeCreate | CFile::modeWrite))
	{
		for (int j = 0; j < m_pRenderWnd->m_nNumVertices; j++)
		{
			strLine.Format(L"v %f %f %f\n", 
				m_pRenderWnd->m_AveFace[j], 
				m_pRenderWnd->m_AveFace[m_pRenderWnd->m_nNumVertices + j], 
				m_pRenderWnd->m_AveFace[2 * m_pRenderWnd->m_nNumVertices + j]);
			f.WriteString(strLine);
		}
		size_t nNumTriangles = m_pRenderWnd->m_vIndices.size() / 3;
		for (int j = 0; j < nNumTriangles; j++)
		{
			strLine.Format(L"f %d %d %d\n",
				m_pRenderWnd->m_vIndices[3 * j + 0] + 1,
				m_pRenderWnd->m_vIndices[3 * j + 1] + 1,
				m_pRenderWnd->m_vIndices[3 * j + 2] + 1);
			f.WriteString(strLine);
		}
		f.Close();
	}
	else
	{
		AfxMessageBox(L"Could not save file");
		return;
	}
	//To load this into matlab using a .,m file, something along the following should work
	//fid = fopen('C:\_3DMMExport\MeanFace.dat');
	//avgFace = [];
	//while ~feof(fid)
	//	tline = fgetl(fid);
	//avgFace = [avgFace; sscanf(tline(1:end), '%f')']; 
	//	end
	//	fclose(fid);
	//fid = fopen('C:\_3DMMExport\Eigen.dat');
	//egn = [length(avgFace), 20];
	//k = 1;
	//while ~feof(fid)
	//	tline = fgetl(fid);
	//egn(k, 1:20) = sscanf(tline(1:end), '%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f');
	//k = k + 1;
	//end
	//	fclose(fid);
}
