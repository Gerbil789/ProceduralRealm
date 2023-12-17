// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Tile.generated.h"


UENUM()
enum class ETileFlags : uint8
{
	None = 0x00,
	Available = 0x01,
	Occupied = 0x02,
	Empty = 0x04
};

ENUM_CLASS_FLAGS(ETileFlags)


UCLASS()
class PROCEDURALREALM_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();
	virtual void Tick(float DeltaTime) override;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	TArray<ATile*> NeighborTiles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	FVector2D Position;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	float Distance = FLT_MAX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	class AUnit* Unit = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	bool IsAvailable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile")
	FString Color = "Empty";
	

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetUnit(AUnit* _unit);

	void AddNeighbor(ATile* Neighbor);
	float GetDistanceToGoal(ATile* GoalTile);
	void SetPosition(const FVector2D& _position);
	void SetParentTile(ATile* NewParentTile);
	
	ATile* GetParentTile() const;
	void SetDistance(float _distance, int _speed);



protected:
	virtual void BeginPlay() override;

private:
	ATile* parentTile;
	ETileFlags TileFlags;
};
