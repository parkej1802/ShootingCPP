// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UBoxComponent;
class ABulletActor;

UCLASS()
class SHOOTINGCPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY( EditAnyWhere , BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY( EditAnyWhere , BlueprintReadWrite)
	class UArrowComponent* FirePositionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* Sub_L_PositionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* Sub_R_PositionComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASubFlightActor> SubFlightFactory;

	UPROPERTY(EditAnywhere)
	class ASubFlightActor* SubFlight_L;

	UPROPERTY(EditAnywhere)
	class ASubFlightActor* SubFlight_R;

	//UPROPERTY(EditAnywhere)

	// 오른쪽으로 이동하고싶다.
	// P = P0 + v(방향 * speed)t

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Speed = 500.0f;

	// 사용자의 입력에 따라 상하좌우로 이동하고 싶다
	float h , v;

	void OnAxisHorizontal( float value );
	void OnAxisVertical( float value );
	void onActionAutoFire_1();

	void OnActionFire();
	UPROPERTY( EditAnywhere)
	TSubclassOf<class ABulletActor> BulletFactory;

	UPROPERTY(EditAnywhere)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere)
	class AShootingGameMode* GM;

	float HP;
	UPROPERTY(EditAnywhere)
	float MaxHP = 2;

	void SetDamage( int32 damage );

	// 총알을 오브젝트 풀 처리하고 싶다
	// [필요 요소]
	// - 최초 생성할 총알 갯수
	
	UPROPERTY(EditAnywhere)
	int32 MaxBulletCount = 20;

	// -총알 목록
	UPROPERTY()
	TArray<class ABulletActor*> Magazine;

	void MakeBullet();

	// 자동 총쏘기 기능을 만들고 싶다.
	// 자동 발사를 기억하기 위한 변수 bAutoFire;	
	// Tick에서 시간이 흐르다가 발사 시간이 도ㅣ면 총알을 만들고 싶다.

	UPROPERTY( EditAnywhere , blueprintReadWrite );
	bool bAutoFire = false;

	float CurrentTime = 0.0f;
	float FireTime = 0.2f;

};

