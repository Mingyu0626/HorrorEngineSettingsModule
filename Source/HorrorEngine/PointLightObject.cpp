// Fill out your copyright notice in the Description page of Project Settings.


#include "PointLightObject.h"
#include "Components/PointLightComponent.h"

// Sets default values
APointLightObject::APointLightObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	RootComponent = PointLightComponent;
}
APointLightObject::APointLightObject(FVector Color)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InstanceLightColor = Color;
}
// Called when the game starts or when spawned
void APointLightObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APointLightObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APointLightObject::SetLightColor(FVector LightColor)
{
	if (PointLightComponent)
	{
		PointLightComponent->SetLightColor(FLinearColor(LightColor.X, LightColor.Y, LightColor.Z));
	}
}