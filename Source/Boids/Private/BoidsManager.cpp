// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidsManager.h"
#include "SingleBoids.h"

// Sets default values
ABoidsManager::ABoidsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoidsManager::BeginPlay()
{
	Super::BeginPlay();

	for (int x = 0; x < NbOfBoids; x++) {

		

		FVector Location(FMath::RandRange(-1000.0f,1000.f), FMath::RandRange(-1000.0f, 1000.f), FMath::RandRange(-1000.0f, 1000.f));
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		ASingleBoids* MaBoids = GetWorld()->SpawnActor<ASingleBoids>(Boids,Location, Rotation);

		MaBoids->Id = x;
		MaBoids->BoidsOwner = this;
		BoidsPosition.Add(Location);
		BoidsDirection.Add(FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f)));

	}


	
}

// Called every frame
void ABoidsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (int i = 0; i < BoidsPosition.Num(); i++) {

		if (BoidsPosition[i].X < -1500.f) BoidsPosition[i].X = 1500.f;
		else if (BoidsPosition[i].X > 1500.f) BoidsPosition[i].X = -1500.f;

		if (BoidsPosition[i].Y < -1500.f) BoidsPosition[i].Y = 1500.f;
		else if (BoidsPosition[i].Y > 1500.f) BoidsPosition[i].Y = -1500.f;

		if (BoidsPosition[i].Z < -1500.f) BoidsPosition[i].Z = 1500.f;
		else if (BoidsPosition[i].Z > 1500.f) BoidsPosition[i].Z = -1500.f;

		FVector Accel = FVector(0.0);
		FVector Separation = FVector(0.0);
		FVector Alignement = FVector(0.0);
		FVector Centroid = FVector(0.0);


		int nb_ali = 0;
		int nb_cen = 0;
		int nb_sep = 0;

		for (int j = 0; j < BoidsPosition.Num(); j++) {
			if (i == j) continue;

			float Dist = FVector::Dist(BoidsPosition[j], BoidsPosition[i]);
			float Angle = acos(BoidsDirection[i].Dot(FVector(BoidsPosition[j] - BoidsPosition[i])));
			float Min_Dist = Dist;

			if (Dist < Dist_Alignement && Angle < FieldOfView) {
				nb_ali++;
				Alignement += BoidsDirection[j];
			}
			if (Dist < Dist_Cohesion && Angle < FieldOfView) {
				nb_cen++;
				Centroid += BoidsPosition[j];
			}
			if (Dist < Dist_Separation && Angle < FieldOfView) {
				nb_sep++;
				Separation += FVector(BoidsPosition[i]-BoidsPosition[j])/Dist;
			}
			
		}

		
		if (nb_ali > 0) Alignement = (Alignement / nb_ali) - BoidsDirection[i];
		if (nb_cen > 0) {
			Centroid = Centroid / nb_cen;
			Centroid = (Centroid - BoidsPosition[i]) - BoidsDirection[i];
		}
		if (nb_sep > 0) Separation = (Separation / nb_sep) - BoidsDirection[i];

		Accel = Alignement.GetSafeNormal()*Str_Alignement;
		Accel += Centroid.GetSafeNormal()*Str_Cohesion;
		Accel += Separation.GetSafeNormal() * Str_Separation;

		BoidsDirection[i] += Accel;

		BoidsPosition[i] += BoidsDirection[i].GetSafeNormal() * DeltaTime * speed;

	}

}