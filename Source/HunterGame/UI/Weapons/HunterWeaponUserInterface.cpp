// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterWeaponUserInterface.h"

#include "Equipment/HunterEquipmentManagerComponent.h"
#include "GameFramework/Pawn.h"
#include "Weapons/HunterWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterWeaponUserInterface)

struct FGeometry;

UHunterWeaponUserInterface::UHunterWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UHunterWeaponUserInterface::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHunterWeaponUserInterface::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHunterWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UHunterEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UHunterEquipmentManagerComponent>())
		{
			if (UHunterWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UHunterWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UHunterWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					RebuildWidgetFromWeapon();
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}

void UHunterWeaponUserInterface::RebuildWidgetFromWeapon()
{
	
}

