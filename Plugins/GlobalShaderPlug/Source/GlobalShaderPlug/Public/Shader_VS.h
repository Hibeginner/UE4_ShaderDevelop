#pragma once
#include "MyShaderBase.h"

class FShader_VS : public FMyShaderBase
{
	DECLARE_GLOBAL_SHADER(FShader_VS);

public:
	FShader_VS();
	FShader_VS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
};

