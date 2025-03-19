


#include "SingleBoids.h"
#include "BoidsManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASingleBoids::ASingleBoids()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingleBoids::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASingleBoids::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector Pos = BoidsOwner->BoidsPosition[Id];

	SetActorLocation(Pos);
	SetActorRotation(BoidsOwner->BoidsDirection[Id].ToOrientationRotator());
	

}


