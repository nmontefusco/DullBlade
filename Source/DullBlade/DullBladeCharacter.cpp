// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "DullBladeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "MyAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DullBladePlayerController.h"
#include "MeleeWeaponBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


ADullBladeCharacter::ADullBladeCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));

	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create ability system component
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");

	// Create the attribute set
	AttributeSetBaseComp = CreateDefaultSubobject<UMyAttributeSetBase>("AttributeBaseComp");

	WeaponAttachSocketName = "WeaponSocket";
	RightFootPrintAttachSocketName = "Foot_RSocket";
	LeftFootPrintAttachSocketName = "Foot_LSocket";
}

void ADullBladeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

		if (ADullBladePlayerController* PC = Cast<ADullBladePlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
}

void ADullBladeCharacter::MouseZoomIn()
{
	FRotator MyCameraRotator = CameraBoom->GetTargetRotation();
	
	if (MyCameraRotator.Pitch >= -80.0f && MyCameraRotator.Pitch <= -45.0f)
	{
		CameraBoom->AddLocalRotation(FRotator(5.0f, 0, 0));
	}
	CameraBoom->TargetArmLength -= 100.f;
	CameraBoom->TargetArmLength =FMath::Clamp(CameraBoom->TargetArmLength, 300.f, 1500.f);
}

void ADullBladeCharacter::MouseZoomOut()
{
	FRotator MyCameraRotator = CameraBoom->GetTargetRotation();
	
	if (MyCameraRotator.Pitch >= -80.0f && MyCameraRotator.Pitch <= -45.0f)
	{
		CameraBoom->AddLocalRotation(FRotator(-5.0f, 0, 0));
	}

	CameraBoom->TargetArmLength += 100.f;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, 300.f, 1500.f);
}

void ADullBladeCharacter::RotateCamera(float AxisValue)
{
	CameraBoom->AddWorldRotation(FRotator(0,(AxisValue * 4.0f),0));
}

void ADullBladeCharacter::FaceActor(AActor* TargetActor)
{
	FVector TargetActorLocation = TargetActor->GetActorLocation();
	FVector PlayerCharacterLocation = GetActorLocation();

	FRotator FaceLocationRaw = UKismetMathLibrary::FindLookAtRotation(PlayerCharacterLocation, TargetActorLocation);
	FRotator FaceLocation = FRotator(0, FaceLocationRaw.Yaw, 0);
	
	SetActorRotation(FaceLocation);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetTargetActor(TargetActor);
	}
	
}

void ADullBladeCharacter::CurrentWeaponPlaySwingSound()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->PlayWeaponSwingSound();
	}
}

void ADullBladeCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<AMeleeWeaponBase>(StarterWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
}

void ADullBladeCharacter::TraceFoot(FHitResult& OutHit, const FVector& Location) const
{
	FVector Start = Location;
	FVector End = Location;

	Start.Z += 20.0f;
	End.Z -= 20.0f;

	//Re-initialize hit info
	OutHit = FHitResult(ForceInit);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility);
}

void ADullBladeCharacter::PlayFootStepSound(bool IsHeavy,bool IsRight)
{
	MakeNoise(1.f, this, GetActorLocation(), 700.f);
	
	if (IsHeavy)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootPrintSoundHeavy, GetActorLocation());
		return;
	}else
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootPrintSoundLight, GetActorLocation());
	}

	if (IsRight)
	{
		RightFootDown();
	}else
	{
		LeftFootDown();
	}
}

UAbilitySystemComponent* ADullBladeCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ADullBladeCharacter::AcquireAbility(TSubclassOf<UGameplayAbility>AbilityToAcquire)
{
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToAcquire)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire,1,0));
		}
		//Initializes our Abilities
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void ADullBladeCharacter::PlayMeleeStrike()
{
	UWorld* World = GetWorld();

	if (LastMeleeAttackTime> World->GetTimeSeconds()- MeleeAttackTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit that cooldown, exiting"));
		return;
	}
	MeleeAttackTime = (MeleeAnimMontage->CalculateSequenceLength()) - .25f;
	LastMeleeAttackTime = World->GetTimeSeconds();
	PlayAnimMontage(MeleeAnimMontage);
}

void ADullBladeCharacter::BeginMeleeStrike()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorEnableCollision(true);
	}
	
}

void ADullBladeCharacter::EndMeleeStrike()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorEnableCollision(false);
	}
	
}

void ADullBladeCharacter::LeftFootDown() 
{
	FHitResult LeftHitResult;
	
	const FVector LeftFootWorldPosition = GetMesh()->GetSocketLocation(LeftFootPrintAttachSocketName);

	TraceFoot(LeftHitResult, LeftFootWorldPosition);
	FVector FootFowardVector = GetActorForwardVector();
	FVector FootUpVector = GetActorUpVector();
	FQuat LeftfloorRot = FRotationMatrix::MakeFromZX(LeftHitResult.Normal, FootFowardVector).ToQuat();
	FQuat offsetRot(FRotator(0.0f, -90.0f, 0.0f));
	FRotator LeftFootRotation = (LeftfloorRot * offsetRot).Rotator();
	
	if (SnowFootPrint)
	{
		AActor* DecalInstanceLeft = GetWorld()->SpawnActor(SnowFootPrint, &LeftHitResult.Location, &LeftFootRotation);
	}
}

void ADullBladeCharacter::RightFootDown()
{

	FHitResult RightHitResult;
	const FVector RightFootWorldPosition = GetMesh()->GetSocketLocation(RightFootPrintAttachSocketName);
	TraceFoot(RightHitResult, RightFootWorldPosition);

	FVector FootFowardVector = GetActorForwardVector();
	FVector FootUpVector = GetActorUpVector();

	FQuat offsetRot(FRotator(0.0f, -90.0f, 0.0f));
	FQuat RightfloorRot = FRotationMatrix::MakeFromZX(RightHitResult.Normal, FootFowardVector).ToQuat();
	FRotator RightFootRotation = (RightfloorRot * offsetRot).Rotator();

	if (SnowFootPrint)
	{
		AActor* DecalInstanceRight = GetWorld()->SpawnActor(SnowFootPrint, &RightHitResult.Location, &RightFootRotation);
	}
}

void ADullBladeCharacter::ResetLastMeleeAttackTime()
{
	LastMeleeAttackTime = 0;
}

void ADullBladeCharacter::SetMovementSpeed()
{
	if (!bIsWalking)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		bIsWalking = true;
	}
	else if (bIsWalking)
	{
		bIsWalking = false;
		this->GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}	
}
