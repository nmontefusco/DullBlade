// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIControllerBase.generated.h"


UCLASS()
class DULLBLADE_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

	AAIControllerBase();

	void OnPossess(APawn* InPawn) override;

	void OnUnPossess() override;

public:

	virtual FGenericTeamId GetGenericTeamId() const override;

	FORCEINLINE class UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp;}

	FORCEINLINE class UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp;}

	bool bCanSeePlayer;

	FVector LastKnownPlayerLocation;

protected:

	 UBehaviorTreeComponent* BehaviorComp;

	 UBlackboardComponent* BlackboardComp;
	
	 UPROPERTY(EditAnywhere)
	 class UBehaviorTree* BehaviorTree;
	 
	 class UAISenseConfig_Sight*  SightConfig;

	 class UAISenseConfig_Hearing* HearingConfig;

	 UFUNCTION()
		 void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
