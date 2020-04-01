// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Deer.generated.h"


/**
 * 
 */
UCLASS()
class DULLBLADE_API ADeer : public AEnemyBase
{
	GENERATED_BODY()

		ADeer();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsGrazing = false;

	FTimerHandle GrazingCountDownTimerHandle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float GrazingTime;

	UFUNCTION()
	void UpdateMoveSpeed(float NewMoveSpeed);

	UFUNCTION()
	void ToggleGrazing();

	UFUNCTION()
	void EndGrazing();

	UFUNCTION(BlueprintCallable)
	void DeerShout();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase* DeerCall;

protected:

	virtual void BeginPlay() override;
};
