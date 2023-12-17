
#include "TileManager.h"
#include "Tile.h"
#include "Unit.h"

ATileManager::ATileManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATileManager::BeginPlay()
{
    Super::BeginPlay();
}

void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATileManager::SpawnTileGrid(int32 InNumRows, int32 InNumColumns, float InTileSpacing)
{
    //delete old tiles
    for (ATile* Tile : AllTiles)
    {
        if (Tile)
        {
            Tile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            Tile->Destroy();
        }
    }
    AllTiles.Empty();
    TileGrid.Empty();

    //spawn new tiles
	for (int32 Row = 0; Row < InNumRows; ++Row)
	{
		TArray<ATile*> RowTiles;
		for (int32 Column = 0; Column < InNumColumns; ++Column)
		{
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            FVector SpawnLocation = FVector(Column * InTileSpacing, Row * InTileSpacing, 0);
            ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileBlueprintClass, SpawnLocation, FRotator::ZeroRotator);

            if (NewTile) {
                NewTile->SetPosition(FVector2D(Row, Column));
                NewTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
                RowTiles.Add(NewTile);
                AllTiles.Add(NewTile);
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Tile."));
                return;
            }
		}
		TileGrid.Add(RowTiles);
	}

    UE_LOG(LogTemp, Warning, TEXT("Number of AllTiles: %d"), AllTiles.Num());
    UE_LOG(LogTemp, Warning, TEXT("Number of TileGrid Rows: %d"), TileGrid.Num());
    if (TileGrid.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Number of TileGrid Columns: %d"), TileGrid[0].Num());
    }

	AssignTileNeighbors();
}

TArray<ATile*> ATileManager::GetTilePath(ATile* Start, ATile* Goal)
{
    TArray<ATile*> Path;
    ATile* current = Goal;

    while (current != Start) {
        Path.Add(current);
        current = current->GetParentTile();

        if (current == nullptr) {
            return TArray<ATile*>(); // if path does not exist, return empty path
        }
    }

    return Path;
}


bool IsDiagonal(ATile* t1, ATile* t2) {
    int deltaX = FMath::Abs(t1->Position.X - t2->Position.X);
    int deltaY = FMath::Abs(t1->Position.Y - t2->Position.Y);

    //UE_LOG(LogTemp, Warning, TEXT("deltaX: %d\t, deltaY: %d"), deltaX, deltaY);

    return deltaX == 1 && deltaY == 1;
}

TArray<ATile*> ATileManager::GetAvailableTiles(ATile* Start, float Speed)
{
    if (Start) {
        UE_LOG(LogTemp, Warning, TEXT("[GetAvailableTiles] : start tile is null"));
        //return TArray<ATile*>();
    }

    for (ATile* t : AllTiles) {
        t->SetDistance(FLT_MAX, 0);
        t->SetParentTile(nullptr);
    }

    SetDistance(nullptr, Start, 0); // set distance from start tile to all other tiles

    TArray<ATile*> tiles;

    for (ATile* t : AllTiles) {
        if (t->IsAvailable) {
            tiles.Add(t);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[GetAvailableTiles] Number of tiles: %d"), tiles.Num());
    return tiles;
}

void ATileManager::SetActiveUnit(AUnit* _unit)
{
    ActiveUnit = _unit;
    GetAvailableTiles(ActiveUnit->Tile, ActiveUnit->Speed);
}

void ATileManager::SetDistance(ATile* Parent, ATile* Tile, float Distance)
{
    if (Tile->Distance < Distance) {
        return;
    }

    Tile->SetDistance(Distance, ActiveUnit->Speed);
    Tile->SetParentTile(Parent);

    for (ATile* Neighbor : Tile->NeighborTiles) {
        if (IsDiagonal(Tile, Neighbor)) {
            SetDistance(Tile, Neighbor, Distance + 1.4f);
        }
        else {
            SetDistance(Tile, Neighbor, Distance + 1.0f);
        }
    }
}


ATile* ATileManager::GetTile(FVector2D _position)
{
    int x = _position.X;
    int y = _position.Y;
    if (x >= 0 && x < TileGrid.Num() && y >= 0 && y < TileGrid[x].Num())
    {
        return TileGrid[x][y];
    }
    UE_LOG(LogTemp, Warning, TEXT("[GetTile] : tile is null"));
    return nullptr;
}

void ATileManager::AssignTileNeighbors() 
{
    for (int32 Row = 0; Row < TileGrid.Num(); ++Row)
    {
        for (int32 Column = 0; Column < TileGrid[Row].Num(); ++Column)
        {
            ATile* CurrentTile = TileGrid[Row][Column];

            // Assigning top neighbor
            if (Row > 0)
            {
                CurrentTile->AddNeighbor(TileGrid[Row - 1][Column]);
            }

            // Assigning bottom neighbor
            if (Row < TileGrid.Num() - 1)
            {
                CurrentTile->AddNeighbor(TileGrid[Row + 1][Column]);
            }

            // Assigning left neighbor
            if (Column > 0)
            {
                CurrentTile->AddNeighbor(TileGrid[Row][Column - 1]);
            }

            // Assigning right neighbor
            if (Column < TileGrid[Row].Num() - 1)
            {
                CurrentTile->AddNeighbor(TileGrid[Row][Column + 1]);
            }

            // Assigning top-left neighbor
            if (Row > 0 && Column > 0)
            {
                CurrentTile->AddNeighbor(TileGrid[Row - 1][Column - 1]);
            }

            // Assigning top-right neighbor
            if (Row > 0 && Column < TileGrid[Row].Num() - 1)
            {
                CurrentTile->AddNeighbor(TileGrid[Row - 1][Column + 1]);
            }

            // Assigning bottom-left neighbor
            if (Row < TileGrid.Num() - 1 && Column > 0)
            {
                CurrentTile->AddNeighbor(TileGrid[Row + 1][Column - 1]);
            }

            // Assigning bottom-right neighbor
            if (Row < TileGrid.Num() - 1 && Column < TileGrid[Row].Num() - 1)
            {
                CurrentTile->AddNeighbor(TileGrid[Row + 1][Column + 1]);
            }
        }
    }
}


