// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChunk.h"
#include "KismetProceduralMeshLibrary.h"

//if none of this works change the FNoise* noise to what that was just typed and redo all the steps

//this function will construct the mesh before the game has run, it needs a function like generatedmesh for it to work. 
void AMyChunk::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	if (!GetWorld()->IsGameWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("Chunkconstruct"));
		generateMesh();
	}
}

// Sets default values
AMyChunk::AMyChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	//ProceduralMesh->SetupAttachment(RootComponent);

	RootComponent = ProceduralMesh;

	chunksize = 30;
	scale = 50.0f;
	uvscale = 1.0f;
	xoffset = 0;
	yoffset = 0;
}



// Called when the game starts or when spawned
void AMyChunk::BeginPlay()
{
	Super::BeginPlay();
	//generateMesh();
}




// Called every frame
void AMyChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyChunk::generateMesh() {
	//SetActorLocation(FVector::ZeroVector);
	//SetActorRotation(FRotator::ZeroRotator);
	Vertices.Reset();
	Triangles.Reset();
	UVO.Reset();
	Normals.Reset();
	Tangents.Reset();

	createVertices();
	createTriangles();

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVO, Normals, Tangents);

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVO, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, material);
}

void AMyChunk::createVertices() {
	float value = 0;

	for (float x = 0; x <= chunksize; ++x) {
		for (float y = 0; y <= chunksize; ++y) {
			value = generateNoise(x, y);
			//UE_LOG(LogTemp, Warning, TEXT("Value: %f"), value);
			Vertices.Add(FVector(x * scale, y * scale, value));
			UVO.Add(FVector2D(x * uvscale, y * uvscale));

			if (DrawSpheres) {
				DrawDebugSphere(GetWorld(), FVector(x * scale, y * scale, 0), 25.0f, 16, FColor::Red, true, -1.0f, 0U, 0.0f);
			}
		}
	}
}

void AMyChunk::createTriangles() {
	int vertex = 0;

	for (int x = 0; x < chunksize; ++x) {
		for (int y = 0; y < chunksize; ++y) {
			Triangles.Add(vertex);
			Triangles.Add(vertex + 1);
			Triangles.Add(vertex + chunksize + 1);
			Triangles.Add(vertex + 1);
			Triangles.Add(vertex + chunksize + 2);
			Triangles.Add(vertex + chunksize + 1);

			++vertex;
		}
		++vertex;
	}
}

void AMyChunk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Destroy();
}

float AMyChunk::generateNoise(float x, float y) {
	float sum = 0;
	for (FNoise& Noise : NoiseMap) {
			float value1 = Noise.FastNoiseWrapper->GetNoise2D(x + (xoffset * chunksize), y + (yoffset * chunksize));
			UE_LOG(LogTemp, Warning, TEXT("Value at the beginning: %f"), value1);
			if (Noise.HeightCurve) {
				value1 = Noise.HeightCurve->GetFloatValue(value1);
				//UE_LOG(LogTemp, Warning, TEXT("Value at the after curve: %f"), value1);
			}
			value1 = value1 * Noise.zmult;
			//UE_LOG(LogTemp, Warning, TEXT("Value at the after: %f"), value1);
			sum += value1;
	}

	if (NoiseMap.Num() > 0) {
		sum /= NoiseMap.Num();
	}

	return sum;
}
void AMyChunk::initNoise(FNoise& Noise) {
	Noise.FastNoiseWrapper = NewObject<UFastNoiseWrapper>(this, TEXT("FastNoise"));
	Noise.FastNoiseWrapper->SetupFastNoise(Noise.noiseType, Noise.seed, Noise.frequency, Noise.interp, Noise.fractaltype, Noise.octaves, Noise.lacunarity, Noise.gain, Noise.cellularJitter, Noise.cellularDistanceFunction, Noise.cellularReturnType);
}