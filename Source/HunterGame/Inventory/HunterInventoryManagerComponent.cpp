// Copyright Epic Games, Inc. All Rights Reserved.

#include "HunterInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "HunterInventoryItemDefinition.h"
#include "HunterInventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HunterInventoryManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Hunter_Inventory_Message_StackChanged, "Hunter.Inventory.Message.StackChanged");

//////////////////////////////////////////////////////////////////////
// FHunterInventoryEntry

FString FHunterInventoryEntry::GetDebugString() const
{
	TSubclassOf<UHunterInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FHunterInventoryList

void FHunterInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FHunterInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FHunterInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FHunterInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FHunterInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FHunterInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FHunterInventoryList::BroadcastChangeMessage(FHunterInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FHunterInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_Hunter_Inventory_Message_StackChanged, Message);
}

UHunterInventoryItemInstance* FHunterInventoryList::AddEntry(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UHunterInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
 	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FHunterInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UHunterInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UHunterInventoryItemFragment* Fragment : GetDefault<UHunterInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	//const UHunterInventoryItemDefinition* ItemCDO = GetDefault<UHunterInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FHunterInventoryList::AddEntry(UHunterInventoryItemInstance* Instance)
{
	unimplemented();
}

void FHunterInventoryList::RemoveEntry(UHunterInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FHunterInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UHunterInventoryItemInstance*> FHunterInventoryList::GetAllItems() const
{
	TArray<UHunterInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FHunterInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UHunterInventoryManagerComponent

UHunterInventoryManagerComponent::UHunterInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UHunterInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UHunterInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UHunterInventoryItemInstance* UHunterInventoryManagerComponent::AddItemDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UHunterInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UHunterInventoryManagerComponent::AddItemInstance(UHunterInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UHunterInventoryManagerComponent::RemoveItemInstance(UHunterInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UHunterInventoryItemInstance*> UHunterInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UHunterInventoryItemInstance* UHunterInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef) const
{
	for (const FHunterInventoryEntry& Entry : InventoryList.Entries)
	{
		UHunterInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UHunterInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FHunterInventoryEntry& Entry : InventoryList.Entries)
	{
		UHunterInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UHunterInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UHunterInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UHunterInventoryItemInstance* Instance = UHunterInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void UHunterInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UHunterInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FHunterInventoryEntry& Entry : InventoryList.Entries)
		{
			UHunterInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UHunterInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FHunterInventoryEntry& Entry : InventoryList.Entries)
	{
		UHunterInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

//////////////////////////////////////////////////////////////////////
//

// UCLASS(Abstract)
// class UHunterInventoryFilter : public UObject
// {
// public:
// 	virtual bool PassesFilter(UHunterInventoryItemInstance* Instance) const { return true; }
// };

// UCLASS()
// class UHunterInventoryFilter_HasTag : public UHunterInventoryFilter
// {
// public:
// 	virtual bool PassesFilter(UHunterInventoryItemInstance* Instance) const { return true; }
// };


