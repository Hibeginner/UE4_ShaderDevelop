// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityFunctions.h"

#include "Shader_PS.h"
#include "Shader_VS.h"
#include "Vertex_Pos_UV_Declaration.h"
#include "Engine/TextureRenderTarget2D.h"

static void DrawToQuad_RenderThread(FRHICommandListImmediate& RHICmdList,
                                    FTextureRenderTargetResource* OutputRenderTargetResource, FLinearColor MyColor,
                                    FRHITexture2D* MyRHITexture)
{
	check(IsInRenderingThread());

	FRHITexture2D* RenderTargetTexture = OutputRenderTargetResource->GetRenderTargetTexture();
	RHICmdList.Transition(FRHITransitionInfo(RenderTargetTexture, ERHIAccess::SRVMask, ERHIAccess::RTV));
	FRHIRenderPassInfo RPInfo(RenderTargetTexture, ERenderTargetActions::Load_Store);
	SCOPED_DRAW_EVENT(RHICmdList, DrawColorPass);
	RHICmdList.BeginRenderPass(RPInfo, TEXT("DrawColorPass"));
	{
		// SetViewport
		RHICmdList.SetViewport(0, 0, 0.f, OutputRenderTargetResource->GetSizeX(),
		                       OutputRenderTargetResource->GetSizeY(), 1.f);

		FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
		TShaderMapRef<FShader_VS> VertexShader(GlobalShaderMap);
		TShaderMapRef<FShader_PS> PixelShader(GlobalShaderMap);

		FVertex_Pos_UV_Declaration VertexDesc;
		VertexDesc.InitRHI();

		// Set the graphic pipeline state.  
		FGraphicsPipelineStateInitializer GraphicsPipelineStateInitializer;
		RHICmdList.ApplyCachedRenderTargets(GraphicsPipelineStateInitializer);
		GraphicsPipelineStateInitializer.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
		GraphicsPipelineStateInitializer.BlendState = TStaticBlendState<>::GetRHI();
		GraphicsPipelineStateInitializer.RasterizerState = TStaticRasterizerState<>::GetRHI();
		GraphicsPipelineStateInitializer.PrimitiveType = PT_TriangleList;
		// GraphicsPipelineStateInitializer.BoundShaderState.VertexDeclarationRHI = GetVertexDeclarationFVector4();
		GraphicsPipelineStateInitializer.BoundShaderState.VertexDeclarationRHI = VertexDesc.VertexDeclarationRHI;
		GraphicsPipelineStateInitializer.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
		GraphicsPipelineStateInitializer.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();
		SetGraphicsPipelineState(RHICmdList, GraphicsPipelineStateInitializer);

		PixelShader->SetParameters(RHICmdList, MyColor, MyRHITexture);

		// Vertex Buffer Begins --------------------------
		FRHIResourceCreateInfo CreateInfo;
		FVertexBufferRHIRef MyVertexBufferRHI = RHICreateVertexBuffer(sizeof(FVertex_Pos_UV) * 4, BUF_Static,
		                                                              CreateInfo);
		void* VoidPtr = RHILockVertexBuffer(MyVertexBufferRHI, 0, sizeof(FVertex_Pos_UV) * 4, RLM_WriteOnly);
		FVertex_Pos_UV v[4];
		// LT
		v[0].Position = FVector4(-1.0f, 1.0f, 0.0f, 1.0f);
		v[0].UV = FVector2D(0, 1.0f);
		// RT
		v[1].Position = FVector4(1.0f, 1.0f, 0.0f, 1.0f);
		v[1].UV = FVector2D(1.0f, 1.0f);
		// LB
		v[2].Position = FVector4(-1.0f, -1.0f, 0.0f, 1.0f);
		v[2].UV = FVector2D(0.0f, 0.0f);
		// RB
		v[3].Position = FVector4(1.0f, -1.0f, 0.0f, 1.0f);
		v[3].UV = FVector2D(1.0f, 0.0f);

		FMemory::Memcpy(VoidPtr, &v, sizeof(FVertex_Pos_UV) * 4);
		RHIUnlockVertexBuffer(MyVertexBufferRHI);
		// Vertex Buffer Ends --------------------------

		// Index Buffer Begins--------------------
		static const uint16 Indices[6] =
		{
			0, 1, 2,
			2, 1, 3
		};

		FRHIResourceCreateInfo IndexBufferCreateInfo;
		FIndexBufferRHIRef MyIndexBufferRHI = RHICreateIndexBuffer(sizeof(uint16), sizeof(uint16) * 6, BUF_Static,
		                                                           IndexBufferCreateInfo);
		void* VoidPtr2 = RHILockIndexBuffer(MyIndexBufferRHI, 0, sizeof(uint16) * 6, RLM_WriteOnly);
		FMemory::Memcpy(VoidPtr2, Indices, sizeof(uint16) * 6);

		RHIUnlockIndexBuffer(MyIndexBufferRHI);
		// Index Buffer Ends-----------------------

		// Draw Indexed
		RHICmdList.SetStreamSource(0, MyVertexBufferRHI, 0);
		RHICmdList.DrawIndexedPrimitive(MyIndexBufferRHI, 0, 0, 4, 0, 2, 1);

		MyIndexBufferRHI.SafeRelease();
		MyVertexBufferRHI.SafeRelease();
	}

	RHICmdList.EndRenderPass();
	RHICmdList.Transition(FRHITransitionInfo(RenderTargetTexture, ERHIAccess::RTV, ERHIAccess::SRVMask));
}

void UUtilityFunctions::DrawToQuad(UTextureRenderTarget2D* OutputRenderTarget, FLinearColor MyColor,
                                   UTexture2D* MyTexture)
{
	check(IsInGameThread());
	if (!OutputRenderTarget)
	{
		return;
	}
	FTextureRenderTargetResource* TextureRenderTargetResource = OutputRenderTarget->
		GameThread_GetRenderTargetResource();
	FRHITexture2D* MyRHITexture2D = MyTexture->TextureReference.TextureReferenceRHI->GetReferencedTexture()->
	                                           GetTexture2D();
	ENQUEUE_RENDER_COMMAND(CaptureCommand)(
		[TextureRenderTargetResource, MyColor,MyRHITexture2D](FRHICommandListImmediate& RHICmdList)
		{
			DrawToQuad_RenderThread(RHICmdList, TextureRenderTargetResource, MyColor, MyRHITexture2D);
		});
}
