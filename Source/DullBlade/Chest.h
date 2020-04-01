
#pragma once

#include "CoreMinimal.h"
#include "UseableActor.h"
#include "Chest.generated.h"




UCLASS()
class DULLBLADE_API AChest : public AUseableActor
{
	GENERATED_BODY()

	
public:
	
	AChest();

	virtual bool BeginFocus() const override;
	virtual bool EndFocus() const override;

	virtual void OnClicked(APlayerController* PlayerCharacter) override;

	UPROPERTY(EditAnywhere)
		UCurveFloat* MyCurveFloat;

protected:

	void OnBeginCursorOver(UPrimitiveComponent* pComponent);
	void OnEndCursorOver(UPrimitiveComponent* pComponent);

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Lid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* OverlapBox;

	UPROPERTY()
		class UTimelineComponent* MyTimeLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* ChestSound;

	UFUNCTION(BlueprintCallable)
		void HandleAnimationProgress(float value);

	UFUNCTION(BlueprintCallable)
		void HandleTimelineFinished();

	UFUNCTION()
		void OnChestOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PlayAnimationTimeline();

	

};
