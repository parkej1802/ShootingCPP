// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 충돌체 등록하기
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	// 2. 충돌 프로파일 설정
	CollisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius(20.0f);

	// 4. 루트로 등록하기
	SetRootComponent(CollisionComp);
	// RootComponent = CollisionComp;

	// 5. 외고ㅏㄴ 컴포넌트 등록하기
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 6. 부모 컴포넌트 지정
	MeshComp->SetupAttachment(CollisionComp);

	// 7. 충돌 비활성화
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 8. 외고나 크기 설정
	MeshComp->SetRelativeScale3D(FVector(0.5f));

	// 발사체 컴포넌트
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	
	// Movement ㅋㅓㅁ포넌트가 갱신시킬 컴포넌트 지정
	MovementComp->SetUpdatedComponent(CollisionComp);

	// 초기 속도
	MovementComp->InitialSpeed = 5000.0f;
	// ㅊㅚ대 속도
	MovementComp->MaxSpeed = 5000.0f;

	// 반동 여부
	MovementComp->bShouldBounce = true;
	// 반동ㄱ밧
	MovementComp->Bounciness = 0.3f;

	InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die( )
{
	Destroy();
}

