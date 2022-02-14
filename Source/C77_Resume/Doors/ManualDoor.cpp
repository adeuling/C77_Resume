// Fill out your copyright notice in the Description page of Project Settings.


#include "C77_Resume/Doors/ManualDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

AManualDoor::AManualDoor()
{
	PrimaryActorTick.bCanEverTick = false;

	DoorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollision"));
	DoorCollision->SetCollisionProfileName(FName("BlockAllDynamic"), true);
	DoorCollision->SetMobility(EComponentMobility::Static);
	RootComponent = DoorCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(FName("NoCollision"));
	StaticMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

void AManualDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AManualDoor, bIsOpen, COND_None);
}

void AManualDoor::OpenDoor(bool bOpen)
{
	if (bOpen != bIsOpen)
	{
		bIsOpen = bOpen;
		DoorCollision->SetCollisionEnabled(bIsOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

		UKismetSystemLibrary::FlushPersistentDebugLines(this);
		UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation(), DoorCollision->GetScaledBoxExtent(), bIsOpen ? FLinearColor::Green : FLinearColor::Red, GetActorRotation(), 999.0f, 5.0f);

		OnRep_IsOpen_Cosmetic();
	}
}

void AManualDoor::OnRep_IsOpen_Cosmetic()
{
	DoorCollision->SetCollisionEnabled(bIsOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

	if (bIsOpen)
	{
		OpenDoor_Cosmetic();
	}
	else
	{
		CloseDoor_Cosmetic();
	}
}

void AManualDoor::BeginPlay()
{
	Super::BeginPlay();

	// Draw the doors collision so we can visualize it
	if (HasAuthority())
	{
		UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation(), DoorCollision->GetScaledBoxExtent(), FLinearColor::Red, GetActorRotation(), 999.0f, 5.0f);
		StaticMesh->SetVisibility(false);

		// Set the initial collision of the door
		DoorCollision->SetCollisionEnabled(bIsOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	}
}