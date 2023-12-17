// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unit.generated.h"


UCLASS()
class PROCEDURALREALM_API AUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnit();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	class ATile* Tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	FVector2D Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	int Speed = 5; //how many tiles can move

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	int Health = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	int Armor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
	int Initiative = 5; //turn order

protected:
	virtual void BeginPlay() override;

};
