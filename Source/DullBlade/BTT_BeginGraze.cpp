// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_BeginGraze.h"
#include "AIControllerBase.h"
#include "Deer.h"

UBTT_BeginGraze::UBTT_BeginGraze(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "BeginGraze";
}

EBTNodeResult::Type UBTT_BeginGraze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	ADeer* MyAIPawn = Cast<ADeer>(MyController->GetPawn());

	if (MyAIPawn)
	{
		MyAIPawn->ToggleGrazing();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
