// Fill out your copyright notice in the Description page of Project Settings.

#include "C77_Resume/Doors/CosmeticDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

ACosmeticDoor::ACosmeticDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
	DoorTrigger->SetCollisionProfileName(FName("OverlapOnlyPawn"), true);
	DoorTrigger->SetMobility(EComponentMobility::Static);
	DoorTrigger->SetGenerateOverlapEvents(true);
	RootComponent = DoorTrigger;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(FName("NoCollision"));
	StaticMesh->SetupAttachment(RootComponent);

}

void ACosmeticDoor::BeginPlay()
{
	Super::BeginPlay();
	
	// Normally this would use IsRunningDedicatedServer() so we keep is on listen servers
	// but I'm making it all servers for visual purposes
	if (HasAuthority())
	{
		Destroy();
	}
	else
	{
		DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACosmeticDoor::OnOverlapBegin);
		DoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ACosmeticDoor::OnOverlapEnd);
	}
}

void ACosmeticDoor::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsOpen)
	{
		bIsOpen = true;
		OpenDoor_Cosmetic();
	}
}

void ACosmeticDoor::OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsOpen)
	{
		bIsOpen = false;
		CloseDoor_Cosmetic();
	}
}