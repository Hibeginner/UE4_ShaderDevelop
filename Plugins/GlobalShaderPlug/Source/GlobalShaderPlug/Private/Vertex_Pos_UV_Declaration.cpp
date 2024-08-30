#include "Vertex_Pos_UV_Declaration.h"

void FVertex_Pos_UV_Declaration::InitRHI()
{
	FVertexDeclarationElementList Elements;
	uint32 Stride = sizeof(FVertex_Pos_UV);
	Elements.Add(FVertexElement(0,STRUCT_OFFSET(FVertex_Pos_UV, Position), VET_Float4, 0, Stride));
	Elements.Add(FVertexElement(0,STRUCT_OFFSET(FVertex_Pos_UV, UV), VET_Float2, 1, Stride));
	VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
}

void FVertex_Pos_UV_Declaration::ReleaseRHI()
{
	VertexDeclarationRHI->Release();
}
