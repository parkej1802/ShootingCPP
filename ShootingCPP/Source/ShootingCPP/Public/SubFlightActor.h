// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "SubFlightActor.generated.h"

UCLASS()
class SHOOTINGCPP_API ASubFlightActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASubFlightActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* FirePositionComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> BulletFactory;

	void MakeBullet();

	UPROPERTY( EditAnywhere , BlueprintReadWrite )
	float CurrentTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireTime = 0.2f;

	UPROPERTY(EditAnywhere)
	bool bStartFire = false;

	UPROPERTY(EditAnywhere)
	class USceneComponent* Target;

	UPROPERTY(EditAnywhere)
	float MoveToTargetSpeed = 700.0f;


};
