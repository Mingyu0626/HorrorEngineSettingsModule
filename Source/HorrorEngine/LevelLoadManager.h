// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelLoadManager.generated.h"

UCLASS()
class HORRORENGINE_API ALevelLoadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelLoadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LoadLevel();

	
private:
	int Level = 1;

	UPROPERTY(EditDefaultsOnly)
	FVector ChairSpawnPoint;
	UPROPERTY(EditDefaultsOnly)
	FVector LightSpawnPoint;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AChairObject> ChairObjectClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APointLightObject> PointLightObjectClass;
};
