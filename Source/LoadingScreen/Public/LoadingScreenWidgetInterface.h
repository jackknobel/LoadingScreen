// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "LoadingScreenTypes.h"
#include "LoadingScreenWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULoadingScreenWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOADINGSCREEN_API ILoadingScreenWidgetInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = LoadingScreen)
	void SetLoadingScreenDescription(const FLoadingScreenDescription& Description);

	UFUNCTION(BlueprintNativeEvent, Category = LoadingScreen)
	void SetLoadingLevelName(const FString& LevelName);

protected:
	virtual void SetLoadingScreenDescription_Implementation(const FLoadingScreenDescription& Description) = 0;
};
