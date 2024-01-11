// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyChunk.h"
#include "TheLand.generated.h"



UCLASS()
class THEGAME_API ATheLand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheLand();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Land")
	TArray<AMyChunk*> Map;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Land")
	int mapsize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Land")
	UMaterialInterface* LandscapeMaterial;

	float zmultiplier;
	UPROPERTY(EditAnywhere, Category = "Land")
	TArray<FNoise> HeightMap;

	int seed;
	bool falloff;

	void generateFalloff();
	void initNoise(FNoise& Noise);
	void generateMap();
	void despawnMap();
};
