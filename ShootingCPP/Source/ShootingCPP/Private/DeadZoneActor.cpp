﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadZoneActor.h"
#include "PlayerPawn.h"
#include "BulletActor.h"

// Sets default values
ADeadZoneActor::ADeadZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트를 생성하고 루트 컴포넌트로 설정한다

	BoxComp = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComp" ) );
	SetRootComponent( BoxComp );

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( RootComponent );

	// 박스 컴포넌트의 모빌리티를 고정 상태로 설정한다
	BoxComp->SetMobility( EComponentMobility::Static );

	// 박스 크길ㅡㄹ 50, 50, 50으로 설정한다.
	BoxComp->SetBoxExtent( FVector( 50.0f ) );
	
	BoxComp->SetCollisionProfileName( FName( "DeadZone" ) );

	BoxComp->OnComponentBeginOverlap.AddDynamic( this , &ADeadZoneActor::OnDeadZoneOverlap );

}

// Called when the game starts or when spawned
void ADeadZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeadZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadZoneActor::NotifyActorBeginOverlap( AActor* OtherActor )
{

	// 부딪힌 상대가 총알이라면 비활성화 하고 싶다.
	ABulletActor* bullet = Cast<ABulletActor>( OtherActor );
	if (bullet) {
		bullet->SetActive( false );
	}
	else {
		if (OtherActor->IsA<APlayerPawn>()) {
			// 부딪힌 상대를 파괴하고 싶다.
			OtherActor->Destroy();
		}
	}
}

void ADeadZoneActor::OnDeadZoneOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweeptResult )
{
	// 부딪힌 OtherActor가 에너미이거나 총알이면 파괴하고 싶다
	APlayerPawn* player = Cast<APlayerPawn>( OtherActor );
	// 플레이어가 아니면
	if (player == nullptr)
	{
		// 부딪힌 상대를 파괴하고 싶다
		OtherActor->Destroy();

	}

	/*AEnemyActor* enemy = Cast<AEnemyActor>( OtherActor );
	if (enemy != nullptr)
	{
		OtherActor->Destroy();
	}
	else
	{
		ABulletActor* bullet = Cast<ABulletActor>( OtherActor );
		if (bullet != nullptr)
		{
			OtherActor->Destroy();
		}
	}*/
	

}

