// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/HunterAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "HunterEquipmentManagerComponent.generated.h"

#define UE_API HUNTERGAME_API

class UActorComponent;
class UHunterAbilitySystemComponent;
class UHunterEquipmentDefinition;
class UHunterEquipmentInstance;
class UHunterEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FHunterEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FHunterAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FHunterAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FHunterEquipmentList;
	friend UHunterEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UHunterEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UHunterEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FHunterAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FHunterEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FHunterEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FHunterEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FHunterAppliedEquipmentEntry, FHunterEquipmentList>(Entries, DeltaParms, *this);
	}

	UHunterEquipmentInstance* AddEntry(TSubclassOf<UHunterEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UHunterEquipmentInstance* Instance);

private:
	UHunterAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UHunterEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FHunterAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FHunterEquipmentList> : public TStructOpsTypeTraitsBase2<FHunterEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages equipment applied to a pawn
 */
UCLASS(MinimalAPI, BlueprintType, Const)
class UHunterEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UE_API UHunterEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UE_API UHunterEquipmentInstance* EquipItem(TSubclassOf<UHunterEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UE_API void UnequipItem(UHunterEquipmentInstance* ItemInstance);

	//~UObject interface
	UE_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	UE_API virtual void InitializeComponent() override;
	UE_API virtual void UninitializeComponent() override;
	UE_API virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UE_API UHunterEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UHunterEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	UE_API TArray<UHunterEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UHunterEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FHunterEquipmentList EquipmentList;
};

#undef UE_API
