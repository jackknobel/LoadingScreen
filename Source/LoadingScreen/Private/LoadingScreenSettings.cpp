// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenSettings.h"
#include "LoadingScreenWidget.h"

// UE Includes
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

FLoadingScreenDescription::FLoadingScreenDescription()
	: MinimumLoadingScreenDisplayTime(-1)
	, bAutoCompleteWhenLoadingCompletes(true)
	, bMoviesAreSkippable(true)
	, bWaitForManualStop(false)
	, bShowUIOverlay(true)
	, LoadingText(LOCTEXT("Loading", "LOADING"))
{
	if (!IsRunningDedicatedServer())
	{
		LoadingFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 32);
	}
}

ULoadingScreenWidget* FLoadingScreenDescription::GetLoadingScreenWidget(UObject* WorldContextObject) const
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World != nullptr && UIOverlayClass != nullptr)
	{
		ULoadingScreenWidget* LoadingWidget = CreateWidget<ULoadingScreenWidget>(World, UIOverlayClass);
		LoadingWidget->LoadingScreenDescription = *this;
		return LoadingWidget;
	}
	return nullptr;
}

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	TipWrapAt = 1000.0f;

	if (!IsRunningDedicatedServer())
	{
		TipFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 32);
	}
}

ULoadingScreenSettings* ULoadingScreenSettings::GetMutableLoadingScreenSettings()
{
	return GetMutableDefault<ULoadingScreenSettings>();
}

const ULoadingScreenSettings* ULoadingScreenSettings::GetLoadingScreenSettings()
{
	return GetDefault<ULoadingScreenSettings>();
}

#undef LOCTEXT_NAMESPACE