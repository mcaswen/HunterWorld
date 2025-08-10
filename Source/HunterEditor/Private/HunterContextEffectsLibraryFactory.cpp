// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterContextEffectsLibraryFactory.h"

#include "Feedback/ContextEffects/HunterContextEffectsLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterContextEffectsLibraryFactory)

class FFeedbackContext;
class UClass;
class UObject;

UHunterContextEffectsLibraryFactory::UHunterContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UHunterContextEffectsLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UHunterContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UHunterContextEffectsLibrary* HunterContextEffectsLibrary = NewObject<UHunterContextEffectsLibrary>(InParent, Name, Flags);

	return HunterContextEffectsLibrary;
}
