// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "ControllerEventsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerEvent, const AController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKillDetailsEvent, const APlayerState*, Victim);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEALTHSYSTEM_API UControllerEventsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UControllerEventsComponent();

	UFUNCTION(Server, Reliable)
		void Death(const AController* Killer);
	UFUNCTION(Server, Reliable)
		void Kill(const AController* Victim);

	UPROPERTY(BlueprintAssignable)
		FControllerEvent DeathEvent;
	UPROPERTY(BlueprintAssignable)
		FControllerEvent KillEvent;

	UFUNCTION(Client, Reliable)
		void ClientInformOfKill(const APlayerState* Victim);
	UFUNCTION(Client, Reliable)
		void ClientInformOfDeath(const APlayerState* Killer);
	
	UPROPERTY(BlueprintAssignable)
		FKillDetailsEvent KillDetailsEvent;
	UPROPERTY(BlueprintAssignable)
		FKillDetailsEvent DeathDetailsEvent;
};
