// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelLoadManager.h"
#include "PointLightObject.h"
#include "ChairObject.h"

// Sets default values
ALevelLoadManager::ALevelLoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevelLoadManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelLoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelLoadManager::LoadLevel()
{
	FVector ChairLocation; // spawnpoint->getcomponentlocation()으로 변경
	FRotator ChairRotation; // 이것도 GetComponentRotation()으로 변경
	FVector LightLocation; // spawnpoint->getcomponentlocation()으로 변경
	FRotator LightRotation; // 이것도 GetComponentRotation()으로 변경
    switch (Level)
    {
    case 1:
        UE_LOG(LogTemp, Log, TEXT("level1"));
		ChairLocation = ChairSpawnPoint;
		ChairRotation = FRotator(0,0,0);
		GetWorld()->SpawnActor<AChairObject>(ChairObjectClass, ChairLocation, ChairRotation);

		
		break;

    case 2:
        UE_LOG(LogTemp, Log, TEXT("level2"));

		LightLocation = LightSpawnPoint;
		LightRotation = FRotator(0,0,0);
		GetWorld()->SpawnActor<APointLightObject>(PointLightObjectClass, LightLocation, LightRotation);
        break;

    default:
        UE_LOG(LogTemp, Log, TEXT("level def"));
        break;
    }

    Level++;
}
