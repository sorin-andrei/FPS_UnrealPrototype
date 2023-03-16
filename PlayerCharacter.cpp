// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.h"

#include "Weapon.h"
#include "Pickup.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActionMovement = CreateDefaultSubobject<UActionMovementComponent>(TEXT("MaxPayneMovement"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnHit);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &APlayerCharacter::CallInitiateDive);
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &APlayerCharacter::CallInitiateSlide);

	// Mouse input
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	// Fire input
	PlayerInputComponent->BindAction("PrimaryFire", IE_Pressed, this, &APlayerCharacter::TryStartFirePrimary);
	PlayerInputComponent->BindAction("PrimaryFire", IE_Released, this, &APlayerCharacter::CallStopFirePrimary);

	PlayerInputComponent->BindAction("SecondaryFire", IE_Pressed, this, &APlayerCharacter::TryStartFireSecondary);
	PlayerInputComponent->BindAction("SecondaryFire", IE_Released, this, &APlayerCharacter::CallStopFireSecondary);
	 
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !ActionMovement->GetIsSliding())
	{
		// Add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}
void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	if (ActionMovement->GetIsDiving())
	{
		ActionMovement->InitiateSlide();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		ActionMovement->SetIsDiving(false);
	}
}

void APlayerCharacter::TryStartFirePrimary()
{
	CombatComponent->StartFiringPrimary();
}

void APlayerCharacter::CallStopFirePrimary()
{
	CombatComponent->StopFiringPrimary();
}

void APlayerCharacter::TryStartFireSecondary()
{
	CombatComponent->StartFiringSecondary();
}

void APlayerCharacter::CallStopFireSecondary()
{
	CombatComponent->StopFiringSecondary();
}

void APlayerCharacter::CallInitiateDive()
{
	ActionMovement->Dive();
}

void APlayerCharacter::CallInitiateSlide()
{
	ActionMovement->InitiateSlide();
}

void APlayerCharacter::TakeDamage()
{
	Hearts--;
	BP_TakeDamage();
}



void APlayerCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APickup* PickupActor = Cast<APickup>(OtherActor);
	if (!IsValid(PickupActor))
	{
		return;
	}
	if (PickupActor->WeaponType)
	{
		if (!CombatComponent->CurrentWeapon)
		{
			CombatComponent->EquipWeapon(PickupActor->WeaponType);
			OtherActor->Destroy();
		}
		else if (!CombatComponent->CurrentWeaponSecondary)
		{
			CombatComponent->EquipWeaponSecondary(PickupActor->WeaponType);
			OtherActor->Destroy();
		}
	}
}

void APlayerCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AEnemyCharacter* EnemyHit = Cast<AEnemyCharacter>(OtherActor);
	FVector EnemyLaunchForce;
	if (EnemyHit != nullptr)
	{
		if (ActionMovement->GetCanSlideDamage())
		{
			EnemyLaunchForce = EnemyHit->GetActorUpVector() * 60000.f;
			EnemyHit->LaunchCharacter(EnemyLaunchForce, false, false);
			EnemyHit->TakeDamage(200.f);
		}
	}
}
