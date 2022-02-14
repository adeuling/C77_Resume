// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class C77_RESUME_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* DoorCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	// Whether or not the door needs to be interacted with to open
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	bool bAutoOpen = false;

	// Whether or not the door can lock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Settings")
	bool bCanLock = false;

	// The current locked state of the door
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Settings")
	bool bIsLocked = false;

	// The current open state of the door
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door Settings")
	bool bIsOpen = false;

	UFUNCTION()
	void OnRep_IsLocked() {};

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OpenDoor() {};

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void OpenDoor_Cosmetic();

};
