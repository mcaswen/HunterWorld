// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonUserWidget.h"
#include "Performance/HunterPerformanceStatTypes.h"

#include "HunterPerfStatContainerBase.generated.h"

class UObject;
struct FFrame;

/**
 * UHunterPerfStatsContainerBase
 *
 * Panel that contains a set of UHunterPerfStatWidgetBase widgets and manages
 * their visibility based on user settings.
 */
 UCLASS(Abstract)
class UHunterPerfStatContainerBase : public UCommonUserWidget
{
public:
	UHunterPerfStatContainerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	GENERATED_BODY()

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintCallable)
	void UpdateVisibilityOfChildren();

protected:
	// Are we showing text or graph stats?
	UPROPERTY(EditAnywhere, Category=Display)
	EHunterStatDisplayMode StatDisplayModeFilter = EHunterStatDisplayMode::TextAndGraph;
};
