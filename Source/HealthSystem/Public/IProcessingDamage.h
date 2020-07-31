// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio


#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IProcessingDamage.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class HEALTHSYSTEM_API UProcessingDamage : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface allows you to add additional damage checks using the CanApplyDamage method and can also modify damage using the ModifireDamage method.
 */
class HEALTHSYSTEM_API IProcessingDamage
{
	GENERATED_BODY()

public:

	// Additional damage checks
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health System")
		bool CanApplyDamage(const AActor* Causer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health System")
		float ModifireDamage(const AActor* Causer, float Damage);
};
