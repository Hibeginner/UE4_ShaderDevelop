#pragma once
#include "GlobalShaderPlugDefine.h"

class FVertex_Pos_UV_Declaration:public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;
	virtual void InitRHI() override;
	virtual void ReleaseRHI() override;
};