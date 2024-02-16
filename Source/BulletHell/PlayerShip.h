// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "PlayerShip.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API APlayerShip : public ABasePawn
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	APlayerShip();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();
	
	APlayerController* GetShipPlayerController() const; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void MoveForward(float Value);

	void MoveSideward(float Value);

	void Fire();

	void AltFire(float Value);

	void UpdateMovement();

	void SetCamera();

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 200.f;

	FVector DeltaLocation;
	
	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 0.1;

	UPROPERTY(EditAnywhere, Category = "Components")
	TSubclassOf<class ACameraActor> CameraToFind;

	APlayerController* ShipPlayerController;
};
