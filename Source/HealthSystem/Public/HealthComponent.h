// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// License:         MIT License (http://www.opensource.org/licenses/mit-license.php)
// Source Code:     https://github.com/Netlex/HealthSystem
// Original Author: Netlex Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFloatValue, float, Current, float, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FDamageTakenEvent, float, Value, FVector, Source, FVector, Impact, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathCauserEvent, const AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS(Blueprintable)
class HEALTHSYSTEM_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Health System")
		virtual void TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health System|Armour")
		bool GrantShield(float Value);

	UFUNCTION(BlueprintCallable, Category = "Health System|Health")
		bool GrantHealth(float Value);

	UFUNCTION(BlueprintPure, Category = "Health System|Health")
	FORCEINLINE float GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	UFUNCTION(BlueprintPure, Category = "Health System|Health")
	FORCEINLINE float GetMaxHealth() const
	{
		return MaxHealth;
	}

	UFUNCTION(BlueprintPure, Category = "Health System|Armour")
	FORCEINLINE bool IsUseArmour() const
	{
		return bIsUseArmour;
	}

	UFUNCTION(BlueprintPure, Category = "Health System|Armour")
	FORCEINLINE float GetCurrentArmour() const
	{
		return CurrentArmour;
	}

	UFUNCTION(BlueprintPure, Category = "Health System|Armour")
	FORCEINLINE float GetMaxArmour() const
	{
		return MaxArmour;
	}

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FFloatValue HealthUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FFloatValue ArmourUpdated;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDamageTakenEvent DamageTaken;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDeathCauserEvent AuthoritativeDeath;
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FDeathEvent Death;

protected:

	UFUNCTION(BlueprintCallable, Category = "Health System")
	void OnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health System")
	void OnPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health System")
	void OnRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	// Notifies all clients that a the actor has been hit and from what direction.
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastDamageTaken(float Value, FVector Source, FVector Impact, AActor* DamageCauser);

	UFUNCTION()
		void OnRep_CurrentHealth();

	UFUNCTION()
		void OnRep_CurrentArmour();

	// Max health this actor can have.
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = "1"))
		float MaxHealth;

	// Current health of the actor, can be at most MaxHealth.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth, Category = "Health")
		float CurrentHealth;

	// Max armour this actor can have.
	UPROPERTY(EditDefaultsOnly, Category = "Armour")
		bool bIsUseArmour = true;

	// Max armour this actor can have.
	UPROPERTY(EditDefaultsOnly, Category = "Armour", meta = (ClampMin = "1", editcondition = "bIsUseArmour"))
		float MaxArmour;

	// Current armour of the actor, can be at most MaxArmour.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentArmour, Category = "Armour")
		float CurrentArmour;

	FTimerHandle HealthRegenerationHandle;

	UFUNCTION()
		void RegenerateHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenInterval;

	FTimerHandle ArmourRegenerationHandle;

	UFUNCTION()
		void RegenerateArmour();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenInterval;
	
	// When hit by radial damage, we assume the impact point is the owner's actor location plus this value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health System")
		FVector RadialDamageImpactOffset;
};
