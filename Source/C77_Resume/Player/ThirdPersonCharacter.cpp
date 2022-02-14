// Fill out your copyright notice in the Description page of Project Settings.

#include "C77_Resume/Player/ThirdPersonCharacter.h"
#include "C77_Resume/Interfaces/Interactable.h"

#include "Components/CapsuleComponent.h"


AThirdPersonCharacter::AThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThirdPersonCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ============================================
	// ============[ SERVER_ONLY_CODE ]============
	// ============================================

	if (IsLocallyControlled())
	{
		SetFocusInteractables(true);
	}
}

void AThirdPersonCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// ============================================
	// ============[ CLIENT_ONLY_CODE ]============
	// ============================================

	if (IsLocallyControlled())
	{
		SetFocusInteractables(true);
	}
}

// =================================================================
// ==========================[ Targeting ]==========================
// =================================================================
#pragma region Targeting

void AThirdPersonCharacter::GetNextTarget()
{
	FVector LineTraceEnd = GetActorLocation() + (GetControlRotation().RotateVector(FVector::ForwardVector * TargetingDistance));

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Untarget current target and make sure we dont target them again
	if (IsValid(CurrentTarget))
	{
		Params.AddIgnoredActor(CurrentTarget);
		CurrentTarget->Targeted_Cosmetic(false);
		CurrentTarget = nullptr;
	}

	FHitResult Hit;

	// This could be improved by using a sphere line trace to get a wider angle of targeting
	if (GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetActorLocation(),
		LineTraceEnd,
		ECollisionChannel::ECC_Pawn,
		Params
	))
	{
		if (auto Char = Cast<AThirdPersonCharacter>(Hit.Actor))
		{
			CurrentTarget = Char;
			CurrentTarget->Targeted_Cosmetic(true);
		}
	}
}

#pragma endregion Targeting


// =================================================================
// =========================[ Interaction ]=========================
// =================================================================
#pragma region Interaction

void AThirdPersonCharacter::Server_Interact_Implementation()
{
	if (auto Interactable = GetClosestInteractable())
	{
		IInteractable::Execute_Interact(Interactable);
	}
}

AActor* AThirdPersonCharacter::GetClosestInteractable() const
{
	TSet<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	AActor* ClosestInteractable = nullptr;
	const FVector MyPos = GetActorLocation();
	float SmallestDist = BIG_NUMBER;

	for (auto Interactable : OverlappingActors)
	{
		if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			float NewDist = FVector::DistSquared(MyPos, Interactable->GetActorLocation());
			if (NewDist < SmallestDist)
			{
				SmallestDist = NewDist;
				ClosestInteractable = Interactable;
			}
		}
	}

	return ClosestInteractable;
}

void AThirdPersonCharacter::SetFocusInteractables(bool bFocus)
{
	if (bFocus)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractableCheckTimer, this, &AThirdPersonCharacter::FocusClosestInteractable, 0.2f, true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractableCheckTimer);

		if (CurrentInteractableFocus != nullptr)
		{
			IInteractable::Execute_SetIsFocus(CurrentInteractableFocus, false);
			CurrentInteractableFocus = nullptr;
		}
	}
}

void AThirdPersonCharacter::FocusClosestInteractable()
{
	if (IsLocallyControlled())
	{
		auto ClosestActor = GetClosestInteractable();

		// Nothing in range
		if (ClosestActor == nullptr)
		{
			// Did we have a target before?  If so, tell them they are no longer the focus
			if (CurrentInteractableFocus != nullptr)
			{
				IInteractable::Execute_SetIsFocus(CurrentInteractableFocus, false);
				CurrentInteractableFocus = nullptr;
			}
		}
		// Something in range
		else
		{
			// There's a new target
			if (ClosestActor != CurrentInteractableFocus)
			{
				// There was a previous target, tell them they are no longer focus
				if (CurrentInteractableFocus != nullptr)
				{
					IInteractable::Execute_SetIsFocus(CurrentInteractableFocus, false);
				}

				CurrentInteractableFocus = ClosestActor;
				IInteractable::Execute_SetIsFocus(CurrentInteractableFocus, true);
			}
		}
	}
	else
	{
		SetFocusInteractables(false);
	}
}

#pragma endregion Interaction