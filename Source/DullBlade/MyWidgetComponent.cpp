// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWidgetComponent.h"
#include "GameFramework/Actor.h"
#include "ActorWidget.h"

void UMyWidgetComponent::InitWidget()
{
	Super::InitWidget();
	if (Widget)
	{
		
		UActorWidget* WidgetInst = Cast<UActorWidget>(Widget);	
		if (WidgetInst)
		{
			WidgetInst->SetOwningActor(GetOwner());	
			WidgetInst->SetItemTitleText();
		}

	}
}

UMyWidgetComponent::UMyWidgetComponent()
{
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
