// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManualDoor.generated.h"

UCLASS()
class C77_RESUME_API AManualDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	AManualDoor();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(BlueprintCallable)
	void OpenDoor(bool bOpen);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* DoorCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_IsOpen_Cosmetic)
	bool bIsOpen = false;

	UFUNCTION(BlueprintCosmetic)
	void OnRep_IsOpen_Cosmetic();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void OpenDoor_Cosmetic();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void CloseDoor_Cosmetic();

	virtual void BeginPlay() override;
};
