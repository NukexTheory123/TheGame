// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseWrapper.h"
#include "MyChunk.generated.h"

USTRUCT()
struct FNoise
{
	GENERATED_BODY()

public:
	UFastNoiseWrapper* FastNoiseWrapper;
	UPROPERTY(EditAnywhere, Category = "Noise")
		EFastNoise_NoiseType noiseType = EFastNoise_NoiseType::Simplex;
	UPROPERTY(EditAnywhere, Category = "Noise")
		int32 seed = 1339;
	UPROPERTY(EditAnywhere, Category = "Noise")
		float frequency = 0.01f;
	UPROPERTY(EditAnywhere, Category = "Noise")
		EFastNoise_Interp interp = EFastNoise_Interp::Quintic;
	UPROPERTY(EditAnywhere, Category = "Noise")
		EFastNoise_FractalType fractaltype = EFastNoise_FractalType::FBM;
	UPROPERTY(EditAnywhere, Category = "Noise")
		int32 octaves = 3;
	UPROPERTY(EditAnywhere, Category = "Noise")
		float lacunarity = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Noise")
		float gain = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Noise")
		float cellularJitter = 0.45f;
	UPROPERTY(EditAnywhere, Category = "Noise")
		EFastNoise_CellularDistanceFunction cellularDistanceFunction = EFastNoise_CellularDistanceFunction::Euclidean;
	UPROPERTY(EditAnywhere, Category = "Noise")
		EFastNoise_CellularReturnType cellularReturnType = EFastNoise_CellularReturnType::CellValue;
	UPROPERTY(EditAnywhere, Category = "Noise")
		float zmult;
	UPROPERTY(EditAnywhere, Category = "Noise")
		UCurveFloat* HeightCurve;

};

UCLASS()
class THEGAME_API AMyChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyChunk();
	UFUNCTION()
	void initNoise(FNoise& Noise);
	UPROPERTY(EditAnywhere, Category = "Thevars")
		int chunksize;
	UPROPERTY(EditAnywhere, Category = "Thevars")
		float scale;
	UPROPERTY(EditAnywhere, Category = "Thevars")
		float uvscale;
	UPROPERTY(EditAnywhere, Category = "Thevars")
		float xoffset;
	UPROPERTY(EditAnywhere, Category = "Thevars")
		float yoffset;

	UPROPERTY(EditAnywhere, Category = "Thevars")
		bool DrawSpheres;
	UPROPERTY(EditAnywhere, Category = "Thevars")
		UMaterialInterface* material;

	UProceduralMeshComponent* ProceduralMesh;
	//FNoise noise;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//virtual void PostInitializeComponents(); change mesh during runtime;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UVO;
	TArray<FVector> Normals;
	TArray<struct FProcMeshTangent> Tangents;
	TArray<FNoise> NoiseMap;

	UFUNCTION(BlueprintCallable, Category = "Thevars")
		void generateMesh();
	
	float generateNoise(float x, float y);
	void createVertices();
	void createTriangles();

};

