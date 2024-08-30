#include "MyShaderBase.h"

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FMyUniform, "MyUniform");

FMyShaderBase::FMyShaderBase()
{
}

FMyShaderBase::FMyShaderBase(const ShaderMetaType::CompiledShaderInitializerType& Initializer): FGlobalShader(
	Initializer)
{
	MainColorVal.Bind(Initializer.ParameterMap, TEXT("MainColor"));
	MainTextureVal.Bind(Initializer.ParameterMap, TEXT("MainTexture"));
	MainTextureSamplerVal.Bind(Initializer.ParameterMap, TEXT("MainTextureSampler"));
}

bool FMyShaderBase::ShouldCache(EShaderPlatform Platform)
{
	return true;
}

bool FMyShaderBase::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return true;
}

void FMyShaderBase::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters,
                                                 FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
}

void FMyShaderBase::SetParameters(FRHICommandListImmediate& RHICmdList, const FLinearColor& MyColor,
                                  FRHITexture2D* InTexture)
{
	SetShaderValue(RHICmdList, RHICmdList.GetBoundPixelShader(), MainColorVal, MyColor);

	SetTextureParameter(RHICmdList, RHICmdList.GetBoundPixelShader(), MainTextureVal, MainTextureSamplerVal,
	                    TStaticSamplerState<SF_Trilinear, AM_Clamp, AM_Clamp>::GetRHI(), InTexture);

	FMyUniform uniform;
	uniform.Color1 = FLinearColor::Blue;
	uniform.Color2 = FLinearColor::Green;
	uniform.LerpValue = 0.5f;
	TUniformBufferRef<FMyUniform> Data = TUniformBufferRef<FMyUniform>::CreateUniformBufferImmediate(
		uniform, UniformBuffer_SingleFrame);
	SetUniformBufferParameter(RHICmdList, RHICmdList.GetBoundPixelShader(), GetUniformBufferParameter<FMyUniform>(),
	                          Data);
}
