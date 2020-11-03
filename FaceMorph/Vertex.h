//=============================================================================
// Vertex.h by Frank Luna (C) 2005 All Rights Reserved.
//
// This file contains all of our vertex structures.  We will add to it
// as needed throughout the book.
//=============================================================================

#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>


// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations(IDirect3DDevice9* pD3DDevice);
void DestroyAllVertexDeclarations();
 
//===============================================================
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f)
		//tex1(0.0f, 0.0f),
		//tangent(0.0f, 0.0f, 0.0f),
		/*binormal(0.0f, 0.0f, 0.0f)*/ {}
	VertexPNT(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v) :pos(x, y, z), normal(nx, ny, nz), tex0(u, v)/*,  tex1(u2,v2)*/ {}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv, const D3DXVECTOR2& uv2)
		:pos(v), normal(n), tex0(uv)/*, tex1(uv2)*/ {}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0; 

	static IDirect3DVertexDeclaration9* Decl;
};

#endif // VERTEX_H