// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "EnemyShip.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API AEnemyShip : public ABasePawn
{
	GENERATED_BODY()

public:
	AEnemyShip();

	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction();

	UFUNCTION(BlueprintCallable)
	void SetVelocity(float XVel, float YVel);

	UFUNCTION(BlueprintCallable)
	void SetDestination(FVector Dest);

	UFUNCTION(BlueprintCallable)
	void SetProjectileRotationOffset(FRotator Offset);

protected:
	virtual void BeginPlay() override;

	void Fire();

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float XVelocity;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float YVelocity;

	FVector Destination;

	bool HasDestination();

private:
	
	//Spinner Variables	
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool Spinner = false;

	UPROPERTY(EditAnywhere, Category="Combat")
	FRotator RotationVelocity;

	//Boomer Variables	
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool Boomer = false;

	//Wiper Variables
	UPROPERTY(EditAnywhere, Category = "Combat")
	bool WiperFire = false;

	int WiperRotationCounter = -9;

	bool Flip = false;

	FRotator ProjectileRotationOffset;

};
