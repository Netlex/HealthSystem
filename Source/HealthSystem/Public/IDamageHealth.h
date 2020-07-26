// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class HEALTHSYSTEM_API UDamageHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HEALTHSYSTEM_API IDamageHealth
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health System")
		bool CanApplyDamage(const AActor* Causer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health System")
		float ModifireDamage(const AActor* Causer, float Damage);
};
