# Boids Simulation in Unreal Engine 5

This project implements a classic Boids flocking algorithm in Unreal Engine 5 using C++. The simulation demonstrates emergent behavior where complex flocking patterns arise from a set of simple rules applied to each individual boid.

## Overview

Boids (bird-oid objects) is an artificial life program developed by Craig Reynolds in 1986 that simulates the flocking behavior of birds. This implementation includes:

- The three classic boid rules:
  - **Separation**: Avoid crowding neighbors
  - **Alignment**: Steer towards average heading of neighbors
  - **Cohesion**: Steer towards average position of neighbors
- **Obstacle Avoidance**: Boids detect and avoid obstacles in the environment
- **Bounded Environment**: Boids wrap around the boundaries of a 3000×3000×3000 unit cube

## Getting Started

### Prerequisites

- Unreal Engine 5.4 or later
- C++ development environment configured for UE5

### Installation

1. Clone or download this repository
2. Open the project in Unreal Engine 5
3. Compile the C++ code through the editor or your IDE

## Usage

1. Place a `BoidsManager` actor in your level
2. Set the `Boids` property to your `SingleBoids` blueprint or class
3. Adjust the parameters to your liking (see Configuration section)
4. Play the level to see the boids in action

## How It Works

### Architecture

The implementation consists of two main classes:

1. **BoidsManager**: Controls the simulation, applies rules, and updates positions
2. **SingleBoids**: Individual actors that represent each boid in the world

The manager handles all the complex calculations in a centralized way, while the individual boids simply follow the directions provided by the manager.

### Simulation Loop

1. On BeginPlay:
   - The manager spawns the specified number of boids at random positions
   - Each boid is assigned a unique ID and random initial direction

2. On each frame:
   - The manager calculates steering forces for each boid based on nearby boids
   - Collision detection is performed using ray casting in multiple directions
   - Position and direction updates are calculated in parallel for performance
   - Boids are wrapped around the boundaries if they exit the simulation area
   - Individual boid actors update their location and rotation based on data from the manager

### Flocking Rules Implementation

#### Separation
Boids steer to avoid crowding local flockmates. The force is stronger when boids get closer together, creating a natural spacing.

```cpp
if (Dist < Dist_Separation && Angle < RFoV) {
    nb_sep++;
    Separation += FVector(BoidsPosition[i] - BoidsPosition[j]) * (Dist/Dist_Separation);
}
```

#### Alignment
Boids steer towards the average heading of local flockmates, causing them to align their direction of travel.

```cpp
if (Dist < Dist_Alignement && Angle < RFoV) {
    nb_ali++;
    Alignement += BoidsDirection[j];
}
```

#### Cohesion
Boids steer towards the average position of local flockmates, keeping the flock together.

```cpp
if (Dist < Dist_Cohesion && Angle < RFoV) {
    nb_cen++;
    Centroid += BoidsPosition[j];
}
```

#### Obstacle Avoidance
Boids cast rays in multiple directions to detect obstacles and steer away from them.

```cpp
// Collision avoidance using spherical point distribution for ray casting
for (int ray = 0; ray < 16; ray++) {
    // Spherical point calculation
    // Line trace to detect obstacles
    if (aHit) {
        nb_col++;
        Collision += ResultatHit.ImpactNormal;
    }
}
```

## Configuration

The following parameters can be adjusted in the editor:

### Basic Settings
- **NbOfBoids**: Number of boids to spawn
- **Speed**: Movement speed of the boids

### Rule Weights
- **Str_Alignement**: Weight of the alignment rule
- **Str_Cohesion**: Weight of the cohesion rule
- **Str_Separation**: Weight of the separation rule
- **Str_Collision**: Weight of the collision avoidance rule

### Perception
- **Dist_Alignement**: Distance within which alignment is considered
- **Dist_Cohesion**: Distance within which cohesion is considered
- **Dist_Separation**: Distance within which separation is considered
- **FieldOfView**: Angular field of view for boid perception (in degrees)

## Performance Considerations

- The implementation uses Unreal's parallel processing for improved performance: `ParallelFor()` is used to update all boids simultaneously
- The architecture separates calculation (BoidsManager) from visualization (SingleBoids)
- Collision detection is optimized using a fixed number of rays per boid

## Extending the Project

Some ideas for extending the simulation:

- Add predator avoidance behavior
- Implement different boid types with varying rules
- Add environmental influences like wind or currents
- Create a user interface for real-time parameter adjustment
- Add a goal-seeking behavior

## License

This project uses code based on Unreal Engine, which is subject to the terms of the Unreal Engine End User License Agreement.

## Acknowledgments

- Craig Reynolds for the original Boids algorithm
- Epic Games for Unreal Engine