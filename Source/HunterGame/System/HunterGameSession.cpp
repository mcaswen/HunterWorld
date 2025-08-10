// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterGameSession.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterGameSession)


AHunterGameSession::AHunterGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool AHunterGameSession::ProcessAutoLogin()
{
	// This is actually handled in HunterGameMode::TryDedicatedServerLogin
	return true;
}

void AHunterGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void AHunterGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

