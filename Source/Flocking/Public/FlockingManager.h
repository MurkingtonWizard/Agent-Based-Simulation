#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init(UWorld* world, UStaticMeshComponent* mesh);
	void Flock();

private:
	UWorld* World;
	bool initialized;
	TArray<class AAgent*> Agents;

	FVector rule1(AAgent* boid);
	FVector rule2(AAgent* boid);
	FVector rule3(AAgent* boid);

	void limitVelocity(AAgent* boid);
};