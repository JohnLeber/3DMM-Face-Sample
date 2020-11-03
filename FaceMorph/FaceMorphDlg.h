
// FaceMorphDlg.h : header file
//

#pragma once

#ifndef SAFE_DELETE
/// For pointers allocated with new.
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
/// For use with COM pointers.
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif
class CDxWnd;
//--------------------------------------------------------------------------------------------------//
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f) {}
	VertexPNT(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v) :pos(x, y, z), normal(nx, ny, nz), tex0(u, v) {}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
		:pos(v), normal(n), tex0(uv) {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};
//--------------------------------------------------------------------------------------------------//
// CFaceMorphDlg dialog
class CFaceMorphDlg : public CDialog
{
// Construction
public:
	CFaceMorphDlg(CWnd* pParent = nullptr);	// standard constructor
	~CFaceMorphDlg();
	
	 
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FACEMORPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CDxWnd* m_pRenderWnd;
	CEigenValues m_Scalars;
 
// Implementation
protected: 
	void OnTimer(UINT_PTR nIdEvent); 
	 
	HICON m_hIcon;
	void OnSlider();

	CSliderCtrl m_Slider[NUM_SLIDERS];
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
public:   
	afx_msg void OnClose();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedCheck1();
};
//--------------------------------------------------------------------------------------------------//