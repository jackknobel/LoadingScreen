// Copyright 2014 - 2017 Pine Fire Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenSettings.h"
#include "LoadingScreenWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class LOADINGSCREEN_API ULoadingScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = Settings)
	FLoadingScreenDescription LoadingScreenDescription;

	/************************************************************************/
	/* Widgets                                                              */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* LoadingText_TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UImage* Background_Image;
};
