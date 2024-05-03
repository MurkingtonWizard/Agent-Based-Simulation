# Agent-Based Simulation

This project is for WPI's IMGD 4000 Agent-Based Simulation assignment. Based off of the Boids algorithm, this projects demonstrates flocking within Unreal Engine 5.2.1. For the Boids algorithm, I implemented rules 1-3 and the "Limiting the Speed" tweak. My gameplay video can be found at [Gameplay Video](../Agent-Based-Simulation/blob/main/Boids%20Gameplay.mp4).

## Rule 1

```FVector UFlockingManager::rule1(AAgent* boid) {
    FVector pc = FVector(0.f);
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pc += currentBoid->GetActorLocation();
    }
    return (pc / (AGENT_COUNT - 1) - boid->GetActorLocation()) / 100;
}
```

## Rule 2

```FVector UFlockingManager::rule2(AAgent* boid) {
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
```

## Rule 3

```FVector UFlockingManager::rule3(AAgent* boid) {
    FVector pv = FVector(0.f);
    for (AAgent* currentBoid : Agents) {
        if (currentBoid != boid)
            pv += currentBoid->Velocity;
    }
    return ((pv / (AGENT_COUNT - 1)) - boid->Velocity) / 8;
}
```

## Limiting the Speed Tweak

```void UFlockingManager::limitVelocity(AAgent* boid) {
    float limit = 500.f;
    if (abs((int32)boid->Velocity.Size()) > limit)
        boid->Velocity = (boid->Velocity / abs((int32)boid->Velocity.Size())) * limit;
}
```
