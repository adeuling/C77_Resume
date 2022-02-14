// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CosmeticDoor.generated.h"

UCLASS()
class C77_RESUME_API ACosmeticDoor : public AActor
{
	GENERATED_BODY()
	
public:
	ACosmeticDoor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBoxComponent* DoorTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadOnly)
	bool bIsOpen = false;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void OpenDoor_Cosmetic();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
	void CloseDoor_Cosmetic();

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
