// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "HunterInventoryManagerComponent.generated.h"

#define UE_API HUNTERGAME_API

class UHunterInventoryItemDefinition;
class UHunterInventoryItemInstance;
class UHunterInventoryManagerComponent;
class UObject;
struct FFrame;
struct FHunterInventoryList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FHunterInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UHunterInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FHunterInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FHunterInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FHunterInventoryList;
	friend UHunterInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UHunterInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FHunterInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FHunterInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FHunterInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UHunterInventoryItemInstance*> GetAllItems() const;

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FHunterInventoryEntry, FHunterInventoryList>(Entries, DeltaParms, *this);
	}

	UHunterInventoryItemInstance* AddEntry(TSubclassOf<UHunterInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UHunterInventoryItemInstance* Instance);

	void RemoveEntry(UHunterInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FHunterInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UHunterInventoryManagerComponent;

private:
	// Replicated list of items
	UPROPERTY()
	TArray<FHunterInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FHunterInventoryList> : public TStructOpsTypeTraitsBase2<FHunterInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages an inventory
 */
UCLASS(MinimalAPI, BlueprintType)
class UHunterInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UE_API UHunterInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API bool CanAddItemDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API UHunterInventoryItemInstance* AddItemDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API void AddItemInstance(UHunterInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UE_API void RemoveItemInstance(UHunterInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	UE_API TArray<UHunterInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UE_API UHunterInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef) const;

	UE_API int32 GetTotalItemCountByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef) const;
	UE_API bool ConsumeItemsByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	UE_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	UE_API virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY(Replicated)
	FHunterInventoryList InventoryList;
};

#undef UE_API
