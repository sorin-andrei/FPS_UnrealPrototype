// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
	// Handle Primary Fire
	
	if (CurrentWeapon && CurrentWeapon->GetIsFiring() && CurrentWeapon->GetCanFire())
	{
		CurrentWeapon->FireEvent(MuzzleLocation->GetComponentLocation());
	}

	// Handle Secondary Fire
	if (CurrentWeaponSecondary && CurrentWeaponSecondary->GetIsFiring() && CurrentWeaponSecondary->GetCanFire())
	{
		CurrentWeaponSecondary->FireEvent(MuzzleLocation->GetComponentLocation());
	}

}

void UCombatComponent::EquipWeapon(TSubclassOf<AWeapon> WeaponType)
{
	CurrentWeapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponType));
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	CurrentWeapon->AttachToComponent(WeaponSocket, AttachmentRules);
}

void UCombatComponent::EquipWeaponSecondary(TSubclassOf<AWeapon> WeaponType)
{
	CurrentWeaponSecondary = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponType));
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	CurrentWeaponSecondary->AttachToComponent(WeaponSocketSecondary, AttachmentRules);
}

void UCombatComponent::StartFiringWeapon(AWeapon* Weapon)
{
	if (!Weapon)
		return;
	if (Weapon->GetAmmo() == 0)
	{
		Weapon->Destroy();

		//Dirty AF way to get the weapon variables back to null.. it is what it is i'm not the best programmer :))
		// Weapon = nullptr won't work 
		if (Weapon == CurrentWeapon)
			CurrentWeapon = nullptr;
		else if (Weapon == CurrentWeaponSecondary)
			CurrentWeaponSecondary = nullptr;

		return;
	}
	Weapon->SetIsFiring(true);
	Weapon->OnFireStarted();
}

void UCombatComponent::StopFiringWeapon(AWeapon* Weapon)
{
	if (!Weapon)
		return;
	Weapon->SetIsFiring(false);
	Weapon->OnFireStopped();
}

