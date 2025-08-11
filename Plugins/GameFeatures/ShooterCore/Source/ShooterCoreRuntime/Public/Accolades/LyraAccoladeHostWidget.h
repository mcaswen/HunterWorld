
#pragma once

#include "Accolades/HunterAccoladeDefinition.h"
#include "AsyncMixin.h"
#include "CommonUserWidget.h"
#include "GameFramework/GameplayMessageSubsystem.h"

#include "HunterAccoladeHostWidget.generated.h"

class UObject;
class USoundBase;
class UUserWidget;
struct FDataRegistryAcquireResult;
struct FHunterNotificationMessage;

USTRUCT(BlueprintType)
struct FPendingAccoladeEntry
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly)
	FHunterAccoladeDefinitionRow Row; 

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> Icon = nullptr;

	UPROPERTY()
	TObjectPtr<UUserWidget> AllocatedWidget = nullptr;

	int32 SequenceID = 0;

	bool bFinishedLoading = false;

	void CancelDisplay();
};

/**
 * 
 */
UCLASS(BlueprintType)
class UHunterAccoladeHostWidget : public UCommonUserWidget, public FAsyncMixin
{
	GENERATED_BODY()

public:
	// The location tag (used to filter incoming messages to only display the appropriate accolades in a given location)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag LocationName;

	//~UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//~End of UUserWidget interface

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyAccoladeWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintImplementableEvent)
	UUserWidget* CreateAccoladeWidget(const FPendingAccoladeEntry& Entry);
private:
	FGameplayMessageListenerHandle ListenerHandle;

	int32 NextDisplaySequenceID = 0;
	int32 AllocatedSequenceID = 0;

	FTimerHandle NextTimeToReconsiderHandle;

	// List of async pending load accolades (which might come in the wrong order due to the row read)
	UPROPERTY(Transient)
	TArray<FPendingAccoladeEntry> PendingAccoladeLoads;

	// List of pending accolades (due to one at a time display duration; the first one in the list is the current visible one)
	UPROPERTY(Transient)
	TArray<FPendingAccoladeEntry> PendingAccoladeDisplays;


	void OnNotificationMessage(FGameplayTag Channel, const FHunterNotificationMessage& Notification);
	void OnRegistryLoadCompleted(const FDataRegistryAcquireResult& AccoladeHandle, int32 SequenceID);

	void ConsiderLoadedAccolades();
	void PopDisplayedAccolade();
	void ProcessLoadedAccolade(const FPendingAccoladeEntry& Entry);
	void DisplayNextAccolade();
};
