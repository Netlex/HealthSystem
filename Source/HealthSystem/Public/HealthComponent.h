// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
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

	// Damage handling
	virtual void TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// Add value for armor
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Health System|Armour")
		void AddArmor(const float Value);

	// Add value for health
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "Health System|Health")
		void AddHealth(const float Value);

	UFUNCTION(BlueprintGetter, Category = "Health System|Health")
	float GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	UFUNCTION(BlueprintGetter, Category = "Health System|Health")
	float GetMaxHealth() const
	{
		return MaxHealth;
	}

	UFUNCTION(BlueprintGetter, Category = "Health System|Armour")
	bool IsUseArmour() const
	{
		return bIsUseArmour;
	}

	UFUNCTION(BlueprintGetter, Category = "Health System|Armour")
	float GetCurrentArmour() const
	{
		return CurrentArmour;
	}

	UFUNCTION(BlueprintGetter, Category = "Health System|Armour")
	float GetMaxArmour() const
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
	
	bool GrantShield(const float Value);
	bool GrantHealth(const float Value);

	UFUNCTION(BlueprintCallable, Category = "Health System", meta = (BlueprintProtected))
	void OnAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health System", meta = (BlueprintProtected))
	void OnPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health System", meta = (BlueprintProtected))
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

	// Enable/Disable use of armor.
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

	// The value that will increase the health during regeneration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenValue;

	// Delay before starting regeneration health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenCooldown;

	// The rate of reaction health.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthRegenInterval;

	FTimerHandle ArmourRegenerationHandle;

	UFUNCTION()
		void RegenerateArmour();

	// The value that will increase the armor during regeneration.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenValue;

	// Delay before starting regeneration armor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenCooldown;

	// The rate of reaction armor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armour", meta = (editcondition = "bIsUseArmour"))
		float ArmourRegenInterval;
	
	// When hit by radial damage, we assume the impact point is the owner's actor location plus this value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health System")
		FVector RadialDamageImpactOffset;
};
