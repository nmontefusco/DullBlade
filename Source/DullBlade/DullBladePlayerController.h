// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DullBladePlayerController.generated.h"



UCLASS()
class ADullBladePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADullBladePlayerController();



protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	bool bMiddleButtonToggle;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void MoveFoward(float axisvalue);
	void MoveRight(float axisvalue);

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	void SetNewMoveDestinationAndAttack(const FVector DestLocation, AActor* Target);
	
	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void ToggleWalkRun();

	void Attack();

	void MouseZoomUp();
	void MouseZoomDown();

	void RotateCamera(float axisvalue);

	void ToggleCameraRotatePressed();
	void ToggleCameraRotateReleased();

	UPROPERTY(EditAnywhere)
		AActor* LastTargetedActor;

};


