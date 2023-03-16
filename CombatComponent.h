// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadOnly)
	AWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	AWeapon* CurrentWeaponSecondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* WeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* WeaponSocketSecondary;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* MuzzleLocation;

	UFUNCTION()
	void EquipWeapon(TSubclassOf<AWeapon> WeaponType);

	UFUNCTION()
	void EquipWeaponSecondary(TSubclassOf<AWeapon> WeaponType);

	UFUNCTION()
	void StartFiringWeapon(AWeapon* Weapon);

	UFUNCTION()
	void StopFiringWeapon(AWeapon* Weapon);

	void StartFiringPrimary() { StartFiringWeapon(CurrentWeapon); };
	void StopFiringPrimary() { StopFiringWeapon(CurrentWeapon); };

	void StartFiringSecondary() { StartFiringWeapon(CurrentWeaponSecondary); };
	void StopFiringSecondary() { StopFiringWeapon(CurrentWeaponSecondary); };



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	


		
};
