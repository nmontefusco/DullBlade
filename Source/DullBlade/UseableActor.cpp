// Fill out your copyright notice in the Description page of Project Settings.

#include "UseableActor.h"
#include "MyWidgetComponent.h"
#include "ActorWidget.h"
#include "ConstructorHelpers.h"


// Sets default values
AUseableActor::AUseableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	DisplayWidget = CreateDefaultSubobject<UMyWidgetComponent>(TEXT("WidgetComp"));
	static ConstructorHelpers::FClassFinder<UActorWidget>WidgetBlueprint(TEXT("/Game/Blueprints/NewBlueprint1"));
	WidgetClass = WidgetBlueprint.Class;
	DisplayWidget->SetWidgetClass(WidgetClass);
	DisplayWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DisplayWidget->SetVisibility(false);
	DisplayWidget->SetDrawSize(FVector2D(100, 50));
	DisplayWidget->SetRelativeLocation(FVector(0, 0, 180));
	
}


bool AUseableActor::BeginFocus() const
{
	MeshComp->SetRenderCustomDepth(true);
	MeshComp->SetCustomDepthStencilValue(252);
	if (DisplayWidget)
	{
		DisplayWidget->SetVisibility(true);
	}
	
	bool Success = true;
	return Success;
}

bool AUseableActor::EndFocus() const
{
	MeshComp->SetRenderCustomDepth(false);
	if (DisplayWidget)
	{
		DisplayWidget->SetVisibility(false);
	}
	bool Success = true;
	return Success;
}



void AUseableActor::OnClicked(APlayerController* PlayerController)
{

}

// Called when the game starts or when spawned

void AUseableActor::BeginPlay()
{
	Super::BeginPlay();

	//We have to add our Dynamic function to the MeshComp in BeginPlay instead of the constructor.
	MeshComp->OnBeginCursorOver.AddDynamic(this, &AUseableActor::OnBeginCursorOver);
	MeshComp->OnEndCursorOver.AddDynamic(this, &AUseableActor::OnEndCursorOver);
}

void AUseableActor::OnBeginCursorOver(UPrimitiveComponent* pComponent)
{
	BeginFocus();
}

void AUseableActor::OnEndCursorOver(UPrimitiveComponent* pComponent)
{
	EndFocus();
}

// Called every frame
void AUseableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


FText AUseableActor::GetActorName()
{
	return Name;
}

