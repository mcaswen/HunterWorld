// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"

#include "HunterWeaponUserInterface.generated.h"

class UHunterWeaponInstance;
class UObject;
struct FGeometry;

UCLASS()
class UHunterWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UHunterWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UHunterWeaponInstance* OldWeapon, UHunterWeaponInstance* NewWeapon);

private:
	void RebuildWidgetFromWeapon();

private:
	UPROPERTY(Transient)
	TObjectPtr<UHunterWeaponInstance> CurrentInstance;
};
