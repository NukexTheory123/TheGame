// Fill out your copyright notice in the Description page of Project Settings.


#include "TheLand.h"

void ATheLand::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	despawnMap();
	for (FNoise& noise : HeightMap) {
		//if(!noise.FastNoiseWrapper)
		initNoise(noise);
	}
	generateMap();
}

// Sets default values
ATheLand::ATheLand()
{
	PrimaryActorTick.bCanEverTick = false;
	mapsize = 1;
	Map.Init(nullptr, mapsize * mapsize);
}


void ATheLand::generateMap(){ 
	int i = 0;
	Map.Empty();
	Map.Init(nullptr, mapsize * mapsize);

	for (int x = -1 * (mapsize / 2); x < (mapsize / 2); ++x) {
		for (int y = -1 * (mapsize / 2); y < mapsize / 2; ++y) {
			if (i < Map.Num()) {
				FVector SpawnLocation = FVector(x * 1500.0f, y * 1500.0f, 0.0f);
				AMyChunk* NewChunk = GetWorld()->SpawnActorDeferred<AMyChunk>(AMyChunk::StaticClass(), FTransform::Identity);

				if (NewChunk) {
					NewChunk->xoffset = x;
					NewChunk->yoffset = y;
					NewChunk->material = LandscapeMaterial;
					//NewChunk->zmult = zmultiplier;
					NewChunk->NoiseMap = HeightMap;

					// Finish spawning for the newly spawned actor
					NewChunk->FinishSpawning(FTransform(SpawnLocation));
					Map[i] = NewChunk;
					//UE_LOG(LogTemp, Warning, TEXT("Spawned chunk at (%f, %f)"), SpawnLocation.X, SpawnLocation.Y);
				}
				++i;
			}
		}
	}
}

void ATheLand::despawnMap() {
	for (AMyChunk* Chunk : Map)
	{
		if (Chunk && !Chunk->IsPendingKillOrUnreachable())
		{
			Chunk->Destroy();
		}
	}
	Map.Empty();
}

// Called every frame
void ATheLand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATheLand::BeginPlay()
{
	Super::BeginPlay();

}

void ATheLand::initNoise(FNoise& Noise) {
	int32 UniqueID = FMath::Rand();
	FString UniqueName = FString::Printf(TEXT("FastNoiseWrapper_%d"), UniqueID);
	Noise.FastNoiseWrapper = NewObject<UFastNoiseWrapper>(this, *UniqueName);
	Noise.FastNoiseWrapper->SetupFastNoise(Noise.noiseType, Noise.seed, Noise.frequency, Noise.interp, Noise.fractaltype, Noise.octaves, Noise.lacunarity, Noise.gain, Noise.cellularJitter, Noise.cellularDistanceFunction, Noise.cellularReturnType);
}