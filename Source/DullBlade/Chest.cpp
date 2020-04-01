
// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyWidgetComponent.h"


AChest::AChest()
{
	Lid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidComp"));
	Lid->AttachToComponent(MeshComp, FAttachmentTransformRules::KeepRelativeTransform);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);

	MyTimeLine = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));
}

bool AChest::BeginFocus() const
{

	if (DisplayWidget)
	{
		DisplayWidget->SetVisibility(true);
	}
	MeshComp->SetRenderCustomDepth(true);
	Lid->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(252);
	Lid->SetCustomDepthStencilValue(252);
	return true;
}

bool AChest::EndFocus() const
{
	if (DisplayWidget)
	{
		DisplayWidget->SetVisibility(false);
	}

	MeshComp->SetRenderCustomDepth(false);
	Lid->SetRenderCustomDepth(false);
	return true;
}

void AChest::OnClicked(APlayerController* PlayerCharacter)
{
	APawn* PlayerChar = PlayerCharacter->GetPawn();
	
	float Distance = FVector::Dist(PlayerChar->GetActorLocation(), GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("My Distance is %f"),Distance);
	bWasUsed = true;

	if (Distance<=180)
	{
		PlayAnimationTimeline();
	}
}

void AChest::OnBeginCursorOver(UPrimitiveComponent* pComponent)
{
	BeginFocus();
}

void AChest::OnEndCursorOver(UPrimitiveComponent* pComponent)
{
	EndFocus();
}

void AChest::BeginPlay()
{
	Super::BeginPlay();

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AChest::OnChestOverlapBegin);
	OverlapBox->OnBeginCursorOver.AddDynamic(this, &AChest::OnBeginCursorOver);
	OverlapBox->OnEndCursorOver.AddDynamic(this, &AChest::OnEndCursorOver);

	FOnTimelineFloat ProgressFunction;
	FOnTimelineEventStatic TimelineFinishedFunction;

	ProgressFunction.BindUFunction(this, FName("HandleAnimationProgress"));
	TimelineFinishedFunction.BindUFunction(this, FName("HandleTimeLineFinished"));

	if (MyCurveFloat)
	{
		MyTimeLine->AddInterpFloat(MyCurveFloat, ProgressFunction);
	}
	MyTimeLine->SetLooping(false);

}

void AChest::HandleAnimationProgress(float value)
{
	FRotator MyRotator(0,0,value);
	Lid->SetRelativeRotation(MyRotator);
	UE_LOG(LogTemp, Warning, TEXT("Timeline Has Started"));
}

void AChest::HandleTimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("TimeLine Has Ended"));
}

void AChest::OnChestOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bWasUsed)
	{
		PlayAnimationTimeline();
	}
}

void AChest::PlayAnimationTimeline()
{
	UGameplayStatics::PlaySoundAtLocation(this, ChestSound, GetActorLocation());
	MyTimeLine->Play();
	SetActorEnableCollision(false);
	
}
