// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBase.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIControllerBase.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	MeleeCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ColliderComp"));

	MeleeCollider->SetupAttachment(RootComponent);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(HealthComp != nullptr))
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &AEnemyBase::OnHealthChanged);
	}	

	MeleeCollider->OnBeginCursorOver.AddDynamic(this, &AEnemyBase::OnBeginCursorOver);
	MeleeCollider->OnEndCursorOver.AddDynamic(this, &AEnemyBase::OnEndCursorOver);
	DeathIndex = FMath::RandRange(int32(0), int32(4));
}

void AEnemyBase::OnHealthChanged(UHealthComponent* EnemyHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	AController* MyAIController = Cast<AAIControllerBase>(GetController());
	
	if (Health <= 0)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
		GetMesh()->GetAnimInstance()->StopAllMontages(0.1f);
		bDied = true;
		SetActorEnableCollision(false);
		MyAIController->UnPossess();
	}
	
	
	if (!bDied)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DamagedSound, GetActorLocation());
		if (TakeDamageMontage)
		{
			PlayAnimMontage(TakeDamageMontage);
		}
	}
}

void AEnemyBase::OnBeginCursorOver(UPrimitiveComponent* pComponent)
{
	
	BeginFocus();
}

void AEnemyBase::OnEndCursorOver(UPrimitiveComponent* pComponent)
{

	EndFocus();
}

void AEnemyBase::ResetTakeDamage()
{
	bTakedamage = false;
}

void AEnemyBase::PlayFootPrint(bool isHeavy)
{
	if (isHeavy)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootPrintSoundHeavy, GetActorLocation());
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(this, FootPrintSoundLight, GetActorLocation());
}

bool AEnemyBase::BeginFocus() const
{
	USkeletalMeshComponent* CharMesh = GetMesh();

	if (CharMesh == nullptr)
	{
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("Came as True"));
	CharMesh->SetRenderCustomDepth(true);
	CharMesh->SetCustomDepthStencilValue(254);
	return true;
}

bool AEnemyBase::EndFocus() const
{
	USkeletalMeshComponent* CharMesh = GetMesh();
	if (CharMesh == nullptr)
	{
		return false;
	}	
	CharMesh->SetRenderCustomDepth(false);
	return true;
}

void AEnemyBase::OnClicked(APlayerController* PlayerCharacter)
{

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

