// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterPawnData.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterPawnData)

UHunterPawnData::UHunterPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	DefaultCameraMode = nullptr;
}

