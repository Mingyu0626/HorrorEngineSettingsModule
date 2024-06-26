// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairObject.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AChairObject::AChairObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ChairMesh;
}

// Called when the game starts or when spawned
void AChairObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChairObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

