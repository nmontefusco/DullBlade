// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "EnemyBase.generated.h"

class UHealthComponent;
class USoundBase;

UCLASS()
class DULLBLADE_API AEnemyBase : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	// Interaction Interface
	virtual bool BeginFocus() const override;
	virtual bool EndFocus() const override;
	virtual void OnClicked(APlayerController* PlayerCharacter) override; 
	// End Interaction Interface

	bool IsDead() { return bDied; }
	bool HasTakenDamage() { return bTakedamage; }
	
protected:
	virtual void BeginPlay() override;

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDied;

	/** */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTakedamage;
	
	/** */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComp;

	/** */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* MeleeCollider;


	/** When this enemy character health has changed
	* @param EnemyHealthComp:
	  @param Health:
	  @param HealthDelta:
	  @param DamageType:
	  @param InstigatedBy:
	  @param DamageCauser:
	  */

	UFUNCTION(BlueprintCallable)
	void OnHealthChanged(UHealthComponent* EnemyHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* pComponent);

	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* pComponent);

	UFUNCTION(BlueprintCallable)
	void ResetTakeDamage();

	UFUNCTION(BlueprintCallable)
	void PlayFootPrint(bool isHeavy);

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* TakeDamageMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DamagedSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FootPrintSoundLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FootPrintSoundHeavy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DeathIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
