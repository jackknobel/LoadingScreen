// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "LoadingScreenSettings.h"
#include "LoadingScreenWidgetInterface.h"

// UE Includes
#include "Engine/Font.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

ULoadingScreenSettings::ULoadingScreenSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
	, bUseStartupScreen(true)
{
	if (!IsRunningDedicatedServer())
	{
		TipFont = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 32);
	}
}

FText ULoadingScreenSettings::GetLevelFriendlyName(const FString& LevelName)
{
	const ULoadingScreenSettings* LoadingScreenSettings = GetLoadingScreenSettings();

	FText LevelFriendlyName = FText::GetEmpty();

	FString CorrectedLevelName = LevelName.Replace(TEXT("/Game/"), TEXT("/Content/")).Append(FPackageName::GetMapPackageExtension());
	if (CorrectedLevelName.StartsWith("/"))
	{
		CorrectedLevelName.RemoveAt(0);
	}

	if (const FLevelLoadScreenData* FoundLevelLoadScreenData = LoadingScreenSettings->FriendlyLevelNames.Find(CorrectedLevelName))
	{
		LevelFriendlyName = FoundLevelLoadScreenData->FriendlyName;
	}

	return LevelFriendlyName;
}

#undef LOCTEXT_NAMESPACE