// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "HunterSettingValueDiscrete_PerfStat.generated.h"

enum class EHunterDisplayablePerformanceStat : uint8;
enum class EHunterStatDisplayMode : uint8;

class UObject;

UCLASS()
class UHunterSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UHunterSettingValueDiscrete_PerfStat();

	void SetStat(EHunterDisplayablePerformanceStat InStat);

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	
	void AddMode(FText&& Label, EHunterStatDisplayMode Mode);
protected:
	TArray<FText> Options;
	TArray<EHunterStatDisplayMode> DisplayModes;

	EHunterDisplayablePerformanceStat StatToDisplay;
	EHunterStatDisplayMode InitialMode;
};
