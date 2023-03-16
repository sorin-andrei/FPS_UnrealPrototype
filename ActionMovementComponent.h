// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UActionMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sets default values for this component's properties
	UActionMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SlideDuration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float SlideForce = 1000.f;
    
	UFUNCTION()
	FVector ComputeGroundNormal();

	void Dive();
	void InitiateSlide();
	void CancelSlide();

	bool GetIsDiving() { return bIsDiving; };
	bool GetIsSliding() { return bIsDiving; };
	bool GetCanSlideDamage() { return bCanSlideDamage; };

	void SetIsDiving(bool Value) { bIsDiving = Value; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float BackupGroundFriction;
	FVector DiveAcceleration;
	ACharacter* PlayerCharacter;

	bool bIsDiving;
	bool bIsSliding;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bCanSlideDamage;
	

private:
	FTimerHandle SlideManager;
		
};
