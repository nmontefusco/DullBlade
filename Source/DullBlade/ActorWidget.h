// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

/**
 * 
 */
UCLASS(Abstract)
class DULLBLADE_API UActorWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Actor Widget")
		AActor* OwningActor;


public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void SetOwningActor(AActor* NewOwner);

	UFUNCTION()
	   void SetItemTitleText();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ItemTitle;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UBorder* ItemTitleBorder;

	UPROPERTY(EditAnywhere)
		FText ItemName;

	UPROPERTY(BlueprintAssignable)
		FOnOwningActorChanged OnOwningActorChanged;

};
