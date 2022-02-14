// Fill out your copyright notice in the Description page of Project Settings.

#include "C77_Resume/Doors/LockingDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

ALockingDoor::ALockingDoor()
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

void ALockingDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ALockingDoor, bIsLocked, COND_InitialOnly);
}

void ALockingDoor::LockDoor_Implementation(bool bLock)
{
	UE_LOG(LogTemp, Warning, TEXT("hmmm"));
	if (bIsLocked != bLock)
	{
		bIsLocked = bLock;
		DoorCollision->SetCollisionEnabled(!bIsLocked ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);

		// Only draw collision on server
		if (HasAuthority())
		{
			UKismetSystemLibrary::FlushPersistentDebugLines(this);
			UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation(), DoorCollision->GetScaledBoxExtent(), !bIsLocked ? FLinearColor::Green : FLinearColor::Red, GetActorRotation(), 999.0f, 5.0f);
		}
		else // This else is just here to keep the listen server from running the cosmetic code for visual purposes
		{
			if (bIsLocked)
			{
				LockDoor_Cosmetic();
			}
			else
			{
				UnlockDoor_Cosmetic();
			}
		}
	}
}

// Called when the game starts or when spawned
void ALockingDoor::BeginPlay()
{
	Super::BeginPlay();
	
	// Draw the doors collision so we can visualize it
	if (HasAuthority())
	{
		UKismetSystemLibrary::DrawDebugBox(this, GetActorLocation(), DoorCollision->GetScaledBoxExtent(), FLinearColor::Red, GetActorRotation(), 999.0f, 5.0f);
		StaticMesh->SetVisibility(false);
	}

	// because bIsOpen does not have an OnRep, we need to set collision on the server and client in BeginPlay
	DoorCollision->SetCollisionEnabled(!bIsLocked ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
}

