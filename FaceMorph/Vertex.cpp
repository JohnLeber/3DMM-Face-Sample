//=============================================================================
// Vertex.cpp by Frank Luna (C) 2005 All Rights Reserved.
//=============================================================================

#include "Vertex.h" 
//---------------------------------------------------------------------------//
// Initialize static variables.
IDirect3DVertexDeclaration9* VertexPNT::Decl = 0;
void InitAllVertexDeclarations(IDirect3DDevice9* pD3DDevice)
{
	//===============================================================
	// VertexPos

	D3DVERTEXELEMENT9 VertexPNTElements[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	HRESULT hr = pD3DDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNT::Decl);
}
//---------------------------------------------------------------------------//
void DestroyAllVertexDeclarations()
{
	if (VertexPNT::Decl)
	{
		VertexPNT::Decl->Release();
		VertexPNT::Decl = 0;
	}
}
//---------------------------------------------------------------------------//