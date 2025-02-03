﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

UCLASS()
class SHOOTINGCPP_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Default|Comp")
	class UBoxComponent* BoxComp;

	UPROPERTY( EditAnyWhere , BlueprintReadWrite, Category = "Default|Comp" )
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFX;

	UPROPERTY( EditAnyWhere , BlueprintReadWrite)
	float Speed = 1000.0f;

	UFUNCTION()
	void OnBulletOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor ,
						  UPrimitiveComponent* OtherComp , int32 OtherBodyIndex ,
						  bool bFromSweep , const FHitResult& SweeptResult );

	void SetActive( bool bValue );

};
