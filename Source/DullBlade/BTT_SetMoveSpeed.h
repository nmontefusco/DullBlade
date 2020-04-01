// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetMoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class DULLBLADE_API UBTT_SetMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

		UBTT_SetMoveSpeed(const FObjectInitializer& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float MinSpeed;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxSpeed;
};
