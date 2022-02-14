// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"


UCLASS()
class C77_RESUME_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AThirdPersonCharacter();

	// Called when this Pawn is possessed. Only called on the server.
	virtual void PossessedBy(AController* NewController) override;

	// Called when the player state is replicated. Only called on clients.
	virtual void OnRep_PlayerState() override;


// =================================================================
// ==========================[ Targeting ]==========================
// =================================================================
#pragma region Targeting
public:
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void GetNextTarget();

protected:
	// How far to trace to find a target
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float TargetingDistance = 2000.0f;

	// Locally cached current target so we can untarget them
	UPROPERTY(BlueprintReadOnly, Category = "Targeting")
	AThirdPersonCharacter* CurrentTarget = nullptr;

	// Called on local client when they target/untarget a ThirdPersonCharacter
	UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Targeting")
	void Targeted_Cosmetic(bool bIsTargeted);

#pragma endregion Targeting

// =================================================================
// =========================[ Interaction ]=========================
// =================================================================
#pragma region Interaction
public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Interaction")
	void Server_Interact();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetClosestInteractable() const;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetFocusInteractables(bool bFocus);

private:
	UPROPERTY()
	AActor* CurrentInteractableFocus = nullptr;

	FTimerHandle InteractableCheckTimer;

	UFUNCTION(BlueprintCosmetic)
	void FocusClosestInteractable();

#pragma endregion Interaction

};
