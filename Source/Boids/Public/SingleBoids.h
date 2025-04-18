

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "SingleBoids.generated.h"


class ABoidsManager;

UCLASS()
class BOIDS_API ASingleBoids : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASingleBoids();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	int Id;

	UPROPERTY()
	ABoidsManager* BoidsOwner;
	
};
