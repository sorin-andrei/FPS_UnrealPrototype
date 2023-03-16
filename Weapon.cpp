// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	Ammo = AmmoCapacity;
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsFiring)
	{
		Recoil = FMath::FInterpTo(Recoil, 0, DeltaTime, RecoilRecoverySpeed);
		
	}
}

void AWeapon::FireEvent(FVector StartLocation)
{
	if (Ammo == 0)
	{
		return;
	}

	FVector RecoilOffset = RecoilCurve->GetVectorValue(Recoil);
	FVector EndLocation = StartLocation + GetActorForwardVector() * Range;

	EndLocation += RecoilOffset;
	//FVector RandomSpreadY, RandomSpreadZ;
	

	for (int i = 0; i < NumberOfShots; i++)
	{
		if (Spread > 0 && i>0) //i>0 condition guarantees at least one of the shotgun shots hits where the player aims
		{
			EndLocation +=  FMath::RandRange(-Spread, Spread) * GetActorRightVector();
			EndLocation += FMath::RandRange(-Spread, Spread) * GetActorUpVector();
			//EndLocation += FVector(0, RandomSpreadY, RandomSpreadZ);
		}
		FireLineTrace(StartLocation, EndLocation);
	}
	if (!bIsAutomatic)
	bIsFiring = false;

	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireRateManager, this, &AWeapon::EndFireCooldown, FireRate, true);
	
	Recoil++;
	Recoil = int(Recoil);

	Ammo--;

	//Call blueprint event
	OnShotFired();

}

void AWeapon::FireLineTrace(FVector StartLocation, FVector EndLocation)
{
	/* Linetrace params */
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult Hit;

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		DrawDebugSphere(GetWorld(), Hit.Location, 5.f, 2, FColor(255, 0, 255), false, 5.f, 100, 5.f);
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor());
		if (Enemy)
		{
			Enemy->TakeDamage(Damage);
		}
	}
}

