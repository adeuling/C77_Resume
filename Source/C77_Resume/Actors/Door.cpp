// Fill out your copyright notice in the Description page of Project Settings.


#include "C77_Resume/Actors/Door.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
	DoorCollision->SetCollisionProfileName(FName("BlockAllDynamic"), true);
	DoorCollision->SetMobility(EComponentMobility::Static);
	RootComponent = DoorCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	DoorCollision->SetCollisionProfileName(FName("NoCollision"), true);
	DoorCollision->SetupAttachment(RootComponent);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	// Normally this would use IsRunningDedicatedServer() so we keep is on listen servers, but I'm making it all servers for visual purposes
	if (bAutoOpen && !bCanLock && HasAuthority())
	{
		// If this door automatically opens and cannot lock, then theres no reason to even have the door on the server!
		Destroy();
	}

	//DoorCollision->SetCollisionEnabled(bLocked || )
}