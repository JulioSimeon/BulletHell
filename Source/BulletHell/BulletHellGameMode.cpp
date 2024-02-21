// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHellGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "BossShip.h"
#include "BulletHellPlayerController.h"

void ABulletHellGameMode::ActorDied(AActor* DeadActor)
{
    bool bBossShipIsAlive = true;
    if(DeadActor == PlayerShip)
    {
        PlayerShip->HandleDestruction();
        if(BulletHellPlayerController)
        {
            BulletHellPlayerController->SetPlayerEnabledState(false);
            //PlayerShip->GetShipPlayerController()->bShowMouseCursor= false;
            GameOver(false);
        }
    }
    else if(AEnemyShip* EnemyShip = Cast<AEnemyShip>(DeadActor))
    {
        if(ABossShip* BossShip = Cast<ABossShip>(EnemyShip))//if EnemyShip is a BossShip
        {
            if(ABossShip* Parent = Cast<ABossShip>(BossShip->GetOwner()))//if BossShip has a parent and the parent is still alive
            {
                Parent->UpdateChildShipStatus(BossShip);//Let parent know that its child has died so that it can spawn new child
            }
            else//if main BossShip has died
            {
                bBossShipIsAlive = false;
            }
        }
        EnemyShip->HandleDestruction();

        if(GetAliveEnemyShipsCount() == 0 && !bBossShipIsAlive)//if there are no more EnemyShips and main BossShip is dead remaining player has won
        {
            GameOver(true);
        }
    }
}

void ABulletHellGameMode::BeginPlay()
{
    Super::BeginPlay();
    HandleGameStart();
}

void ABulletHellGameMode::HandleGameStart()
{
    PlayerShip = Cast<APlayerShip>(UGameplayStatics::GetPlayerPawn(this, 0));
    BulletHellPlayerController = Cast<ABulletHellPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if(BulletHellPlayerController)
    {
        BulletHellPlayerController->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(BulletHellPlayerController, &ABulletHellPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
    }

}

void ABulletHellGameMode::SpawnEnemies()
{
    if(EnemySpawnsDataTable)
    {
        //if row is valid/found
        if(FEnemySpawns* EnemySpawn = EnemySpawnsDataTable->FindRow<FEnemySpawns>(FName(FString::FromInt(SpawnSequenceCounter)), "Enemy Spawn Context"))
        {
            UE_LOG(LogTemp, Display, TEXT("Row: %d"), SpawnSequenceCounter);
            //while actual spawn < spawn count
            for(int index = 0; index <= EnemySpawn->SpawnCount; index++)
            {
                //keep spawning
                    //Delay before next spawn count (SpawnCountDelay)
                    FTimerHandle SpawnDelayTimerHandle;
                    FTimerDelegate SpawnDelayTimerDelegate = FTimerDelegate::CreateUObject(this, &ABulletHellGameMode::SpawnEnemyShip, EnemySpawn);
                    GetWorldTimerManager().SetTimer(SpawnDelayTimerHandle, SpawnDelayTimerDelegate, Delay += EnemySpawn->SpawnCountDelay, false);
                    //Spawn EnemyShip until EnemyCount reached
            }
            //exit loop when finished spawning for sequence
            //increment SpawnSequenceCounter
            SpawnSequenceCounter++;
            //Delay before next sequence (next iteration of while loop)
            FTimerHandle SequenceDelayTimerHandle;
            //UE_LOG(LogTemp, Display, TEXT("Delay: %f"), Delay + EnemySpawn->DelayBeforeNextSequence);
            GetWorldTimerManager().SetTimer(SequenceDelayTimerHandle, this, &ABulletHellGameMode::SpawnEnemies, (Delay += EnemySpawn->DelayBeforeNextSequence) / 2, false);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Finished Spawning"));
        }
        
    }
}

void ABulletHellGameMode::SpawnEnemyShip(FEnemySpawns* EnemySpawn) 
{
    for(int i = 0; i < EnemySpawn->EnemyCount; i++)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        auto SpawnedEnemyShip = GetWorld()->SpawnActor<AEnemyShip>(EnemyShipClasses[EnemySpawn->EnemyType[i]], EnemySpawn->SpawnLocation[i], EnemySpawn->SpawnRotation[i], SpawnParameters);
        if(SpawnedEnemyShip)
        {
            SpawnedEnemyShip->SetVelocity(EnemySpawn->XYVelocity[i].X, EnemySpawn->XYVelocity[i].Y);
            SpawnedEnemyShip->SetDestination(EnemySpawn->Destination[i]);
            SpawnedEnemyShip->SetProjectileRotationOffset(EnemySpawn->ProjectileRotation[i]);
            UE_LOG(LogTemp, Display, TEXT("Spawned: %s"), *SpawnedEnemyShip->GetActorNameOrLabel());
        }
        
    }
    
}

int ABulletHellGameMode::GetAliveEnemyShipsCount()
{
    TArray<AActor*> EnemyShips;
    UGameplayStatics::GetAllActorsOfClass(this, AEnemyShip::StaticClass(), EnemyShips);
    return EnemyShips.Num();
}
