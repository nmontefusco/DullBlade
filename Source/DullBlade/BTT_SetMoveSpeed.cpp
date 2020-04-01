// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetMoveSpeed.h"
#include "AIControllerBase.h"
#include "Deer.h"

UBTT_SetMoveSpeed::UBTT_SetMoveSpeed(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "SetMoveSpeed";
}

EBTNodeResult::Type UBTT_SetMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	ADeer* MyAIPawn = Cast<ADeer>(MyController->GetPawn());

	if (MyAIPawn)
	{
		float NewMovementSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
		MyAIPawn->UpdateMoveSpeed(NewMovementSpeed);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


