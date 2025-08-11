// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterGameplayEffectContext.h"

#include "AbilitySystem/LyraAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationState/PropertyNetSerializerInfoRegistry.h"
#include "Serialization/GameplayEffectContextNetSerializer.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterGameplayEffectContext)

class FArchive;

FHunterGameplayEffectContext* FHunterGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FHunterGameplayEffectContext::StaticStruct()))
	{
		return (FHunterGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

bool FHunterGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FHunterGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(HunterGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif

void FHunterGameplayEffectContext::SetAbilitySource(const ILyraAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const ILyraAbilitySourceInterface* FHunterGameplayEffectContext::GetAbilitySource() const
{
	return Cast<ILyraAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FHunterGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

