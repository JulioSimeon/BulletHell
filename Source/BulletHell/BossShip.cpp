// Fill out your copyright notice in the Description page of Project Settings.


#include "BossShip.h"
#include "HealthComponent.h"


void ABossShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(!HasDestination())
    {
        OutofBoundsCheck();
    }
    else //if(HasDestination())
    {
        if(FVector::Dist(GetActorLocation(), Destination) == 0)
        {
            Destination = FVector::ZeroVector;
        }
    }
    
    CheckSpawnCount();
}

void ABossShip::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABossShip::Fire, FireRate, true);
    GetWorldTimerManager().SetTimer(SpawnRateTimerHandle, this, &ABossShip::SpawnChildShip, SpawnRate, true);
}

void ABossShip::OutofBoundsCheck()
{
    if(GetActorLocation().X < XYLowerBoundary.X || GetActorLocation().X > XYUpperBoundary.X)
    {
        XVelocity = -XVelocity;
    }
    if(GetActorLocation().Y < XYLowerBoundary.Y || GetActorLocation().Y > XYUpperBoundary.Y)
    {
        YVelocity = -YVelocity;
    }
}

void ABossShip::CheckSpawnCount()
{
    if(ChildShips.Num() >= MaxNumberOfChildren)
    {
        GetWorldTimerManager().PauseTimer(SpawnRateTimerHandle);
    }
    else
    {
        GetWorldTimerManager().UnPauseTimer(SpawnRateTimerHandle);
    }
}

void ABossShip::SpawnChildShip()
{   

    FVector Origin;
    FVector BoxExtent;
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    GetActorBounds(true, Origin, BoxExtent);
    UE_LOG(LogTemp, Display, TEXT("Origin %s\n BoxExtent %s\n SpawnPointOffset %s"), *Origin.ToString(), *BoxExtent.ToString(), *(BoxExtent * FVector::YAxisVector).ToString());
    auto ChildShip = GetWorld()->SpawnActor<ABossShip>(BossShipClass, ProjectileSpawnPoint->GetComponentLocation() + (BoxExtent * FVector::YAxisVector), ProjectileSpawnPoint->GetComponentRotation(), SpawnParameters);
    //auto ChildShip = GetWorld()->SpawnActor<ABossShip>(BossShipClass, ProjectileSpawnPoint->GetComponentLocation() + (BoxExtent * FVector::YAxisVector), ProjectileSpawnPoint->GetComponentRotation());
    UE_LOG(LogTemp, Display, TEXT("%s Spawning Child"), *GetActorNameOrLabel());
    if(ChildShip)
    {
        ChildShips.Add(ChildShip);
        ChildShip->SetOwner(this);
        ChildShip->SetActorScale3D(GetActorScale3D()/2);
        
        ChildShip->SetVelocity(-XVelocity, YVelocity);
        float YBoundaryHalfRange = (XYUpperBoundary.Y - XYLowerBoundary.Y) / 2;
        ChildShip->SetXYBoundary(FVector2D(XYUpperBoundary.X, YBoundaryHalfRange + ChildShip->GetActorLocation().Y), FVector2D(XYLowerBoundary.X, ChildShip->GetActorLocation().Y - YBoundaryHalfRange));

        HealthComponent = Cast<UHealthComponent>(GetComponentByClass(HealthComponentClass));
        ChildHealthComponent = Cast<UHealthComponent>(ChildShip->GetComponentByClass(HealthComponentClass));
        if(HealthComponent && ChildHealthComponent)
        {
            ChildHealthComponent->setMaxHealth(HealthComponent->getMaxHealth()/2);
        }
    }
	
}

void ABossShip::HandleDestruction()
{
    Super::HandleDestruction();
    for(auto child : ChildShips)
    {
        if(child)
        {
            child->HandleDestruction();
        }
    }
}

void ABossShip::SetXYBoundary(FVector2D XYUpperBound, FVector2D XYLowerBound)
{
    XYUpperBoundary = XYUpperBound;
    XYLowerBoundary = XYLowerBound;
}

void ABossShip::UpdateChildShipStatus(ABossShip* DeadChild)
{
    for(auto child : ChildShips)
    {
        if(child == DeadChild)
        {
            ChildShips.Remove(child);
        }
    }
}
