// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ILoadingScreenModule.h"
#include "LoadingScreenSettings.h"
#include "LoadingScreenWidget.h"

// UE Includes
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "LoadingScreen"

class FLoadingScreenModule : public ILoadingScreenModule
{
public:
	FLoadingScreenModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool IsGameModule() const override
	{
		return true;
	}

private:

	TWeakObjectPtr<UWorld> ModuleWorld;

	void HandlePrepareLoadingScreen();

	void BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription);
};

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen)

FLoadingScreenModule::FLoadingScreenModule()
{

}

void FLoadingScreenModule::StartupModule()
{
	if ( !IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
	{
		// Load for cooker reference
		const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
		for ( const FStringAssetReference& Ref : Settings->StartupScreen.Images )
		{
			Ref.TryLoad();
		}
		for ( const FStringAssetReference& Ref : Settings->DefaultScreen.Images )
		{
			Ref.TryLoad();
		}

		if (IsMoviePlayerEnabled())
		{
			GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FLoadingScreenModule::HandlePrepareLoadingScreen);
		}

		FWorldDelegates::OnPreWorldInitialization.AddLambda([this](UWorld* World, const UWorld::InitializationValues InitValues)
		{
			ModuleWorld = World;
		});

		// Prepare the startup screen, the PrepareLoadingScreen callback won't be called
		// if we've already explicitly setup the loading screen.
		BeginLoadingScreen(Settings->StartupScreen);
	}
}

void FLoadingScreenModule::ShutdownModule()
{
	if (!IsRunningDedicatedServer())
	{
		GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
	}

	FWorldDelegates::OnPreWorldInitialization.RemoveAll(this);
}

void FLoadingScreenModule::HandlePrepareLoadingScreen()
{
	const ULoadingScreenSettings* Settings = GetDefault<ULoadingScreenSettings>();
	BeginLoadingScreen(Settings->DefaultScreen);
}

void FLoadingScreenModule::BeginLoadingScreen(const FLoadingScreenDescription& ScreenDescription)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.MinimumLoadingScreenDisplayTime	= ScreenDescription.MinimumLoadingScreenDisplayTime;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = ScreenDescription.bAutoCompleteWhenLoadingCompletes;
	LoadingScreen.bMoviesAreSkippable				= ScreenDescription.bMoviesAreSkippable;
	LoadingScreen.bWaitForManualStop				= ScreenDescription.bWaitForManualStop;
	LoadingScreen.MoviePaths						= ScreenDescription.MoviePaths;
	LoadingScreen.PlaybackType						= ScreenDescription.PlaybackType;

	if (ModuleWorld.Get() != nullptr && ScreenDescription.bShowUIOverlay && ScreenDescription.UIOverlayClass != nullptr)
	{
		if (ULoadingScreenWidget* LoadingScreenWidget = ScreenDescription.GetLoadingScreenWidget(ModuleWorld.Get()))
		{
			LoadingScreen.WidgetLoadingScreen = LoadingScreenWidget->TakeWidget();
		}
	}

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

#undef LOCTEXT_NAMESPACE