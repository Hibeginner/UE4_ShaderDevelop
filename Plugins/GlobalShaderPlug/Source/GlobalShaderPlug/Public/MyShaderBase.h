#pragma once

#include "GlobalShader.h"

BEGIN_UNIFORM_BUFFER_STRUCT(FMyUniform,)
	SHADER_PARAMETER(FLinearColor, Color1)
	SHADER_PARAMETER(FVector4, Color2)
	SHADER_PARAMETER(float, LerpValue)
END_UNIFORM_BUFFER_STRUCT()

class FMyShaderBase : public FGlobalShader
{
	DECLARE_INLINE_TYPE_LAYOUT(FMyShaderBase, NonVirtual);

private:
	LAYOUT_FIELD(FShaderParameter, MainColorVal);
	LAYOUT_FIELD(FShaderResourceParameter, MainTextureVal);
	LAYOUT_FIELD(FShaderResourceParameter, MainTextureSamplerVal);

public:
	FMyShaderBase();
	FMyShaderBase(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform);
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters,
	                                         FShaderCompilerEnvironment& OutEnvironment);
	void SetParameters(FRHICommandListImmediate& RHICmdList, const FLinearColor& MyColor, FRHITexture2D* InTexture);
};
