#include "Shader_VS.h"

FShader_VS::FShader_VS()
{
}

FShader_VS::FShader_VS(const ShaderMetaType::CompiledShaderInitializerType& Initializer): FMyShaderBase(Initializer)
{
}

IMPLEMENT_SHADER_TYPE(, FShader_VS, TEXT("/GlobalShaderPlug/MyGlobalShader.usf"),TEXT("MainVS"),SF_Vertex)
