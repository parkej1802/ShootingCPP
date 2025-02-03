// Fill out your copyright notice in the Description page of Project Settings.


#include "SubFlightActor.h"

// Sets default values
ASubFlightActor::ASubFlightActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComp" ) );
	SetRootComponent( BoxComp );

	BoxComp->SetBoxExtent( FVector( 25 ) );

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( RootComponent );
	BoxComp->SetRelativeScale3D( FVector( 0.5f ) );

	FirePositionComp = CreateDefaultSubobject<UArrowComponent>( TEXT( "FirePositionComp" ) );
	FirePositionComp->SetupAttachment( RootComponent );
	FirePositionComp->SetRelativeLocationAndRotation( FVector( 0.0f , 0.0f , 50.0f ) , FRotator( 90.0f , 0.0f , 0.0f ) );

	// 오버랩 이벤트를 끈다

	BoxComp->SetGenerateOverlapEvents( false );

	// 모든 채널을 충돌 응답 없음으로 설정한다
	BoxComp->SetCollisionResponseToAllChannels( ECR_Ignore );
}

// Called when the game starts or when spawned
void ASubFlightActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASubFlightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{

		if (Target != nullptr) {
			// 내가 도착할 ㅇㅟ치ㄲㅏ지 바ㅇ향 벡터를 만든다.
			FVector dir = Target->GetComponentLocation() - GetActorLocation();

			// 거리를 체크하자
			if (dir.Length() > 5.0f)
			{

				// P = P0 + vt
				// Target을 향해서 직선 이동
				FVector p0 = GetActorLocation();
				FVector vt = dir.GetSafeNormal() * MoveToTargetSpeed * DeltaTime;
				SetActorLocation( p0 + vt );
			}
			else {
				FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
				this->AttachToComponent(Target, rules);
				this->SetActorLocation( Target->GetComponentLocation() );

				FRotator NewRotation = MeshComp->GetRelativeRotation();


				// target을 nullptr
				Target = nullptr;

				//총알이 자동으로 발사 되도록 한다
				bStartFire = true;
			}
		}
	}

	{

		// 총알 발사가 시작되면
		if (bStartFire == true) {
			// 1. 시간이 흐르다가
			CurrentTime += DeltaTime;
			// 2. 현재시간이 발사시간이 되면
			if (CurrentTime >= FireTime) {
				// 3. 총알을 만들고 싶다.
				MakeBullet();
				// 4. 현재시간을 0으로 초기화 하고 싶다.
				CurrentTime = 0.0f;
			}
		}
	}
}

void ASubFlightActor::MakeBullet()
{
	FTransform FirePos = FirePositionComp->GetComponentTransform();
	GetWorld()->SpawnActor<ABulletActor>( BulletFactory , FirePos );
}

