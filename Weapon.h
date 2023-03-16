// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Curves/CurveVector.h"


#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector StartFireLocation;
    
	void FireEvent(FVector StartLocation);

	bool GetIsFiring() { return bIsFiring; };
	void SetIsFiring(bool Value) { bIsFiring = Value; };

	bool GetCanFire() { return bCanFire; };

	int32 GetAmmo() { return Ammo; };

	UFUNCTION(BlueprintImplementableEvent)
	void OnShotFired();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFireStarted();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFireStopped();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Parameters")
	UCurveVector* RecoilCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Parameters")
    bool bIsAutomatic = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	int32 NumberOfShots = 1;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float Spread;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float FireRate = 0.05f;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float Range = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	float RecoilRecoverySpeed = 2.5f;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	int32 AmmoCapacity = 30;

	UPROPERTY(EditAnywhere, Category = "Weapon Parameters")
	int32 Damage = 20;

	


	UFUNCTION()
	void FireLineTrace(FVector StartLocation, FVector EndLocation);

private:	

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Recoil = 0;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring = false;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanFire = true;

	void EndFireCooldown() { bCanFire = true; };

	float FireOffsetX;
	float FireOffsetY;
	
	//Once firing, after <FireRate> seconds you can fire again
	FTimerHandle FireRateManager;


};
