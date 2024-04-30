#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 10    

void UFlockingManager::Init(UWorld* world, UStaticMeshComponent* mesh) {
    UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

    World = world;

    float incr = (PI * 2.f) / AGENT_COUNT;
    for (int i = 0; i < AGENT_COUNT; i++) {
        if (World != nullptr) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin(incr * i) * 150.f;
            location.Z = FMath::Cos(incr * i) * 150.f + 250.f;

            AAgent* agent = World->SpawnActor<AAgent>(location, rotation);
            agent->Init(mesh, i);
            Agents.Add(agent);
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    if (!initialized) return;
    FVector v1, v2, v3;
    for (AAgent* currentBoid : Agents) {
        v1 = rule1(currentBoid);
        v2 = rule2(currentBoid);
        v3 = rule3(currentBoid);

        currentBoid->Velocity += v1 + v2 + v3;

        limitVelocity(currentBoid);
    }
}

FVector UFlockingManager::rule1(AAgent* boid) {
    FVector pc = FVector(0.f);
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pc += currentBoid->GetActorLocation();
    }
    return (pc / (AGENT_COUNT - 1) - boid->GetActorLocation()) / 100;
}

FVector UFlockingManager::rule2(AAgent* boid) {
    FVector c = FVector(0.f);
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid) {
            if (abs(FVector::Dist(currentBoid->GetActorLocation(), boid->GetActorLocation())) < 100) {
                FVector displacement = currentBoid->GetActorLocation() - boid->GetActorLocation();
                c -= displacement;
            }

        }
    }
    return c;
}

FVector UFlockingManager::rule3(AAgent* boid) {
    FVector pv = FVector(0.f);
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pv += currentBoid->Velocity;
    }
    return ((pv / (AGENT_COUNT - 1)) - boid->Velocity) / 8;
}

void UFlockingManager::limitVelocity(AAgent* boid) {
    float limit = 500.f;
    if (abs((int32)boid->Velocity.Size()) > limit)
        boid->Velocity = (boid->Velocity / abs((int32)boid->Velocity.Size())) * limit;
}