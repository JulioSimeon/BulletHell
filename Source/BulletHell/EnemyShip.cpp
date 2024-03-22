// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyShip.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
//#include "TimerManager.h"

AEnemyShip::AEnemyShip()
{
    
}

void AEnemyShip::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Move(DeltaTime);
}

void AEnemyShip::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

void AEnemyShip::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AEnemyShip::Fire, FireRate, true);
    
    XOriginalLocation = GetActorLocation().X;
}

void AEnemyShip::Fire()
{
    if(WiperFire)
    {
        ProjectileRotationOffset = FRotator(0, WiperRotationCounter * 10, 0);
        
        if(WiperRotationCounter <= 9 && !Flip)
        {
            ++WiperRotationCounter;
        }
        else if(WiperRotationCounter > -9 || Flip)
        {
            Flip = true;
            --WiperRotationCounter;
        }
        if(WiperRotationCounter == -9)
        {
            Flip = false;
        }
    }
    
    if(Boomer)
    {   
        for(int i = 1; i < 8; i++)
        {
            ProjectileRotationOffset = FRotator(0, WiperRotationCounter * 45 * i, 0);
            auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation() + ProjectileRotationOffset);
	        Projectile->SetOwner(this);
            
        }
    }

    auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation() + ProjectileRotationOffset);
	Projectile->SetOwner(this);
    
    Super::Fire();
}

bool AEnemyShip::HasDestination()
{
    return !Destination.Equals(FVector::ZeroVector);
}

void AEnemyShip::Move(float DeltaTime)
{
    if(Spinner)
    {
        AddActorLocalRotation(RotationVelocity * DeltaTime);
    }
    if(HasDestination())
    {
        FVector CurrentLocation = GetActorLocation();
        float Speed = FMath::Sqrt(FMath::Square(XVelocity) + FMath::Square(YVelocity));
        FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, Destination, DeltaTime, Speed);
	    SetActorLocation(NewLocation);
    }
    else
    {
        FVector LocationOffset = FVector::ZeroVector;
        if(XMoveDistance > 0 && ShouldReturn())
        {
            XVelocity = -XVelocity;
        }
        LocationOffset.X = DeltaTime * XVelocity;
        LocationOffset.Y = DeltaTime * YVelocity;
        //AddActorLocalOffset(LocationOffset);
        SetActorLocation(GetActorLocation() + LocationOffset);
    }
    
}

bool AEnemyShip::ShouldReturn()
{
    return UKismetMathLibrary::Abs(GetActorLocation().X - XOriginalLocation) > XMoveDistance;
}

void AEnemyShip::SetVelocity(float XVel, float YVel)
{
    XVelocity = XVel;
    YVelocity = YVel;
}

void AEnemyShip::SetDestination(FVector Dest)
{
    Destination = Dest;
}

void AEnemyShip::SetProjectileRotationOffset(FRotator Offset)
{
    ProjectileRotationOffset = Offset;
}

void AEnemyShip::SetXMoveDistance(float Distance)
{
    XMoveDistance = Distance;
}
