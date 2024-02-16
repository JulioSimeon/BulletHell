// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHellGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "BossShip.h"
#include "BulletHellPlayerController.h"

void ABulletHellGameMode::ActorDied(AActor* DeadActor)
{

    if(DeadActor == PlayerShip)
    {
        PlayerShip->HandleDestruction();
        if(BulletHellPlayerController)
        {
            BulletHellPlayerController->SetPlayerEnabledState(false);
            //PlayerShip->GetShipPlayerController()->bShowMouseCursor= false;
        }
    }
    else if(AEnemyShip* EnemyShip = Cast<AEnemyShip>(DeadActor))
    {
        if(ABossShip* BossShip = Cast<ABossShip>(EnemyShip))//if EnemyShip if a BossShip
        {
            if(ABossShip* Parent = Cast<ABossShip>(BossShip->GetOwner()))//if BossShip has a parent and the parent is still alive
            {
                Parent->UpdateChildShipStatus(BossShip);
                //Let parent know that its child has died so that it can spawn new child
            }
        }
        EnemyShip->HandleDestruction();
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
