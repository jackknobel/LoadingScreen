// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenTypes.h"
#include "LoadingScreenWidgetInterface.h"

// UE Includes
#include "UserWidget.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

FLoadingScreenDescription::FLoadingScreenDescription()
	: MinimumLoadingScreenDisplayTime(-1)
	, bAutoCompleteWhenLoadingCompletes(true)
	, bMoviesAreSkippable(true)
	, bWaitForManualStop(false)
	, PlaybackType(EMoviePlaybackType::MT_Normal)
	, bShowUIOverlay(true)
	, LoadingText(LOCTEXT("Loading", "LOADING"))
{
	if (!IsRunningDedicatedServer())
	{
		LoadingFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 32);
	}
}

UUserWidget* FLoadingScreenDescription::GetLoadingScreenWidget(UObject* WorldContextObject) const
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr && UIOverlayClass != nullptr)
	{
		UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(World, UIOverlayClass);
		ILoadingScreenWidgetInterface::Execute_SetLoadingScreenDescription(LoadingWidget, *this);
		return LoadingWidget;
	}
	return nullptr;
}

#undef LOCTEXT_NAMESPACE