// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidsManager.generated.h"


class ASingleBoids;

UCLASS()
class BOIDS_API ABoidsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<ASingleBoids> Boids;

	UPROPERTY(EditAnywhere)
	TArray<FVector> BoidsPosition;

	UPROPERTY(EditAnywhere)
	TArray<FVector> BoidsDirection;

	UPROPERTY(EditAnywhere, Category="Boids")
	int NbOfBoids = 300;

	UPROPERTY(EditAnywhere, Category = "Boids")
	float Str_Alignement = 1.0;

	UPROPERTY(EditAnywhere, Category = "Boids")
	float Str_Cohesion = 1.0;

	UPROPERTY(EditAnywhere, Category = "Boids")
	float Str_Separation = 1.0;

	UPROPERTY(EditAnywhere, Category = "Boids")
	float Dist_Alignement = 300.0;
	UPROPERTY(EditAnywhere, Category = "Boids")
	float Dist_Cohesion = 300.0;
	UPROPERTY(EditAnywhere, Category = "Boids")
	float Dist_Separation = 150.0;

	UPROPERTY(EditAnywhere, Category = "Boids")
	float FieldOfView = 200.0;

};
