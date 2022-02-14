// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class C77_RESUME_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void Interact();

	// Called when the interactable either receives or loses focus (only run on client)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintCosmetic, Category = "Interactable|Client")
	void SetIsFocus(bool bIsFocus);

	// Called when the interactable either receives or loses focus (only run on client)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, BlueprintCosmetic, Category = "Interactable|Client")
	void OnInteract();
};
