// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "HunterVerbMessage.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "HunterVerbMessageReplication.generated.h"

class UObject;
struct FHunterVerbMessageReplication;
struct FNetDeltaSerializeInfo;

/**
 * Represents one verb message
 */
USTRUCT(BlueprintType)
struct FHunterVerbMessageReplicationEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FHunterVerbMessageReplicationEntry()
	{}

	FHunterVerbMessageReplicationEntry(const FHunterVerbMessage& InMessage)
		: Message(InMessage)
	{
	}

	FString GetDebugString() const;

private:
	friend FHunterVerbMessageReplication;

	UPROPERTY()
	FHunterVerbMessage Message;
};

/** Container of verb messages to replicate */
USTRUCT(BlueprintType)
struct FHunterVerbMessageReplication : public FFastArraySerializer
{
	GENERATED_BODY()

	FHunterVerbMessageReplication()
	{
	}

public:
	void SetOwner(UObject* InOwner) { Owner = InOwner; }

	// Broadcasts a message from server to clients
	void AddMessage(const FHunterVerbMessage& Message);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FHunterVerbMessageReplicationEntry, FHunterVerbMessageReplication>(CurrentMessages, DeltaParms, *this);
	}

private:
	void RebroadcastMessage(const FHunterVerbMessage& Message);

private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FHunterVerbMessageReplicationEntry> CurrentMessages;
	
	// Owner (for a route to a world)
	UPROPERTY()
	TObjectPtr<UObject> Owner = nullptr;
};

template<>
struct TStructOpsTypeTraits<FHunterVerbMessageReplication> : public TStructOpsTypeTraitsBase2<FHunterVerbMessageReplication>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
