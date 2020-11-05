#pragma once
 
//---------------------------------------------------------------------------//
// CDxWnd dialog 
class CDxWnd : public CWnd
{
	DECLARE_DYNAMIC(CDxWnd)

public:
	CDxWnd();   // standard constructor
	virtual ~CDxWnd();
	bool Initialize(CWnd* pParent);
	void Render();
	void ShutDown();
	void RecalcMesh(CEigenValues& e);
public:
	std::vector<float> m_AveFace;
	std::vector<float> m_Mesh;
	std::vector<std::vector<float> > m_Eigen;
	std::vector<long> m_vIndices;
	long m_nNumVertices;
protected:
	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9*  m_pIB;
	//bool CalcEigenValues(CString strMeshPath);
	bool DoesFileExist(CString strFile);
	// the next two functions were used for loading precompted data using Matlab.
	void LoadEigenValues();
	void LoadMeanFace();
 
	 
	 

	HRESULT DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
		FLOAT x, FLOAT y, FLOAT z,
		FLOAT width, FLOAT height,
		D3DXVECTOR2 uvTopLeft, D3DXVECTOR2 uvTopRight,
		D3DXVECTOR2 uvBottomLeft, D3DXVECTOR2 uvBottomRight,
		D3DCOLOR c1, D3DCOLOR c2, D3DCOLOR c3, D3DCOLOR c4);

	HRESULT DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
		FLOAT x, FLOAT y, FLOAT z,
		FLOAT width, FLOAT height,
		D3DCOLOR c1 = 0xFFFFFFFF,
		D3DCOLOR c2 = 0xFFFFFFFF,
		D3DCOLOR c3 = 0xFFFFFFFF,
		D3DCOLOR c4 = 0xFFFFFFFF);

	HRESULT InitD3D();
	HRESULT OnCreateDevice();
 
	D3DXHANDLE		m_hTech;
	D3DXHANDLE		m_hWVP;
	D3DXHANDLE		m_hWorldInvTrans;
	D3DXHANDLE		m_hLightVecW;
	D3DXHANDLE		m_hDiffuseMtrl;
	D3DXHANDLE		m_hDiffuseLight;
	D3DXHANDLE		m_hAmbientMtrl;
	D3DXHANDLE		m_hAmbientLight;
	D3DXHANDLE		m_hSpecularMtrl;
	D3DXHANDLE		m_hSpecularLight;
	D3DXHANDLE		m_hSpecularPower;
	D3DXHANDLE		m_hEyePos;
	D3DXHANDLE		m_hWorld;
	D3DXHANDLE		m_hTex;
	D3DXVECTOR3		m_LightVecW;
	D3DXCOLOR		m_AmbientMtrl;
	D3DXCOLOR		m_AmbientLight;
	D3DXCOLOR		m_DiffuseMtrl;
	D3DXCOLOR		m_DiffuseLight;
	D3DXCOLOR		m_SpecularMtrl;
	D3DXCOLOR		m_SpecularLight;
	float			m_SpecularPower;

	//m_hTex;
	//m_hDiffuseMtrl;
	//m_hDiffuseLight;
	//m_hAmbientMtrl;
	//m_hAmbientLight;
	//m_hSpecularMtrl;
	//m_hSpecularLight;
	//m_hSpecularPower;

	HWND							g_hWnd;
	HWND							g_hMainDlg;
	LPDIRECT3D9						g_pD3D;
	LPDIRECT3DDEVICE9				g_pD3DDevice;
	LPDIRECT3DTEXTURE9				g_pTexture; 
	float							g_fScale;
	IDirect3DVertexDeclaration9* m_pVertexPNTDecl;
	LPD3DXEFFECT					g_pEffect; 
	D3DXMATRIX m_World, m_WVP, m_WIT, m_View, m_Proj;
	ID3DXMesh* m_pFaceMesh;  

	DECLARE_MESSAGE_MAP()
public:
	bool m_bWireframe;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
