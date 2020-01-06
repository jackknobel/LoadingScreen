// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LoadingScreenTypes.h"
#include "Engine/DeveloperSettings.h"
#include "LoadingScreenSettings.generated.h"

class UObject;
class UUserWidget;

/**
 * Settings for the simple loading screen plugin.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loading Screen"))
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	ULoadingScreenSettings(const FObjectInitializer& Initializer);

public:
	/** Whether or not we want to display the start up screen or not */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = Screens)
	bool bUseStartupScreen;

	/** The startup screen for the project. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (EditCondition = bUseStartupScreen), Category = Screens)
	FLoadingScreenDescription StartupScreen;

	/** The default load screen between maps. */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, Category = Screens)
	FLoadingScreenDescription DefaultScreen;

	/** Level Data */
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (TitleProperty = FriendlyName), Category = Settings)
	TSet<FLevelLoadScreenData> FriendlyLevelNames;

	/** The font to display the tips in. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	FSlateFontInfo TipFont;

	/** The tips to display on the load screen. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	TArray<FText> Tips;

	UFUNCTION(BlueprintCallable, Category = Settings, meta = (WorldContext = "WorldContextObject"))
	static FText GetLevelFriendlyName(const FString& LevelName);

	static const ULoadingScreenSettings* GetLoadingScreenSettings()	{ return GetDefault<ULoadingScreenSettings>(); }
};
