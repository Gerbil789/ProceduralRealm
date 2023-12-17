// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"


UCLASS()
class PROCEDURALREALM_API ATileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "TileManager")
	void SpawnTileGrid(int32 InNumRows, int32 InNumColumns, float InTileSpacing = 100.0f);

	UPROPERTY(EditAnywhere, Category = "TileManager")
	TSubclassOf<class ATile> TileBlueprintClass; // Reference to the Tile Blueprint class

	UFUNCTION(BlueprintCallable, Category = "TileManager")
	ATile* GetTile(FVector2D _position);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	class AUnit* ActiveUnit = nullptr;

	UFUNCTION(BlueprintCallable, Category = "TileManager")
	TArray<ATile*> GetTilePath(ATile* Start, ATile* Goal);

	UFUNCTION(BlueprintCallable, Category = "TileManager")
	TArray<ATile*> GetAvailableTiles(ATile* Start, float Speed);

	UFUNCTION(BlueprintCallable, Category = "TileManager")
	void SetActiveUnit(AUnit* _unit);


	// Array to hold references to all tiles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileManager")
	TArray<ATile*> AllTiles;

	// 2D array to represent the grid of tiles
	TArray<TArray<class ATile*>> TileGrid;

private:
	

	// Function to assign neighbors to each tile
	void AssignTileNeighbors();


	//recursive function
	void SetDistance(ATile* Parent, ATile* Tile, float Distance);

	

};
