

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility.h"
#include "DullBladeCharacter.generated.h"

class UMyAttributeSetBase;


UCLASS(Blueprintable)
class ADullBladeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADullBladeCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWalking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WeaponAttachSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName LeftFootPrintAttachSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName RightFootPrintAttachSocketName;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire);

	UFUNCTION()
	void PlayMeleeStrike();

	UFUNCTION(BlueprintCallable)
	void BeginMeleeStrike();

	UFUNCTION(BlueprintCallable)
	void EndMeleeStrike();

	UFUNCTION(BlueprintCallable)
	void LeftFootDown();

	UFUNCTION(BlueprintCallable)
	void RightFootDown();

	UFUNCTION()
	void ResetLastMeleeAttackTime();

	UFUNCTION()
	void MouseZoomIn();

	UFUNCTION()
	void MouseZoomOut();

	UFUNCTION()
	void RotateCamera(float axisvalue);

	UFUNCTION()
	void FaceActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void CurrentWeaponPlaySwingSound();

	void SetMovementSpeed();

	
	virtual void Tick(float DeltaSeconds) override;

	
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	FORCEINLINE class UAnimMontage* GetAnimMontage() { return MeleeAnimMontage; }

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FootPrintSoundLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* FootPrintSoundHeavy;

	/** used to handle future ability system interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UAbilitySystemComponent* AbilitySystemComp;
	
	/** List of attributes modified by the ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	UMyAttributeSetBase* AttributeSetBaseComp;

	/** Abilities to grant to this character on creation. These will be activated by tag or event and are not bound to specific inputs */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY()
	class AMeleeWeaponBase*  CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ADecalActor> SnowFootPrint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AMeleeWeaponBase> StarterWeapon;

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere)
	float MeleeAttackTime;

	UPROPERTY(VisibleAnywhere)
	float LastMeleeAttackTime;

	void TraceFoot(FHitResult& OutHit, const FVector& Location) const;

	UFUNCTION(BlueprintCallable)
	void PlayFootStepSound(bool IsHeavy, bool IsRight);

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(VisibleAnywhere)
	bool bIsDead;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* MeleeAnimMontage;
	
};

