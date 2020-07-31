// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio

#include "HealthComponent.h"
#include "IProcessingDamage.h"
#include "GameFramework/Pawn.h"
#include "ControllerEventsComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	MaxHealth = 100.f;
	MaxArmour = 100.f;
	bIsUseArmour = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		CurrentHealth = MaxHealth;
		CurrentArmour = MaxArmour;
	}
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, CurrentArmour);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetOwner()->GetWorldTimerManager().IsTimerActive(HealthRegenerationHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(HealthRegenerationHandle);
	}

	if (GetOwner()->GetWorldTimerManager().IsTimerActive(ArmourRegenerationHandle))
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(ArmourRegenerationHandle);
	}
}

void UHealthComponent::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetOwner()->Implements<UProcessingDamage>())
	{
		if (!IProcessingDamage::Execute_CanApplyDamage(GetOwner(), DamageCauser))
		{
			return;
		}

		IProcessingDamage::Execute_ModifireDamage(GetOwner(), DamageCauser, Damage);
	}

	int32 DamageDealt;

	if (bIsUseArmour)
	{
		const int32 ArmourRemoved = FMath::Min(Damage, CurrentArmour);
		CurrentArmour -= ArmourRemoved;
		DamageDealt = FMath::Min(Damage - ArmourRemoved, CurrentHealth);
	}
	else
	{
		DamageDealt = FMath::Min(Damage, CurrentHealth);
	}

	const bool bWasDead = CurrentHealth <= 0.f;
	CurrentHealth -= DamageDealt;
	const bool bIsDead = CurrentHealth <= 0.f;

	FVector Source;
	FVector Impact;

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
		Source = DamageCauser ? DamageCauser->GetActorLocation() : GetOwner()->GetActorLocation() - PointDamageEvent->ShotDirection;
		Impact = PointDamageEvent->HitInfo.ImpactPoint;
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* const RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
		Impact = GetOwner()->GetActorLocation() + RadialDamageImpactOffset;
		Source = RadialDamageEvent->Origin;
	}
	else
	{
		Source = DamageCauser ? DamageCauser->GetActorLocation() : GetOwner()->GetActorLocation();
		Impact = GetOwner()->GetActorLocation();
	}

	MulticastDamageTaken(Damage, Source, Impact, DamageCauser);

	if (!bWasDead && bIsDead)
	{
		AuthoritativeDeath.Broadcast(EventInstigator);

		if (APawn* OwnerAsPawn = Cast<APawn>(GetOwner()))
		{
			if (AController* Controller = OwnerAsPawn->GetController())
			{
				if (UControllerEventsComponent* ControllerEvents = Cast<UControllerEventsComponent>(Controller->GetComponentByClass(UControllerEventsComponent::StaticClass())))
				{
					ControllerEvents->Death(EventInstigator);
				}

				if (EventInstigator != nullptr)
				{
					if (UControllerEventsComponent* ControllerEvents = Cast<UControllerEventsComponent>(EventInstigator->GetComponentByClass(UControllerEventsComponent::StaticClass())))
					{
						ControllerEvents->Kill(Controller);
					}
				}
			}
		}
	}


	if(!bIsDead)
	{
		if (GetOwner()->GetWorldTimerManager().IsTimerActive(HealthRegenerationHandle))
		{
			GetOwner()->GetWorldTimerManager().ClearTimer(HealthRegenerationHandle);
		}
		if (GetOwner()->GetWorldTimerManager().IsTimerActive(ArmourRegenerationHandle))
		{
			GetOwner()->GetWorldTimerManager().ClearTimer(ArmourRegenerationHandle);
		}
		if (HealthRegenInterval > 0)
		{
			GetOwner()->GetWorldTimerManager().SetTimer(HealthRegenerationHandle, this, &UHealthComponent::RegenerateHealth, HealthRegenInterval, true, HealthRegenCooldown);
		}
		if (bIsUseArmour && ArmourRegenInterval > 0)
		{
			GetOwner()->GetWorldTimerManager().SetTimer(ArmourRegenerationHandle, this, &UHealthComponent::RegenerateArmour, ArmourRegenInterval, true, ArmourRegenCooldown);
		}
	}
}

void UHealthComponent::AddHealth_Implementation(const float Value)
{
	GrantHealth(Value);
}

bool UHealthComponent::AddHealth_Validate(const float Value)
{
	return true;
}

void UHealthComponent::AddArmor_Implementation(const float Value)
{
	GrantShield(Value);
}

bool UHealthComponent::AddArmor_Validate(const float Value)
{
	return true;
}

bool UHealthComponent::GrantHealth(const float Value)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth = FMath::Min(CurrentHealth + Value, MaxHealth);

		return true;
	}

	return false;
}

bool UHealthComponent::GrantShield(const float Value)
{
	if (CurrentArmour < MaxArmour)
	{
		CurrentArmour = FMath::Min(CurrentArmour + Value, MaxArmour);

		return true;
	}

	return false;
}

void UHealthComponent::RegenerateHealth()
{
	if (CurrentHealth > 0.f)
	{
		GrantHealth(HealthRegenValue);
	}
}

void UHealthComponent::RegenerateArmour()
{
	if (CurrentArmour > 0.f)
	{
		GrantHealth(ArmourRegenValue);
	}
}

void UHealthComponent::OnRep_CurrentArmour()
{
	ArmourUpdated.Broadcast(CurrentArmour, MaxArmour);
}

void UHealthComponent::OnRep_CurrentHealth()
{
	HealthUpdated.Broadcast(CurrentHealth, MaxHealth);
	if (CurrentHealth <= 0.f)
	{
		Death.Broadcast();
	}
}

void UHealthComponent::MulticastDamageTaken_Implementation(float Value, FVector Source, FVector Impact, AActor* DamageCauser)
{
	DamageTaken.Broadcast(Value, Source, Impact, DamageCauser);
}

void UHealthComponent::OnAnyDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	TakeDamage(Damage, FDamageEvent(), InstigatedBy, DamageCauser);
}

void UHealthComponent::OnPointDamage(AActor * DamagedActor, float Damage, AController * InstigatedBy, FVector HitLocation, UPrimitiveComponent * FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType * DamageType, AActor * DamageCauser)
{
	TakeDamage(Damage, FPointDamageEvent(), InstigatedBy, DamageCauser);
}

void UHealthComponent::OnRadialDamage(AActor * DamagedActor, float Damage, const UDamageType * DamageType, FVector Origin, FHitResult HitInfo, AController * InstigatedBy, AActor * DamageCauser)
{
	TakeDamage(Damage, FRadialDamageEvent(), InstigatedBy, DamageCauser);
}