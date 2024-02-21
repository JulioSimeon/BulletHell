// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "BulletHellGameMode.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct FEnemySpawns : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Sequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DelayBeforeNextSequence;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int EnemyCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> EnemyType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVector> SpawnLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVector2D> XYVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EnemySpawnDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRotator> ProjectileRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRotator> SpawnRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int SpawnCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnCountDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FVector> Destination;
};

UCLASS()
class BULLETHELL_API ABulletHellGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);
	
private:
	class APlayerShip* PlayerShip;

	class ABulletHellPlayerController* BulletHellPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemies();

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	UDataTable* EnemySpawnsDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TArray<TSubclassOf<class AEnemyShip>> EnemyShipClasses;

	void SpawnEnemyShip(FEnemySpawns* EnemySpawn);

	int GetAliveEnemyShipsCount();

	int SpawnSequenceCounter = 0;

	float Delay = 0;
};
