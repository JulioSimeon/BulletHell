// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BulletHellGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

\
// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);

	BulletHellGameMode = Cast<ABulletHellGameMode>(UGameplayStatics::GetGameMode(this));
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if(Damage <= 0.f)
	{
		return;
	}
	if(Instigator != GetOwner()->GetInstigatorController())
	{
		Health -= Damage;
		UE_LOG(LogTemp, Display, TEXT("%s Got Damaged \t Remaining Health: %f"), *DamagedActor->GetActorNameOrLabel(), Health);
	}
	if(Health <= 0.f && BulletHellGameMode)
	{
		BulletHellGameMode->ActorDied(DamagedActor);
	}
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::setMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
}

float UHealthComponent::getMaxHealth()
{
	return MaxHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

