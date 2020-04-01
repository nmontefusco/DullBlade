// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

APickupActor::APickupActor()
{
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}


void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &APickupActor::OnOverlapBegin);
}

void APickupActor::OnClicked(APlayerController* PlayerCharacter)
{
	bWasUsed = true;
	
	APawn* PlayerChar = PlayerCharacter->GetPawn();

	float Distance = FVector::Dist(PlayerChar->GetActorLocation(), GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("I'm %f away"),Distance);
	if (Distance<=160)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GoldSound, GetActorLocation());
		Killmyself();
	}
	
}


void APickupActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bWasUsed)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GoldSound, GetActorLocation());
		Killmyself();
	}	
}

void APickupActor::Killmyself()
{
	Destroy();
}
