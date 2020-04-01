// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateChasing.h"
#include "AIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTService_UpdateChasing::UBTService_UpdateChasing(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Update Chasing Behavior";

	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = false;

	//Filtering our BlackBoard Key Selectors
	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateChasing, TargetActor), AActor::StaticClass());
	Destination.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateChasing, Destination));
	bHasLineOfSight.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateChasing, bHasLineOfSight));
}

void UBTService_UpdateChasing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == NULL)
	{
		return;
	}
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	AAIControllerBase* EnemyAIController = Cast<AAIControllerBase>(AIController);

	//Update the Blackboard with the current value of bCanSeePlayer from the EnemyAI Controller.
	BlackboardComp->SetValueAsBool(bHasLineOfSight.SelectedKeyName, EnemyAIController->bCanSeePlayer);

	//If the LastCanSeePlayer is different from the current one, then we update the Destination
	if (EnemyAIController->bCanSeePlayer != bLastCanSeePlayer)
	{
		BlackboardComp->SetValueAsVector(Destination.SelectedKeyName, EnemyAIController->LastKnownPlayerLocation);
	}

	//Update the Service's bLastCanSeePlayer with the current EnemyAI's bCanSeePlayer
	bLastCanSeePlayer = EnemyAIController->bCanSeePlayer;
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

FString UBTService_UpdateChasing::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: '%s'"), TEXT("Player Class"), PlayerClass ? *PlayerClass->GetName() : TEXT(""))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("TargetActor"), TargetActor.IsSet() ? *TargetActor.SelectedKeyName.ToString() : TEXT("")))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("LastKnownPositionKey"), Destination.IsSet() ? *Destination.SelectedKeyName.ToString() : TEXT("")))
		.Append(FString::Printf(TEXT("\n%s: '%s'"), TEXT("CanSeePlayerKey"), bHasLineOfSight.IsSet() ? *bHasLineOfSight.SelectedKeyName.ToString() : TEXT("")));
}

void UBTService_UpdateChasing::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
