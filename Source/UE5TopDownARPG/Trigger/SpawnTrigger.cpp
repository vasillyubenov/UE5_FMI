// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnTrigger.h"
#include "../UE5TopDownARPG.h"
#include "../UE5TopDownARPGCharacter.h"

ASpawnTrigger::ASpawnTrigger()
{
	SpawnLocationComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationComponent"));
	SpawnLocationComponent->SetupAttachment(RootComponent);
}

void ASpawnTrigger::ActionStart(AActor* ActorInRange)
{
	CurrentWave = 1;

	SpawnWave();
}

void ASpawnTrigger::SpawnWave()
{
	if (CurrentWave >= NumberOfWaves)
	{
		return;
	}

	CurrentWave++;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (int i = 0; i < NumberOfActorsToSpawn; i++)
	{
		AActor* Enemy = GetWorld()->SpawnActor<AActor>(ActorToSpawnClass, SpawnLocationComponent->GetComponentLocation(), FRotator(), SpawnParameters);
		AUE5TopDownARPGCharacter* EnemyCharacter = Cast<AUE5TopDownARPGCharacter>(Enemy);
		if (IsValid(EnemyCharacter)) 
		{
			EnemyCharacter->OnCharacterDeath.BindUObject(this, &ASpawnTrigger::SpawnWaveCheck);
			AliveCount++;
		}
	}
}

void ASpawnTrigger::SpawnWaveCheck()
{
	AliveCount--; 

	// Don't spawn more waves if all enemies are not killed
	if (AliveCount > 0) 
	{
		return;
	}

	SpawnWave();
}