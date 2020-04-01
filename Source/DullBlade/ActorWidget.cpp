// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorWidget.h"
#include "Border.h"
#include "TextBlock.h"
#include "UseableActor.h"

void UActorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UActorWidget::SetOwningActor(AActor* NewOwner)
{
	if (OwningActor== NewOwner)
	{
		return;
	}
	OwningActor = NewOwner;
	OnOwningActorChanged.Broadcast(NewOwner);
	
}

void UActorWidget::SetItemTitleText()
{
	AUseableActor* MyOwningActor = Cast<AUseableActor>(OwningActor);
	ItemName = MyOwningActor->GetActorName();
	ItemTitle->SetText(ItemName);
}


