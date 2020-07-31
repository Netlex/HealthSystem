// Project:         Health System
// Copyright:       Copyright (C) 2020 Netlex Studio
// Original Author: Netlex Studio

#include "ControllerEventsComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"


UControllerEventsComponent::UControllerEventsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UControllerEventsComponent::Death_Implementation(const AController* Killer)
{
	DeathEvent.Broadcast(Killer);

	if (Killer != nullptr)
	{
		APlayerState* KillerPlayerState = Killer->PlayerState;
		if (KillerPlayerState != nullptr)
		{
			ClientInformOfDeath(KillerPlayerState);
		}
	}
}

void UControllerEventsComponent::Kill_Implementation(const AController* Victim)
{
	KillEvent.Broadcast(Victim);

	if (Victim != nullptr)
	{
		APlayerState* VictimPlayerState = Victim->PlayerState;
		if (VictimPlayerState != nullptr)
		{
			ClientInformOfKill(VictimPlayerState);
		}
	}
}

void UControllerEventsComponent::ClientInformOfKill_Implementation(const APlayerState* Victim)
{
	KillDetailsEvent.Broadcast(Victim);
}

void UControllerEventsComponent::ClientInformOfDeath_Implementation(const APlayerState* Killer)
{
	DeathDetailsEvent.Broadcast(Killer);
}
