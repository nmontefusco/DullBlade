// Fill out your copyright notice in the Description page of Project Settings.


#include "Deer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADeer::ADeer()
{

}

void ADeer::UpdateMoveSpeed(float NewMoveSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMoveSpeed;
}

void ADeer::ToggleGrazing()
{
		bIsGrazing = true;
		GrazingTime = FMath::RandRange(4.f, 10.f);
		GetWorldTimerManager().SetTimer(GrazingCountDownTimerHandle,this,&ADeer::EndGrazing, GrazingTime, false);
		UpdateMoveSpeed(0.0f);	
}

void ADeer::EndGrazing()
{
	bIsGrazing = false;
	GetWorldTimerManager().ClearTimer(GrazingCountDownTimerHandle);
	float WakeUpMoveSpeed = FMath::RandRange(135.f, 265.f);
	UpdateMoveSpeed(WakeUpMoveSpeed);
}

void ADeer::DeerShout()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeerCall, GetActorLocation());
}

void ADeer::BeginPlay()
{
	Super::BeginPlay();

}
