// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LockingDoor.generated.h"

UCLASS()
class C77_RESUME_API ALockingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALockingDoor();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void LockDoor(bool bLock);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* DoorCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	bool bIsLocked = true;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void LockDoor_Cosmetic();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void UnlockDoor_Cosmetic();

	virtual void BeginPlay() override;
};
