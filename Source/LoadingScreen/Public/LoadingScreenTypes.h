// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoviePlayer.h"
#include "SlateFontInfo.h"
#include "LoadingScreenTypes.generated.h"

class UUserWidget;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RelativeToGameDir), Category = Movies)
	TArray<FString> MoviePaths;

	/* Should we just play back, loop, etc.  NOTE: if the playback type is MT_LoopLast, then bAutoCompleteWhenLoadingCompletes will be togged on when the last movie is hit*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movies)
	TEnumAsByte<EMoviePlaybackType> PlaybackType;

	/*  Should we show the images/tips/loading text?  Generally you'll want to set this to false if you just want to show a movie. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Show UI Overlay"), Category = UI)
	bool bShowUIOverlay;

	/* User Widget Class we will use in the Loading Screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MustImplement = LoadingScreenWidgetInterface, EditCondition = bShowUIOverlay, DisplayName = "UI Overlay Class"), Category = UI)
	TSubclassOf<UUserWidget> UIOverlayClass;

	/*  The text we want to display in the loading screen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bShowUIOverlay), Category = UI)
	FText LoadingText;

	/* The font used for the loading text  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = bShowUIOverlay), Category = UI)
	FSlateFontInfo LoadingFont;

	UUserWidget* GetLoadingScreenWidget(UObject* WorldContextObject) const;
};

/**
 * A Level associated with a UI Friendly FText variable
 */
USTRUCT(BlueprintType)
struct LOADINGSCREEN_API FLevelLoadScreenData
{
	GENERATED_BODY()

	FLevelLoadScreenData()
	{}

	FLevelLoadScreenData(const FString& LevelPath)
	: FriendlyName()
	{
		Level.FilePath = LevelPath;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RelativeToGameDir, FilePathFilter = "umap"), Category = LevelData)
	FFilePath Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LevelData)
	FText FriendlyName;

	FORCEINLINE bool operator==(const FLevelLoadScreenData& Other) const
	{
		return Level.FilePath == Other.Level.FilePath;
	}

	friend FORCEINLINE uint32 GetTypeHash(const FLevelLoadScreenData& Other)
	{
		return GetTypeHash(Other.Level.FilePath);
	}
};