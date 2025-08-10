// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterEquipmentDefinition.h"
#include "HunterEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterEquipmentDefinition)

UHunterEquipmentDefinition::UHunterEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UHunterEquipmentInstance::StaticClass();
}

