// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "UseableActor.generated.h"

class UMyWidgetComponent;

UCLASS()
class DULLBLADE_API AUseableActor : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	

	UFUNCTION()
	virtual bool BeginFocus() const override;

	UFUNCTION()
	virtual bool EndFocus() const override;

	UFUNCTION()
		virtual void OnClicked(APlayerController* PlayerController);

	/*
	UFUNCTION()
	virtual float GetUseDistance()  const override;

	UFUNCTION()
	virtual bool OnUsed() override;

	UFUNCTION()
	virtual bool IsUsable() const override;
	*/
	AUseableActor();

	UPROPERTY()
	TSubclassOf<class UActorWidget> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMyWidgetComponent* DisplayWidget;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginCursorOver(UPrimitiveComponent* pComponent);

	UFUNCTION()
	void OnEndCursorOver(UPrimitiveComponent* pComponent);

	UPROPERTY(EditAnywhere)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWasUsed;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		FText GetActorName();
		
};
