// Fill out your copyright notice in the Description page of Project Settings.


#include "First/TestMyActor.h"


// Sets default values
ATestMyActor::ATestMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

