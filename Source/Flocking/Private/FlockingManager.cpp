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
            location.Z = FMath::Cos(incr * i) * 150.f;

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

        currentBoid->Velocity = currentBoid->Velocity + v1 + v2 + v3;
        
        FVector loc = currentBoid->GetActorLocation() + currentBoid->Velocity;
        if (loc.X > 500) loc.Set(500, loc.Y, loc.Z);
        if (loc.Y > 500) loc.Set(loc.X, 500, loc.Z);
        if (loc.X < -500) loc.Set(-500, loc.Y, loc.Z);
        if (loc.Y < -500) loc.Set(loc.X, -500, loc.Z);
        if (loc.Z > 500) loc.Set(loc.X, loc.Y, 500);
        if (loc.Z < -500) loc.Set(loc.X, loc.Y, 0);
        /*
        FStringFormatOrderedArguments LogArguments;
        LogArguments.Add(FString::SanitizeFloat(loc.X));
        LogArguments.Add(FString::SanitizeFloat(loc.Y));
        LogArguments.Add(FString::SanitizeFloat(loc.Z));
        FString LogString = FString::Format(TEXT("%s"), loc.ToString());*/
        GEngine->AddOnScreenDebugMessage((uint64)-1, 5.f, FColor::White, loc.ToString());

        currentBoid->SetActorLocation(loc);
    }
}

FVector UFlockingManager::rule1(AAgent* boid) {
    FVector pc;
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pc += currentBoid->GetActorLocation();
    }
    return (pc - boid->GetActorLocation()) / 100;
}

FVector UFlockingManager::rule2(AAgent* boid) {
    FVector c;
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid) {
            if (FVector::Distance(currentBoid->GetActorLocation(), boid->GetActorLocation()) < 100) {
                FVector displacement = currentBoid->GetActorLocation() - boid->GetActorLocation();
                c = c - displacement;
            }

        }
    }
    return c;
}

FVector UFlockingManager::rule3(AAgent* boid) {
    FVector pv;
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pv += currentBoid->Velocity;
    }
    return (pv - boid->Velocity) / 8;
}
