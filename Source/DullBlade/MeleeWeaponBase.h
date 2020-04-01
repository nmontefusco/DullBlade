// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeWeaponBase.generated.h"

class USoundBase;

UCLASS()
class DULLBLADE_API AMeleeWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeaponBase();

	UFUNCTION()
	void SetTargetActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void PlayWeaponSwingSound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent*  CapsuleComp;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float LastMeleeAttackTime;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MeleeCoolDown;

	UPROPERTY(EditAnywhere)
	AActor* TargetedActor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USoundBase* WeaponSwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* WeaponImpactSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
