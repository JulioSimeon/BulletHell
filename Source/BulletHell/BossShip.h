// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyShip.h"
#include "BossShip.generated.h"

/**
 * 
 */
UCLASS()
class BULLETHELL_API ABossShip : public AEnemyShip
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction() override;

	void SetXYBoundary(FVector2D XYUpperBound, FVector2D XYLowerBound);

	void UpdateChildShipStatus(ABossShip* DeadChild);

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle FireRateTimerHandle;

	FTimerHandle SpawnRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpawnRate = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class ABossShip> BossShipClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class UActorComponent> HealthComponentClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	int MaxNumberOfChildren = 2;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector2D XYUpperBoundary;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector2D XYLowerBoundary;

	class UHealthComponent* HealthComponent;

	class UHealthComponent* ChildHealthComponent;


	void OutofBoundsCheck();

	void CheckSpawnCount();

	void SpawnChildShip();

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TArray<ABossShip*> ChildShips;
};
