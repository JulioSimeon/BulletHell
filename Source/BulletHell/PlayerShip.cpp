// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Projectile.h"


APlayerShip::APlayerShip()
{
    
}

void APlayerShip::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerShip::Fire);
        EnhancedInputComponent->BindAction(AltFireAction, ETriggerEvent::Triggered, this, &APlayerShip::StartFire);
        EnhancedInputComponent->BindAction(AltFireAction, ETriggerEvent::Completed, this, &APlayerShip::EndFire);
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerShip::PauseGame);
        EnhancedInputComponent->BindAction(SwitchFireAction, ETriggerEvent::Started, this, &APlayerShip::SwitchFireMode);
    }
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

void APlayerShip::BeginPlay()
{
    Super::BeginPlay();
    ShipPlayerController = Cast<APlayerController>(GetController());
    if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(ShipPlayerController->GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
    SetCamera();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &APlayerShip::Fire, FireRate, true);
    GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
}

void APlayerShip::Fire()
{
    if(bSwitchFireMode)
    {
        for(int i = -1; i < 2; i++)
        {
            FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
            Rotation.Yaw += (i * 30);
            auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), Rotation);
            Projectile->SetOwner(this);
        }
    }
    else
    {
        for(int i = -1; i <= 1; i += 2)
        {
            auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation() + FVector(i * 50, 0, 0), ProjectileSpawnPoint->GetComponentRotation());
            Projectile->SetOwner(this);
        }
    }
    Super::Fire();
    
}

void APlayerShip::SwitchFireMode()
{
    bSwitchFireMode = !bSwitchFireMode;
    UE_LOG(LogTemp, Warning, TEXT("I was CALLED"));
}

void APlayerShip::StartFire()
{
    GetWorldTimerManager().UnPauseTimer(FireRateTimerHandle);
    // if(value.Get<float>() > 0)
    // {
    //     GetWorldTimerManager().UnPauseTimer(FireRateTimerHandle);
    // }
    // else
    // {
    //     GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
    // }
}

void APlayerShip::EndFire()
{
    GetWorldTimerManager().PauseTimer(FireRateTimerHandle);
}

void APlayerShip::UpdateMovement()
{
    //Control movement via cursor
    if(ShipPlayerController)
    {
        FHitResult HitResult;
        ShipPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false);
        FVector NewLocation = HitResult.ImpactPoint;
        NewLocation.Z = 124.f;
        if(NewLocation.X < -270)
        {
            NewLocation.X = -270;   
        }
        else if(NewLocation.X > 3200)
        {
            NewLocation.X = 3200;   
        }
        SetActorLocation(NewLocation, true);
        //SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), HitResult.ImpactPoint - FVector(0, 0 , 580.f), DeltaTime, InterpSpeed), true);
    }
}

void APlayerShip::SetCamera()
{
    if(ShipPlayerController)
    {
        //Sets Camera
        TArray<AActor*> Cameras;
        UGameplayStatics::GetAllActorsOfClass(this, CameraToFind, Cameras);
        if(Cameras.Num() > 0 && Cameras[0])
        {
            ShipPlayerController->SetViewTarget(Cameras[0]);
        }
        
    }
}