// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "SScaleBox.h"
#include "MoviePlayer.h"
#include "Engine/DeveloperSettings.h"
#include "LoadingScreenSettings.generated.h"

class ULoadingScreenWidget;
class UObject;

USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLoadingScreenDescription
{
	GENERATED_BODY()

	FLoadingScreenDescription();

	/* The minimum time that a loading screen should be opened for, -1 if there is no minimum time. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	float MinimumLoadingScreenDisplayTime;

	/* If true, the loading screen will disappear as soon as all movies are played and loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bAutoCompleteWhenLoadingCompletes;

	/* If true, movies can be skipped by clicking the loading screen as long as loading is done. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bMoviesAreSkippable;

	/* If true, movie playback continues until Stop is called. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Loading)
	bool bWaitForManualStop;

	/* The movie paths local to the game's Content/Movies/ directory we will play. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movies)
	TArray<FString> MoviePaths;

	/* Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movies)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/*  Should we show the images/tips/loading text?  Generally you'll want to set this to false if you just want to show a movie. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	bool bShowUIOverlay;

	/* User Widget Class we will use in the Loading Screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition= bShowUIOverlay), Category = UI)
	TSubclassOf<ULoadingScreenWidget> UIOverlayClass;

	/*  The text we want to display in the loading screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bShowUIOverlay), Category = UI)
	FText LoadingText;

	/* The font used for the loading text  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bShowUIOverlay), Category = UI)
	FSlateFontInfo LoadingFont;

	/* The texture display while in the loading screen on top of the movie. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowedClasses = "Texture2D", EditCondition = bShowUIOverlay), Category = UI)
	TArray<FStringAssetReference> Images;

	ULoadingScreenWidget* GetLoadingScreenWidget(UObject* WorldContextObject) const;
};

/**
 * Settings for the simple loading screen plugin.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Loading Screen"))
class LOADINGSCREEN_API ULoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	ULoadingScreenSettings(const FObjectInitializer& Initializer);
public:

	/** The startup screen for the project. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription StartupScreen;

	/** The default load screen between maps. */
	UPROPERTY(config, EditAnywhere, Category=Screens)
	FLoadingScreenDescription DefaultScreen;

	/** The font to display the tips in. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "FSlateFontInfo"), Category=Advice)
	FSlateFontInfo TipFont;

	/** The size of the tip before it's wrapped to the next line. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	float TipWrapAt;

	/** The tips to display on the load screen. */
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category=Advice)
	TArray<FText> Tips;

	UFUNCTION(BlueprintCallable, meta = (DisplayName=GetLoadingScreenSettings), Category = Settings)
	static ULoadingScreenSettings* GetMutableLoadingScreenSettings();

	static const ULoadingScreenSettings* GetLoadingScreenSettings();
};
