// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Projectile.h"


APlayerShip::APlayerShip()
{
    
}

void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    UpdateMovement();
}

void APlayerShip::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
}

APlayerController* APlayerShip::GetShipPlayerController() const
{
    return ShipPlayerController;
}

void APlayerShip::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerShip::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveSideward"), this, &APlayerShip::MoveSideward);

    PlayerInputComponent->BindAxis(TEXT("FireAlt"), this, &APlayerShip::AltFire);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerShip::Fire);

}

void APlayerShip::BeginPlay()
{
    Super::BeginPlay();
    ShipPlayerController = Cast<APlayerController>(GetController());
    SetCamera();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &APlayerShip::Fire, FireRate, true);
    GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
}

void APlayerShip::MoveForward(float Value)
{
    DeltaLocation.Zero();
    DeltaLocation.X = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void APlayerShip::MoveSideward(float Value)
{
    DeltaLocation.Zero();
    DeltaLocation.Y = Value * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
    AddActorLocalOffset(DeltaLocation, true);
}

void APlayerShip::Fire()
{
    for(int i = -1; i < 2; i++)
	{
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
		Rotation.Yaw += (i * 30);
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), Rotation);
		Projectile->SetOwner(this);
		//UE_LOG(LogTemp, Display, TEXT("%s"), *Rotation.ToString());
	}
}

void APlayerShip::AltFire(float Value)
{
    if(Value)
    {
        GetWorldTimerManager().UnPauseTimer(FireRateTimerHandle);
    }
    else
    {
        GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
    }
}

void APlayerShip::UpdateMovement()
{
    //Control movement via cursor
    if(ShipPlayerController)
    {
        FHitResult HitResult;
        ShipPlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false);
        SetActorLocation(HitResult.ImpactPoint - FVector(0, 0 , 580.f), true);
    }
}

void APlayerShip::SetCamera()
{
    if(ShipPlayerController)
    {
        //Sets Camera
        TArray<AActor*> Cameras;
        UGameplayStatics::GetAllActorsOfClass(this, CameraToFind, Cameras);
        ShipPlayerController->SetViewTarget(Cameras[0]);
    }
}