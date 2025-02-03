// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class SHOOTINGCPP_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EEndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MakeTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AEnemyActor> EnemyFactory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UArrowComponent* SpawnArrow;

	void MakeEnemy();
	FTimerHandle TimerHandle;

};
