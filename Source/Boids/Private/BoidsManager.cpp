// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidsManager.h"
#include "SingleBoids.h"
#include "Kismet/KismetMathLibrary.h"
#include <omp.h>

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

		FVector Location(FMath::RandRange(1.f, 2999.f), FMath::RandRange(1.0f, 2999.f), FMath::RandRange(1.0f, 2999.f));
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		ASingleBoids* MaBoids = GetWorld()->SpawnActor<ASingleBoids>(Boids,Location, Rotation);
		FVector Dir = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f));
		MaBoids->Id = x;
		MaBoids->BoidsOwner = this;

		//Array
		BoidsPosition.Add(Location);
		BoidsDirection.Add(Dir);


		//Buffr Array
		BufBoidsPosition.Add(Location);
		BufBoidsDir.Add(Dir);
	}
	
}

// Called every frame
void ABoidsManager::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	ParallelFor(BoidsPosition.Num(), [&](int32 i) {


		// Box tp stuff
		if (BoidsPosition[i].X < 0.f) BufBoidsPosition[i].X = 3000.f;
		else if (BoidsPosition[i].X > 3000.f) BufBoidsPosition[i].X = 0.f;

		if (BoidsPosition[i].Y < 0.f) BufBoidsPosition[i].Y = 3000.f;
		else if (BoidsPosition[i].Y > 3000.f) BufBoidsPosition[i].Y = 0.f;

		if (BoidsPosition[i].Z < 0.f) BufBoidsPosition[i].Z = 3000.f;
		else if (BoidsPosition[i].Z > 3000.f) BufBoidsPosition[i].Z = 0.f;

		FVector Accel = FVector(0.0);
		FVector Separation = FVector(0.0);
		FVector Alignement = FVector(0.0);
		FVector Centroid = FVector(0.0);
		FVector Collision = FVector(0.0);

		int nb_ali = 0;
		int nb_cen = 0;
		int nb_sep = 0;
		int nb_col = 0;

		// Calculation of different rules
		for (int j = 0; j < BoidsPosition.Num(); j++) {
			if (i == j) {}
			else {
				float Dist = FVector::Dist(BoidsPosition[i], BoidsPosition[j]);
				float Angle = acos(BoidsDirection[i].Dot(FVector(BoidsPosition[j] - BoidsPosition[i])));
				float Min_Dist = Dist;
				float RFoV = FMath::DegreesToRadians(FieldOfView*0.5f);

				if (Dist < Dist_Alignement && Angle < RFoV) {
					nb_ali++;
					Alignement += BoidsDirection[j];
				}
				if (Dist < Dist_Cohesion && Angle < RFoV) {
					nb_cen++;
					Centroid += BoidsPosition[j];
				}
				if (Dist < Dist_Separation && Angle < RFoV) {
					nb_sep++;
					Separation += FVector(BoidsPosition[i] - BoidsPosition[j]) * (Dist/Dist_Separation);
				}
			}


		}
		// Collision avoidace
		for (int ray = 0; ray < 16; ray++) {
			float t = ray / 16.f;
			float theta = FMath::Acos(1.f - 2.f * t);
			float phi = 2.f * 3.1415f * 1.618f * ray;
			FVector Pos;
			Pos.X = FMath::Sin(theta) * FMath::Cos(phi);
			Pos.Y = FMath::Sin(theta) * FMath::Sin(phi);
			Pos.Z = FMath::Cos(theta);

			FHitResult ResultatHit;
			FCollisionQueryParams ParamsCollision;
			ParamsCollision.AddIgnoredActor(this);

			bool aHit = GetWorld()->LineTraceSingleByChannel(
				ResultatHit,
				BoidsPosition[i],
				BoidsPosition[i] + (Pos * 60),
				ECC_Visibility,
				ParamsCollision
			);
			if (aHit) {
				nb_col++;
				Collision += ResultatHit.ImpactNormal;
			}
		}

		//Steering forces
		if (nb_ali > 0) Alignement = (Alignement / nb_ali) - BoidsDirection[i];
		if (nb_cen > 0) {
			Centroid = Centroid / nb_cen;
			Centroid = (Centroid - BoidsPosition[i]) - BoidsDirection[i];
		}
		if (nb_sep > 0) Separation = (Separation / nb_sep) - BoidsDirection[i];

		//Adding the forces with their corresponding weight
		Accel = Alignement.GetSafeNormal() * Str_Alignement;
		Accel += Centroid.GetSafeNormal() * Str_Cohesion;
		Accel += Separation.GetSafeNormal() * Str_Separation;
		Accel += Collision.GetSafeNormal() * Str_Collision;


		//Updating the buffer
		BufBoidsDir[i] = BoidsDirection[i] + Accel;

		BufBoidsPosition[i] += BufBoidsDir[i].GetSafeNormal() * DeltaTime * speed;

		}, false);

	//Updating the Positon and Direction array with the buffers
	ParallelFor(BoidsPosition.Num(), [&](int32 x) {
		BoidsDirection[x] = BufBoidsDir[x].GetSafeNormal();
		BoidsPosition[x] = BufBoidsPosition[x];
		}, false);
}