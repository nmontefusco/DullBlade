// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class DULLBLADE_API IInteractionInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
	virtual bool BeginFocus() const  { return false; }

	UFUNCTION()
	virtual bool EndFocus() const  { return false; }

	UFUNCTION()
	virtual void OnClicked(APlayerController* PlayerController) = 0;

	/*
	UFUNCTION()
	virtual float GetUseDistance() const = 0;

	UFUNCTION()
	virtual bool OnUsed() const = 0;
	
	UFUNCTION()
	virtual bool IsUsable() const = 0;
	*/
};
