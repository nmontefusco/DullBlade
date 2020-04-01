// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndGraze.h"
#include "AIControllerBase.h"
#include "Deer.h"

UBTTask_EndGraze::UBTTask_EndGraze(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "EndGraze";
}

EBTNodeResult::Type UBTTask_EndGraze::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIControllerBase* MyController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner());
	ADeer* MyAIPawn = Cast<ADeer>(MyController->GetPawn());

	if (MyAIPawn)
	{
		MyAIPawn->EndGrazing();
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

