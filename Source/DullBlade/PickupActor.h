// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UseableActor.h"
#include "PickupActor.generated.h"

/**
 * 
 */
UCLASS()
class DULLBLADE_API APickupActor : public AUseableActor
{
	GENERATED_BODY()
	
public:

		APickupActor();

		virtual void OnClicked(APlayerController* PlayerCharacter) override;

		

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* GoldSound;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UBoxComponent* OverlapBox;

	void Killmyself();
};
