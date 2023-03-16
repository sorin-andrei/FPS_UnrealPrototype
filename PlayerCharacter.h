// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "ActionMovementComponent.h"
#include "CombatComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SHOOTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the player has landed from a jump
	virtual void Landed(const FHitResult& Hit) override;

	//Decrement hearts
	UFUNCTION(BlueprintCallable)
	void TakeDamage();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_TakeDamage();

	//UPROPERTY(BlueprintReadWrite)
	//FVector StartFirePosition;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Player Status
	UPROPERTY(BlueprintReadWrite)
	int32 Hearts = 3;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UActionMovementComponent* ActionMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComponent;

	//Combat
	void TryStartFirePrimary();
	void CallStopFirePrimary();

	void TryStartFireSecondary();
	void CallStopFireSecondary();

	//Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	void CallInitiateDive();
	void CallInitiateSlide();

	
 
private:	
    


};
