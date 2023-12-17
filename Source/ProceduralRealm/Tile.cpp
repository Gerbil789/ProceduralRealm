
#include "Tile.h"
#include "Unit.h"

// Sets default values
ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;
	parentTile = nullptr;
}


// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::AddNeighbor(ATile* Neighbor)
{
	if (Neighbor)
	{
		NeighborTiles.AddUnique(Neighbor);
	}
}



float ATile::GetDistanceToGoal(ATile* GoalTile)
{
	if (GoalTile)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector GoalLocation = GoalTile->GetActorLocation();

		return FVector::Dist(CurrentLocation, GoalLocation);
	}
	return MAX_FLT;
}

void ATile::SetPosition(const FVector2D& _position)
{
	Position = _position;
}

void ATile::SetParentTile(ATile* _arentTile)
{
	parentTile = _arentTile;
}

void ATile::SetUnit(AUnit* _unit)
{
	Unit = _unit;
	Unit->Tile = this;
}

ATile* ATile::GetParentTile() const
{
	return parentTile;
}

void ATile::SetDistance(float _distance, int _speed)
{
	Distance = _distance;
	IsAvailable = (_speed >= Distance);

	UE_LOG(LogTemp, Warning, TEXT("[Tile] distance: %f\t speed: %d\t is available %s"), Distance, _speed, IsAvailable ? TEXT("true") : TEXT("false"));
}


