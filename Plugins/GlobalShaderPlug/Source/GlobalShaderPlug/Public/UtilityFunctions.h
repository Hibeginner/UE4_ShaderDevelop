// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityFunctions.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALSHADERPLUG_API UUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "KenUtility", meta = (WorldContext = "WorldContexObject"))
	static void DrawToQuad(class UTextureRenderTarget2D* OutputRenderTarget, FLinearColor MyColor,
	                       UTexture2D* MyTexture);
};
