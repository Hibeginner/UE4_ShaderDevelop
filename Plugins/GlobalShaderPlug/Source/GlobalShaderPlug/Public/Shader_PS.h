#pragma once
#include "MyShaderBase.h"

class FShader_PS : public FMyShaderBase
{
	DECLARE_GLOBAL_SHADER(FShader_PS);

public:
	FShader_PS();
	FShader_PS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
};
