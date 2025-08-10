// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonListView.h"

#include "HunterListView.generated.h"

#define UE_API HUNTERGAME_API

class UUserWidget;
class ULocalPlayer;
class UHunterWidgetFactory;

UCLASS(MinimalAPI, meta = (DisableNativeTick))
class UHunterListView : public UCommonListView
{
	GENERATED_BODY()

public:
	UE_API UHunterListView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	UE_API virtual void ValidateCompiledDefaults(IWidgetCompilerLog& InCompileLog) const override;
#endif

protected:
	UE_API virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	//virtual bool OnIsSelectableOrNavigableInternal(UObject* SelectedItem) override;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category="Entry Creation")
	TArray<TObjectPtr<UHunterWidgetFactory>> FactoryRules;
};

#undef UE_API
