// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DullBladeCharacter.h"


AAIControllerBase::AAIControllerBase()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	SightConfig->SetMaxAge(3.f);
	SightConfig->SightRadius = 3000;
	SightConfig->LoseSightRadius = 3500;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing"));
	HearingConfig->SetMaxAge(0.3f);
	HearingConfig->HearingRange = 2500.f;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionComponent->ConfigureSense(*HearingConfig);
	
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdated);
}
	

void AAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		BlackboardComp->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorComp->StartTree(*BehaviorTree);
	}
}


void AAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();
	BehaviorComp->StopTree();
}

FGenericTeamId AAIControllerBase::GetGenericTeamId() const
{
	const FGenericTeamId ID(254);
	return ID;
}

void AAIControllerBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Cast<ADullBladeCharacter>(Actor)) 
	{
		bCanSeePlayer = Stimulus.WasSuccessfullySensed();
		LastKnownPlayerLocation = Stimulus.StimulusLocation;
	}
}


