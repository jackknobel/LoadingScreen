// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ILoadingScreenModule.h"
#include "LoadingScreenSettings.h"
#include "LoadingScreenWidgetInterface.h"

// UE Includes
#include "Framework/Application/SlateApplication.h"
#include "Private/DefaultGameMoviePlayer.h"
#include "UserWidget.h"
#include "Engine/Engine.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool IsGameModule() const override
	{
		return true;
	}

private:

	UWorld* GetModuleWorld() const;

	void HandlePrepareLoadingScreen();

	void BeginLoadingScreen(FLoadingScreenDescription& ScreenDescription);

	// Used to update our loading screen widget with the map we're loading
	void OnPreLoadMap(const FString& MapName);

	// Only valid while we're loading
	TWeakObjectPtr<UUserWidget> LoadingScreenWidget;
};

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)

UWorld* FLoadingScreenModule::GetModuleWorld() const
{
	if (GEngine != nullptr)
	{
		const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : WorldContexts)
		{
			if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
			{
				return Context.World();
			}
		}
	}
	return nullptr;
}

void FLoadingScreenModule::StartupModule()
{
	if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen.
		ULoadingScreenSettings* LoadingScreenSettings = GetMutableDefault<ULoadingScreenSettings>();
		if (LoadingScreenSettings->bUseStartupScreen)
		{		
			BeginLoadingScreen(LoadingScreenSettings->StartupScreen);
		}

		FCoreUObjectDelegates::PreLoadMap.AddRaw(this, &FLoadingScreenModule::OnPreLoadMap);
	}
}

void FLoadingScreenModule::ShutdownModule()
{
	if (!IsRunningDedicatedServer())
	{
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
}

void FLoadingScreenModule::OnPreLoadMap(const FString& MapName)
{
	if (LoadingScreenWidget.IsValid())
	{
		ILoadingScreenWidgetInterface::Execute_SetLoadingLevelName(LoadingScreenWidget.Get(), MapName);
	}
}

void FLoadingScreenModule::HandlePrepareLoadingScreen()
{
	BeginLoadingScreen(GetMutableDefault<ULoadingScreenSettings>()->DefaultScreen);
}

void FLoadingScreenModule::BeginLoadingScreen(FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime	= ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable				= ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop				= ScreenDescription.bWaitForManualStop;
	LoadingScreen.MoviePaths						= ScreenDescription.MoviePaths;
	LoadingScreen.PlaybackType						= ScreenDescription.PlaybackType;

	UWorld* ModuleWorld = GetModuleWorld();

	if (ModuleWorld != nullptr && ScreenDescription.bShowUIOverlay && ScreenDescription.UIOverlayClass != nullptr)
	{
		LoadingScreenWidget = ScreenDescription.GetLoadingScreenWidget(ModuleWorld);
		if (LoadingScreenWidget.IsValid())
		{
			LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();
		}
	}

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE