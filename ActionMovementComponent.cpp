// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UActionMovementComponent::UActionMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UActionMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<ACharacter>(GetOwner());
}

// Called every frame
void UActionMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Max Payne dive + Bullet time
void UActionMovementComponent::Dive()
{
	UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();

	//We can only dive from the ground
	if (Movement->IsFalling() || Movement->GetCurrentAcceleration() == FVector(0, 0, 0))
		return;

	//Update diving state
	bIsDiving = true;

	//Compute the dive impulse force
	DiveAcceleration = Movement->GetCurrentAcceleration();
	FVector DiveImpulse = DiveAcceleration * 0.5f + PlayerCharacter->GetActorUpVector() * 520.f;

	//Throw the character
	PlayerCharacter->LaunchCharacter(DiveImpulse, false, false);

	//Set crouching state so hitbox is smaller (we can jump through tighter spaces and it's cool)
	PlayerCharacter->Crouch();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
}

//Slide
void UActionMovementComponent::InitiateSlide()
{
	UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();
	if (bIsSliding || (Movement->IsFalling() && !bIsDiving))
		return;

	float MoveForwardInput = PlayerCharacter->GetInputAxisValue("MoveForward");
	if (!bIsDiving && MoveForwardInput < 1)
		return;

	bIsSliding = true;
	BackupGroundFriction = Movement->GroundFriction; // Backing up the ground friction value, since we are gonna set it to 0

	PlayerCharacter->Crouch();
	Movement->GroundFriction = 0.f;

	if (!bIsDiving)
	{
		FVector LaunchVelocity = FVector::CrossProduct(ComputeGroundNormal(), PlayerCharacter->GetActorRightVector()); //Calculate sliding force
		LaunchVelocity *= SlideForce;
		LaunchVelocity *= -1.f;
		PlayerCharacter->LaunchCharacter(LaunchVelocity, true, true);
		bCanSlideDamage = true;
	}


	GetWorld()->GetTimerManager().SetTimer(SlideManager, this, &UActionMovementComponent::CancelSlide, SlideDuration, true);

}

void UActionMovementComponent::CancelSlide()
{
	UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();
	PlayerCharacter->UnCrouch();
	Movement->GroundFriction = BackupGroundFriction;
	bIsSliding = false;
	bCanSlideDamage = false;
}

FVector UActionMovementComponent::ComputeGroundNormal()
{
	/* Linetrace params */
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult Hit;

	FVector StartLocation = PlayerCharacter->GetActorLocation();
	FVector EndLocation = StartLocation * (PlayerCharacter->GetActorUpVector() * -1.f) * 100.f;

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		return Hit.ImpactNormal;
	}
	else
	{
		return FVector(0, 0, 0);
	}
}