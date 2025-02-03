// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "EnemyActor.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>( TEXT( "RootScene" ) );
	SetRootComponent( RootScene );

	SpawnArrow = CreateDefaultSubobject<UArrowComponent>( TEXT( "SpawnArrow" ) );
	SpawnArrow->SetupAttachment( RootComponent );
	SpawnArrow->SetRelativeRotation( FRotator( -90 , 180 , 0 ) );

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle , this , &AEnemyManager::MakeEnemy , MakeTime , true );
	
}

void AEnemyManager::EndPlay( const EEndPlayReason::Type EEndPlayReason )
{
	Super::EndPlay( EEndPlayReason );
	if (TimerHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive( TimerHandle )) {
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 시간이 흐르다가
	CurrentTime += DeltaTime;

	// 2. 만약에 현재시간이 생성시간을 초과하면
	if (CurrentTime > MakeTime) {
		// 3. Enemy를 스폰하고 싶다.	
		MakeEnemy();
		// 4. 현재시간을 0으로 초기화하고 싶다
		CurrentTime = 0.0f;
	}
	
	 
}

void AEnemyManager::MakeEnemy()
{
	auto Enemy = GetWorld()->SpawnActor<AEnemyActor>( EnemyFactory , SpawnArrow->GetComponentTransform() );
}
