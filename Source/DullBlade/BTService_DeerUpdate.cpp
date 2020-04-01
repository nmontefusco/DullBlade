// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DeerUpdate.h"
#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Deer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTService_DeerUpdate::UBTService_DeerUpdate(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Deer Brain";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;

	//Filtering our BlackBoard Key Selectors
	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DeerUpdate, TargetActor), AActor::StaticClass());
	Destination.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DeerUpdate, Destination));
	bHasLineOfSight.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DeerUpdate, bHasLineOfSight));
	GrazeTime.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DeerUpdate, GrazeTime));
}

void UBTService_DeerUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == NULL)
	{
		return;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIControllerBase* EnemyAIController = Cast<AAIControllerBase>(AIController);
	ADeer* DeerEnemy = Cast<ADeer>(EnemyAIController->GetPawn());

	//Update the Blackboard with the current value of bCanSeePlayer from the EnemyAI Controller.
	BlackboardComp->SetValueAsBool(bHasLineOfSight.SelectedKeyName, EnemyAIController->bCanSeePlayer);

	//If the LastCanSeePlayer is different from the current one, then we update the Destination
	if (EnemyAIController->bCanSeePlayer != bLastCanSeePlayer)
	{
		BlackboardComp->SetValueAsVector(Destination.SelectedKeyName, EnemyAIController->LastKnownPlayerLocation);
	}

	//Update the Service's bLastCanSeePlayer with the current Deer's bCanSeePlayer
	bLastCanSeePlayer = EnemyAIController->bCanSeePlayer;

	//Update the Service's GrazeTime with the current Deer's GrazeTime.
	BlackboardComp->SetValueAsFloat(GrazeTime.SelectedKeyName, DeerEnemy->GrazingTime);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTService_DeerUpdate::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: '%s'"), TEXT("Player Class"), PlayerClass ? *PlayerClass->GetName() : TEXT(""))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("TargetActor"), TargetActor.IsSet() ? *TargetActor.SelectedKeyName.ToString() : TEXT("")))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("LastKnownPositionKey"), Destination.IsSet() ? *Destination.SelectedKeyName.ToString() : TEXT("")))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("CanSeePlayerKey"), bHasLineOfSight.IsSet() ? *bHasLineOfSight.SelectedKeyName.ToString() : TEXT("")))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("GrazeTime"), GrazeTime.IsSet() ? *GrazeTime.SelectedKeyName.ToString() : TEXT("")));
}

void UBTService_DeerUpdate::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == NULL)
	{
		return;
	}

	if (!TargetActor.IsSet()) {
		//Retrieve Player and Update the Blackboard
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerClass, FoundActors);
		if (FoundActors[0]) {
			BlackboardComp->SetValueAsObject(TargetActor.SelectedKeyName, FoundActors[0]);
		}
	}
}
