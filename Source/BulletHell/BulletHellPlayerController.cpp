// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletHellPlayerController.h"


void ABulletHellPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if(bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
        GetPawn()->SetActorTickEnabled(true);
    }
    else
    {
        GetPawn()->DisableInput(this);
        GetPawn()->SetActorTickEnabled(false);
    }
}
