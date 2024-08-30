#include "Shader_PS.h"

FShader_PS::FShader_PS()
{
}

FShader_PS::FShader_PS(const ShaderMetaType::CompiledShaderInitializerType& Initializer): FMyShaderBase(Initializer)
{
}

IMPLEMENT_SHADER_TYPE(, FShader_PS, TEXT("/GlobalShaderPlug/MyGlobalShader.usf"), TEXT("MainPS"), SF_Pixel)