// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HunterNumberPopComponent.h"

#include "HunterNumberPopComponent_NiagaraText.generated.h"

class UHunterDamagePopStyleNiagara;
class UNiagaraComponent;
class UObject;

UCLASS(Blueprintable)
class UHunterNumberPopComponent_NiagaraText : public UHunterNumberPopComponent
{
	GENERATED_BODY()

public:

	UHunterNumberPopComponent_NiagaraText(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UHunterNumberPopComponent interface
	virtual void AddNumberPop(const FHunterNumberPopRequest& NewRequest) override;
	//~End of UHunterNumberPopComponent interface

protected:
	
	TArray<int32> DamageNumberArray;

	/** Style patterns to attempt to apply to the incoming number pops */
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UHunterDamagePopStyleNiagara> Style;

	//Niagara Component used to display the damage
	UPROPERTY(EditDefaultsOnly, Category = "Number Pop|Style")
	TObjectPtr<UNiagaraComponent> NiagaraComp;
};
