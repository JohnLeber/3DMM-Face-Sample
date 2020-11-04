// D3DDlg.cpp : implementation file
//

#include "pch.h"
#include "FaceMorph.h"
#include "DxDlg.h" 
#include "vertex.h" 

 
//#include <iostream>
//#include <Eigen/Dense>
const int LINE_BUFF_SIZE = 4096;
// CDxWnd 

IMPLEMENT_DYNAMIC(CDxWnd, CWnd)

CDxWnd::CDxWnd()
{
	m_nNumVertices = 0;
	m_bWireframe = false;
	m_pVB = nullptr;
	m_pIB = nullptr;
	g_hWnd = NULL;
	g_hMainDlg = NULL;
	g_pD3D = NULL;
	g_pD3DDevice = NULL;
	g_pTexture = NULL; 
	g_fScale = 1.0f;
	m_pVertexPNTDecl = 0; 
	g_pEffect = NULL; 
	m_pFaceMesh = NULL;  
	m_hTech = NULL;
	m_hWVP = NULL;
	m_hWorldInvTrans = NULL;
	m_hLightVecW = NULL;
	m_hDiffuseMtrl = NULL;
	m_hDiffuseLight = NULL;
	m_hAmbientMtrl = NULL;
	m_hAmbientLight = NULL;
	m_hSpecularMtrl = NULL;
	m_hSpecularLight = NULL;
	m_hSpecularPower = NULL;
	m_hEyePos = NULL;
	m_hWorld = NULL;
	m_hTex = NULL;
	m_LightVecW = D3DXVECTOR3(0.0, 0.8f, 1.0f);

	D3DXVec3Normalize(&m_LightVecW, &m_LightVecW);
	 
	m_DiffuseMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_DiffuseLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_AmbientMtrl = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_AmbientLight = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_SpecularMtrl = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_SpecularLight = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 8.0f;
}
//---------------------------------------------------------------//
CDxWnd::~CDxWnd()
{
	ShutDown();
} 
//---------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDxWnd, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//---------------------------------------------------------------//
// CDxWnd message handlers
bool CDxWnd::Initialize(CWnd* pParent)
{ 
	bool bResult = SUCCEEDED(InitD3D());
	if (bResult)
	{ 
		InitAllVertexDeclarations(g_pD3DDevice);
		//LoadEigenValues();
		//LoadMeanFace();
	 
		{
			HRESULT hr = S_OK;
			if (m_pVB == nullptr)
			{
				hr = g_pD3DDevice->CreateVertexBuffer(m_nNumVertices * sizeof(VertexPNT), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
			}
			if (m_pIB == nullptr)
			{
				hr = g_pD3DDevice->CreateIndexBuffer(m_vIndices.size() * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &m_pIB, 0);
			}
			CEigenValues c;//default mesh to mean mesh only i.e. all eigenvalues set to zero)
			RecalcMesh(c);
		}
	}
	else
	{
		ATLASSERT(!L"InitD3D failed");
	}
	return bResult;
}  
////---------------------------------------------------------------//
//bool CDxWnd::CalcEigenValues(CString strMeshPath)
//{
//	std::vector<std::vector<float> > vMeshes;
//	 
//	CFileFind finder;
//	int nNumVertices = -1;
//	int nI1 = 0;
//	int nI2 = 0;
//	int nI3 = 0;
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	//STEP ONE: load the files
//	////////////////////////////////////////////////////////////////////////////////////////
//	
//	//count the number of files we will have to load
//	int nNumFiles = 0;
//	BOOL bWorking = finder.FindFile(strMeshPath + _T("*.mesh"));
//	while (bWorking)
//	{
//		bWorking = finder.FindNextFile();
//		nNumFiles++; 
//	}
//	finder.Close();
//	 
//	int nFileCounter = 0;
//	bWorking = finder.FindFile(strMeshPath + _T("*.mesh"));
//	while (bWorking)
//	{
//		bWorking = finder.FindNextFile();
//		if (!bWorking) {
//			long ert = 0;
//		}
//		CString strPath = finder.GetFilePath();
//		FILE* pFile = 0;
//		int nCounter = 0;
//		errno_t ret = _tfopen_s(&pFile, strPath, TEXT("r"));	 
//		if (ret == 0)
//		{ 
//			//cound the vertices in advance and make sure they fit the vector
//			while (!feof(pFile))
//			{
//				CHAR buffer[LINE_BUFF_SIZE];
//				fgets(buffer, LINE_BUFF_SIZE, pFile);
//				if (buffer[0] == 'v')
//				{
//					nCounter++;
//				}				
//			}
//			if (nNumVertices == -1)
//			{
//				nNumVertices = nCounter;
//			} 
//			else if (nNumVertices != nCounter)
//			{
//				fclose(pFile);
//				AfxMessageBox(L"wrong size");
//				return false;
//			}
//			if(nFileCounter == 0)
//			{
//				m_nNumVertices = nNumVertices;
//				//we have enough info now to preallocate memory to avoid performance penalties caused by dynamic memory allocations...
//				vMeshes.reserve(nNumFiles);
//				vMeshes.resize(nNumFiles);
//				for (int h = 0; h < nNumFiles; h++)
//				{
//					vMeshes[h].reserve(nNumVertices * 3);
//					vMeshes[h].resize(nNumVertices * 3);
//				}
//				m_AveFace.reserve(nNumVertices * 3);
//				m_AveFace.resize(nNumVertices * 3);
//				m_Mesh.reserve(nNumVertices * 3);
//				m_Mesh.resize(nNumVertices * 3);
//			}
//			rewind(pFile);
//			int nIndex = 0;
//			float x = 0;
//			float y = 0;
//			float z = 0;
//			while (!feof(pFile))
//			{
//				CHAR buffer[LINE_BUFF_SIZE];
//				fgets(buffer, LINE_BUFF_SIZE, pFile);
//				if (0 == strncmp("v ", buffer, 2))
//				{
//					//load vertices into the vectors
//					sscanf_s(buffer + 1, "%f %f %f", &x, &y, &z);
//					m_AveFace[nIndex] += x;
//					m_AveFace[nIndex + nNumVertices] +=y;
//					m_AveFace[nIndex + 2 * nNumVertices] += z;
//					vMeshes[nFileCounter][nIndex] = x;
//					vMeshes[nFileCounter][nIndex + nNumVertices] = y;
//					vMeshes[nFileCounter][nIndex + 2 * nNumVertices] = z;
//					nIndex++;
//				}
//				else if (nFileCounter == 0 && 0 == strncmp("f ", buffer, 2))
//				{
//					//load face information (list of three points forming a triangle). Do it for the first mesh only as the rest should be the same
//					sscanf_s(buffer + 1, "%d %d %d", &nI1, &nI2, &nI3);
//					m_vIndices.push_back(nI1 - 1);
//					m_vIndices.push_back(nI2 - 1);
//					m_vIndices.push_back(nI3 - 1);
//				}
//			}
//			nFileCounter++;
//			fclose(pFile);
//		}
//	}
//	finder.Close();
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	//STEP TWO: calculate the mean/average face mesh
//	////////////////////////////////////////////////////////////////////////////////////////
//	for (std::vector<float>::iterator i = m_AveFace.begin(); i != m_AveFace.end(); ++i)
//	{
//		*i = *i / nNumFiles;
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	//STEP THREE: subtract the mean from each face mesh
//	////////////////////////////////////////////////////////////////////////////////////////
//	for (int h = 0; h < nNumFiles; h++)
//	{
//		for (int j = 0; j < nNumVertices * 3; j++)
//		{
//			vMeshes[h][j] = vMeshes[h][j] - m_AveFace[j];
//		}
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////
//	//STEP FOUR: calculate the eigenvectors
//	////////////////////////////////////////////////////////////////////////////////////////
//	MatrixXf X(nNumVertices * 3, nNumFiles);
//	for (int h = 0; h < nNumFiles; h++)
//	{
//		for (int j = 0; j < nNumVertices * 3; j++)
//		{
//			X(j, h) = vMeshes[h][j];
//		}
//	}
//
//	//Use Principle Component Analysis. SVD routine in the Eigen library to do this
//	JacobiSVD<MatrixXf> svd(X, ComputeThinU | ComputeThinV); 
//	MatrixXf U = svd.matrixU();
//	MatrixXf V = svd.matrixV();
//	VectorXf s = svd.singularValues();
//	int rows = s.rows();
//	int columns = s.cols();
//
//	//the following should give us an idea of which/how manu eigencvectors are important.
//	for (int h = 0; h < rows; h++)
//	{
//		ATLTRACE2(L"s(%d) = %f\n", h + 1, s(h));
//	}
//	if (rows < NUM_EIGENS)
//	{
//		CString strMsg;
//		strMsg.Format(L"Not enough eigenvectors found. We need at least %d. Please increase the number of meshes and restart the application.", NUM_EIGENS);
//		AfxMessageBox(strMsg);
//		return false;
//	}
//	  
//	//There will be nNumFiles eigenvectors, but only take the first 20 (NUM_EIGENS)
//	m_Eigen.reserve(NUM_EIGENS);
//	m_Eigen.resize(NUM_EIGENS);
//	for (int h = 0; h < NUM_EIGENS; h++)
//	{
//		m_Eigen[h].reserve(nNumVertices * 3);
//		m_Eigen[h].resize(nNumVertices * 3);
//		for (int j = 0; j < nNumVertices * 3; j++)
//		{
//			m_Eigen[h][j] = U(j, h);
//		}
//	} 
//	//We now have the eignevectors. The mesh that is rendered will be the average/mean mesh plus a linesy combintation of the 
//	//the eignevector meshes (m_Eigen). That is each eignevector is multipled by the scalar correpnding to each slider in the UI.
//	return true;
//}
//---------------------------------------------------------------//
void CDxWnd::LoadEigenValues()
{
	/*CString strPath = L"D:\\_FacesInWirld\\_Eigenvalues\\_eigen.dat";
	FILE* pFile = 0;
	errno_t ret = _tfopen_s(&pFile, strPath, TEXT("r"));
	if (ret == 0)
	{
		int h = 0;
		m_Eigen.reserve(NUM_EIGENS);
		m_Eigen.resize(NUM_EIGENS);
		for (int h = 0; h < NUM_EIGENS; h++)
		{
			m_Eigen[h].reserve(NUM_EIGEN_ELEMENTS);
			m_Eigen[h].resize(NUM_EIGEN_ELEMENTS);
		}
		while (!feof(pFile))
		{
			CHAR buffer[LINE_BUFF_SIZE];
			fgets(buffer, LINE_BUFF_SIZE, pFile);

			sscanf_s(buffer, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ",
				&m_Eigen[0][h], &m_Eigen[1][h], &m_Eigen[2][h], &m_Eigen[3][h], &m_Eigen[4][h],
				&m_Eigen[5][h], &m_Eigen[6][h], &m_Eigen[7][h], &m_Eigen[8][h], &m_Eigen[9][h],
				&m_Eigen[10][h], &m_Eigen[11][h], &m_Eigen[12][h], &m_Eigen[13][h], &m_Eigen[14][h],
				&m_Eigen[15][h], &m_Eigen[16][h], &m_Eigen[17][h], &m_Eigen[18][h], &m_Eigen[19][h]);
			h++;
		}
		fclose(pFile);
	}*/
}
//---------------------------------------------------------------//
void CDxWnd::LoadMeanFace()
{
	{
		/*CString strPath = L"D:\\_FacesInWirld\\_Eigenvalues\\_aveface.dat";
		FILE* pFile = 0;
		errno_t ret = _tfopen_s(&pFile, strPath, TEXT("r"));
		if (ret == 0)
		{
			m_AveFace.clear();
			m_AveFace.reserve(NUM_EIGEN_ELEMENTS);
			m_AveFace.resize(NUM_EIGEN_ELEMENTS);
			int h = 0;
			while (!feof(pFile))
			{
				CHAR buffer[LINE_BUFF_SIZE];
				fgets(buffer, LINE_BUFF_SIZE, pFile);
				sscanf_s(buffer, "%f", &m_AveFace[h]);
				m_AveFace[h] = m_AveCalc[h];
				h++;
			}
			fclose(pFile);
		}*/
		//m_AveFace = m_AveCalc;
	}
	/*{
		CString strPath = L"D:\\_FacesInWirld\\triangles.dat";
		FILE* pFile = 0;
		errno_t ret = _tfopen_s(&pFile, strPath, TEXT("r"));
		if (ret == 0)
		{
			m_vIndices.clear();
			m_vIndices.reserve(NUM_TRIANGLES * 3);  
			while (!feof(pFile))
			{
				INT nI1 = 0;
				INT nI2 = 0;
				INT nI3 = 0;
				CHAR buffer[LINE_BUFF_SIZE];
				fgets(buffer, LINE_BUFF_SIZE, pFile);
				sscanf_s(buffer + 1, "%d %d %d", &nI1, &nI2, &nI3); 
				m_vIndices.push_back(nI1 - 1);
				m_vIndices.push_back(nI2 - 1);
				m_vIndices.push_back(nI3 - 1);
			}
			fclose(pFile);
		}
	}*/
}
//---------------------------------------------------------------//
void CDxWnd::OnTimer(UINT_PTR nIDEvent)
{ 
	CWnd::OnTimer(nIDEvent);
}
//---------------------------------------------------------------//
void CDxWnd::RecalcMesh(CEigenValues& e)
{
	if (m_nNumVertices == 0) return;
	for (int j = 0; j < m_nNumVertices * 3; j++)
	{
		float r = m_AveFace[j] / 8.0f;
		for (int h = 0; h < NUM_EIGENS; h++)
		{
			r = r + e.m_Next[h] * m_Eigen[h][j];
		}
		m_Mesh[j] = r;
	}
	if (m_pVB && m_pIB)
	{
		VertexPNT* v = 0;
		HRESULT hr = m_pVB->Lock(0, 0, (void**)&v, 0); 
		for (int j = 0; j < m_nNumVertices; j++)
		{
			float x = m_Mesh[j];
			float y = m_Mesh[m_nNumVertices + j];
			float z = m_Mesh[2 * m_nNumVertices + j];
			v[j] = VertexPNT(x, y, z, 0, 0, 0, 0, 0);
		}
		{
			DWORD* k = 0;
			HRESULT hr = m_pIB->Lock(0, 0, (void**)&k, 0);
			int nNumTriangles = m_vIndices.size();
			for (int j = 0; j < nNumTriangles; j++)
			{
				k[j] = m_vIndices[j];
			}
			//compute normals (or at least approximate them)
			for (int j = 0; j < nNumTriangles; j++)
			{ 
				if (j > 1 && j < nNumTriangles - 2)
				{	
					int vip = k[j - 1];//prev
					int vic = k[j];//current					
					int vin = k[j + 1];//next 
					D3DXVECTOR3 va = v[vip].pos;
					D3DXVECTOR3 vb = v[vic].pos;
					D3DXVECTOR3 vc = v[vin].pos;
					D3DXVECTOR3 cross;
					D3DXVECTOR3 vc1 = vb - va;
					D3DXVECTOR3 vc2 = vc - va;
					D3DXVec3Cross(&cross, &vc1, &vc2);
					D3DXVec3Normalize(&cross, &cross);  
					v[vip].normal = cross;
					v[vic].normal = cross;
					v[vin].normal = cross;
				}
			}
			hr = m_pIB->Unlock();
		}
		hr = m_pVB->Unlock();
	}
}
//---------------------------------------------------------------//
void CDxWnd::Render()
{
	if (g_pD3DDevice == 0) return;

	HRESULT hr = S_OK;
	  
	if (FAILED(g_pD3DDevice->BeginScene()))
		return;
 
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 170, 50), 1.0f, 0);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	// Draw the background gradient.
	RECT rClient;
	GetClientRect(&rClient);
 
	D3DXCOLOR c1(0.2, 0.2, 0.2, 1.0);
	D3DXCOLOR c2(0.45, 0.45, 0.45, 1.0);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	DrawTransformedQuad(g_pD3DDevice, -0.5f, -0.5f, 0.f,
		(FLOAT)(rClient.right - rClient.left),
		(FLOAT)(rClient.bottom - rClient.top),
		c1, c1, c2, c2);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	 
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	if (m_bWireframe)
	{
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	D3DXMatrixIdentity(&m_World);
	D3DXVECTOR3 vEye(0, 0, 10);
	float nRenderFOV = 12.59f;
	D3DXMatrixLookAtLH(&m_View, &vEye, &(D3DXVECTOR3(0, 0, 0)), &(D3DXVECTOR3(0, 1, 0)));
	D3DXMatrixPerspectiveFovLH(&m_Proj, nRenderFOV, 1, 1, 50);

	m_WVP = m_World * m_View * m_Proj;
	D3DXMATRIX WI;
	D3DXMatrixInverse(&WI, NULL, &m_World);
	D3DXMatrixTranspose(&m_WIT, &WI);
	hr = g_pEffect->SetTechnique(m_hTech); 
	 
	hr = g_pD3DDevice->SetVertexDeclaration( VertexPNT::Decl);
	UINT numPasses = 0;
 
	hr = g_pEffect->Begin(&numPasses, 0);
	for (UINT k = 0; k < numPasses; ++k)
	{
		hr = g_pEffect->BeginPass(k);
		hr = g_pEffect->SetMatrix(m_hWorldInvTrans, &m_WIT);
		hr = g_pEffect->SetMatrix(m_hWVP, &m_WVP);
		hr = g_pEffect->SetValue(m_hLightVecW, &m_LightVecW, sizeof(D3DXVECTOR3));
		hr = g_pEffect->SetValue(m_hDiffuseMtrl, &m_DiffuseMtrl, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetValue(m_hDiffuseLight, &m_DiffuseLight, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetValue(m_hAmbientMtrl, &m_AmbientMtrl, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetValue(m_hAmbientLight, &m_AmbientLight, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetValue(m_hSpecularLight, &m_SpecularLight, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetValue(m_hSpecularMtrl, &m_SpecularMtrl, sizeof(D3DXCOLOR));
		hr = g_pEffect->SetFloat(m_hSpecularPower, m_SpecularPower);
		hr = g_pEffect->SetMatrix(m_hWorld, &m_World);
		hr = g_pEffect->SetTexture(m_hTex, g_pTexture);
		hr = g_pEffect->SetValue(m_hEyePos, &vEye, sizeof(D3DXVECTOR3));
		g_pEffect->CommitChanges();
		hr = g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(VertexPNT));
		hr = g_pD3DDevice->SetIndices(m_pIB);		 
		hr = g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0, m_nNumVertices, 0, m_vIndices.size() / 3);
		if (FAILED(hr))
		{
			ATLASSERT(0);
		}
		hr = g_pEffect->EndPass();
	} 
	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//---------------------------------------------------------------//
// Initialize Direct3D stuff.
HRESULT CDxWnd::InitD3D()
{
	HRESULT hr;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!g_pD3D)
		return E_FAIL;

	D3DDISPLAYMODE d3ddm;

	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // v-sync on - No need to burn the CPU and GPU over nothing.
	d3dpp.hDeviceWindow = m_hWnd;

	if (FAILED(hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		return hr;

	return OnCreateDevice();
}
//---------------------------------------------------------------//
// This is called after the device has been created.
HRESULT CDxWnd::OnCreateDevice()
{
	HRESULT hr;
	CString strFileName;
	TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
	DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 0, MAX_PATH);
	if (dwFLen == 0 || dwFLen == MAX_PATH)
	{
		ATLASSERT(0);
		return E_FAIL;
	}
	else
	{
		strFileName = CString(szFilePath);
		long nRight = strFileName.ReverseFind(_T('\\'));
		strFileName = strFileName.Left(nRight);
		//if running otuside of VS, the next few lines will need to change.
		strFileName.Replace(L"\\Debug", L"");
		strFileName.Replace(L"\\Release", L"");
		strFileName.Append(_T("\\FaceMorph\\FaceMorph.fx"));
	}
	// Load the effect from file.
	LPD3DXBUFFER pErrors = NULL;
	hr = D3DXCreateEffectFromFile(g_pD3DDevice, strFileName, NULL, NULL, 0, NULL, &g_pEffect, &pErrors);
	if (FAILED(hr))
	{
		if (pErrors)
		{
			CA2T tErrors = (char*)pErrors->GetBufferPointer();
			AfxMessageBox((LPCTSTR)tErrors, MB_OK | MB_ICONSTOP);
			pErrors->Release();
		}
		return hr;
	}
	strFileName.Replace(_T("FaceMorph.fx"), _T("texture.png"));
	hr = D3DXCreateTextureFromFile(g_pD3DDevice, strFileName, &g_pTexture);
	 
	m_hTech				= g_pEffect->GetTechniqueByName("DirLightTexTech");
	m_hWVP				= g_pEffect->GetParameterByName(0, "gWVP");
	m_hWorldInvTrans	= g_pEffect->GetParameterByName(0, "gWorldInvTrans");
	m_hLightVecW		= g_pEffect->GetParameterByName(0, "gLightVecW");
	m_hDiffuseMtrl		= g_pEffect->GetParameterByName(0, "gDiffuseMtrl");
	m_hDiffuseLight		= g_pEffect->GetParameterByName(0, "gDiffuseLight");
	m_hAmbientMtrl		= g_pEffect->GetParameterByName(0, "gAmbientMtrl");
	m_hAmbientLight		= g_pEffect->GetParameterByName(0, "gAmbientLight");
	m_hSpecularMtrl		= g_pEffect->GetParameterByName(0, "gSpecularMtrl");
	m_hSpecularLight	= g_pEffect->GetParameterByName(0, "gSpecularLight");
	m_hSpecularPower	= g_pEffect->GetParameterByName(0, "gSpecularPower");
	m_hEyePos			= g_pEffect->GetParameterByName(0, "gEyePosW");
	m_hWorld			= g_pEffect->GetParameterByName(0, "gWorld");
	m_hTex				= g_pEffect->GetParameterByName(0, "gTex");

	D3DVERTEXELEMENT9 VertexPNTElements[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	hr = g_pD3DDevice->CreateVertexDeclaration(VertexPNTElements, &m_pVertexPNTDecl);


	return S_OK;
}
//---------------------------------------------------------------//
void CDxWnd::ShutDown()
{
	SAFE_RELEASE(g_pTexture); 

	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(m_pFaceMesh);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}
//---------------------------------------------------------------//
HRESULT CDxWnd::DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
	FLOAT x, FLOAT y, FLOAT z,
	FLOAT width, FLOAT height,
	D3DCOLOR c1,
	D3DCOLOR c2,
	D3DCOLOR c3,
	D3DCOLOR c4)
{
	return DrawTransformedQuad(pDevice, x, y, z, width, height,
		D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 0), D3DXVECTOR2(0, 1), D3DXVECTOR2(1, 1),
		c1, c2, c3, c4);
}
//---------------------------------------------------------------//
HRESULT CDxWnd::DrawTransformedQuad(LPDIRECT3DDEVICE9 pDevice,
	FLOAT x, FLOAT y, FLOAT z,
	FLOAT width, FLOAT height,
	D3DXVECTOR2 uvTopLeft, D3DXVECTOR2 uvTopRight,
	D3DXVECTOR2 uvBottomLeft, D3DXVECTOR2 uvBottomRight,
	D3DCOLOR c1, D3DCOLOR c2, D3DCOLOR c3, D3DCOLOR c4)
{
	struct
	{
		float pos[4];
		D3DCOLOR color;
		float uv[2];
	} quad[] =
	{
		x,			y,				z, 1.f,		c1, uvTopLeft.x,		uvTopLeft.y,
		x + width,	y,				z, 1.f,		c2, uvBottomRight.x,	uvTopLeft.y,
		x,			y + height,		z, 1.f,		c3, uvTopLeft.x,		uvBottomRight.y,
		x + width,	y + height,		z, 1.f,		c4, uvBottomRight.x,	uvBottomRight.y,
	};
	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0));
	return pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(quad[0]));
}
//---------------------------------------------------------------//