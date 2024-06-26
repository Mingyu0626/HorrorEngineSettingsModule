

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointLightObject.generated.h"

UCLASS()
class HORRORENGINE_API APointLightObject : public AActor
{
	GENERATED_BODY()

	public:
	// Sets default values for this actor's properties
	APointLightObject();
	APointLightObject(FVector LightColor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InstanceLightColor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// PointLightComponent to represent the light
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Light")
	class UPointLightComponent* PointLightComponent;

	// Function to set the light color
	void SetLightColor(FVector LightColor);
};
