// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


AMeleeWeaponBase::AMeleeWeaponBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	
	RootComponent = MeshComp;
	CapsuleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleComp->SetCapsuleHalfHeight(58.0f);
	CapsuleComp->SetCapsuleRadius(8.0f);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeaponBase::OnOverlapBegin); 
	this->SetActorEnableCollision(false);
}

void AMeleeWeaponBase::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* MyOwner = GetOwner();
	AActor* HitActor = SweepResult.GetActor();

	if (OtherActor == MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit OtherActor %s"),*MyOwner->GetName());
	}
	
	if (HitActor == TargetedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit %s"), *HitActor->GetName());
		TSubclassOf<UDamageType> DefaultMeleeDamage;
		UGameplayStatics::ApplyDamage(HitActor, 30.f, GetInstigatorController(), this, DefaultMeleeDamage);
		UGameplayStatics::PlaySoundAtLocation(this, WeaponImpactSound, GetActorLocation());
	}		
}
	
void AMeleeWeaponBase::SetTargetActor(AActor* TargetActor)
{
	TargetedActor = TargetActor;
}

void AMeleeWeaponBase::PlayWeaponSwingSound()
{
	if (WeaponSwingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSwingSound, GetActorLocation());
	}
}

void AMeleeWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

