// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateChasing.generated.h"

UCLASS()
class DULLBLADE_API UBTService_UpdateChasing : public UBTService
{
	GENERATED_BODY()

		UBTService_UpdateChasing(const FObjectInitializer & ObjectInitializer);

public:

	UPROPERTY(EditAnywhere, Category = PlayerClass)
		TSubclassOf<AActor> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector bHasLineOfSight;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector TargetActor;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		FBlackboardKeySelector Destination;

private:
	bool bLastCanSeePlayer;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual FString GetStaticDescription() const override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
