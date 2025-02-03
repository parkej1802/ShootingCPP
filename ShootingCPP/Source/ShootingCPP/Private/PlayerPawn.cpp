// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "BulletActor.h"
#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SubFlightActor.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// 충돌체 컴포넌트를 추가해서 Root 컴포넌트로 설정
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent( BoxComp );

	BoxComp->SetBoxExtent( FVector ( 50 , 50 , 50 ));

	// 메시 컴포넌트를 추가해서 Root 컴포넌트에 Attach
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( BoxComp );

	FirePositionComp = CreateDefaultSubobject<UArrowComponent>( TEXT( "FirePositionComp" ) );
	FirePositionComp->SetupAttachment( RootComponent );
	FirePositionComp->SetRelativeLocationAndRotation( FVector( 0.0f , 0.0f , 100.0f ) , FRotator(90.0f, 0.0f, 0.0f) );

	//오버랩 이벤트를 켠다
	BoxComp->SetGenerateOverlapEvents( true );

	//// 충돌 응답을 Query And Physics로 설정한다
	//BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//// Object Type을 1번 채널(Player)로 설정한다
	//BoxComp->SetCollisionObjectType( ECollisionChannel::ECC_GameTraceChannel3 );

	//// 모든 채널을 충돌 응답 없음으로 설정한다
	//BoxComp->SetCollisionResponseToAllChannels( ECR_Ignore );

	//// 에너미와의 충돌 이벤트 체크(Query) 한다
	//BoxComp->SetCollisionResponseToChannel( ECC_GameTraceChannel1 , ECR_Overlap );

	//// WorldStatic( DeadZone) 과 충돌 이벤트 체크를 한다.
	//BoxComp->SetCollisionResponseToChannel( ECC_WorldStatic , ECR_Block );

	BoxComp->SetCollisionProfileName( TEXT( "Player" ) );

	// 서브 L/R 위치를 루트에 붙이고 배치하고 싶다
	Sub_L_PositionComp = CreateDefaultSubobject<UArrowComponent>( TEXT( "Sub_L_PositionComp" ) );
	Sub_L_PositionComp->SetupAttachment( RootComponent );
	Sub_L_PositionComp->SetRelativeLocationAndRotation( FVector( 0.0f , -150.0f , 0.0f ) , FRotator( 90.0f , 0.0f , 0.0f ) );

	Sub_R_PositionComp = CreateDefaultSubobject<UArrowComponent>( TEXT( "Sub_R_PositionComp" ) );
	Sub_R_PositionComp->SetupAttachment( RootComponent );
	Sub_R_PositionComp->SetRelativeLocationAndRotation( FVector( 0.0f , 150.0f , 0.0f ) , FRotator( 90.0f , 0.0f , 0.0f ) );
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// 현재 체력을 최대 체력으로 설정한다.
	GM = Cast<AShootingGameMode>( GetWorld()->GetAuthGameMode());
	// GameOverUI 숨기기, 마우스 커서 초기화, 게임 일시정지 해지, 인풋모드 게임으로 전환등 초기화 설정
	GM->ShowGameOver( false );

	/*UGameplayStatics::SetGamePaused( GetWorld() , false );

	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor( false );
	pc->SetInputMode( FInputModeGameOnly() );*/

	HP = MaxHP;

	// HP Bar를 갱신해준다
	GM->SetHP( HP , MaxHP );
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 오른족으로 이동하고 싶다
	// P = P0 + v(방향 * 속도)t
	
	{
		/*FVector p0 = GetActorLocation();
		FVector velocity = GetActorRightVector() * Speed;
		SetActorLocation( p0 + velocity * DeltaTime );*/

		// 좌우방향, 상하방향
		FVector dir = FVector( 0.0f , h , v );
		FVector dir2 = dir.GetSafeNormal();

		// dir의 길이를 1로 만들고 싶다
		//dir.Normalize();
	
		// P = P0 + vt;
		FVector P0 = GetActorLocation();
		FVector velocity = dir * Speed;

		SetActorLocation( P0 + velocity * DeltaTime, true );
		//SetActorLocation( GetActorLocation() + dir.GetSafeNormal() * Speed * DeltaTime );

	}
	{
		// 1. Tick에서 bAutoFire가 true일때 
		if (bAutoFire == true)
		{
			// 2. 시간이 흐르다가
			CurrentTime += DeltaTime;
			// 3.누적시간이 발사시간 지나면
			if (CurrentTime >= FireTime) 
			{
				// 4. 총알을 만들고 싶다.
				MakeBullet();
				

				// 5. 누적시간을 0으로 초기화
				CurrentTime = 0.0f;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{ 
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( TEXT( "Horizontal" ) , this , &APlayerPawn::OnAxisHorizontal );
	PlayerInputComponent->BindAxis( TEXT( "Vertical" ) , this , &APlayerPawn::OnAxisVertical );
	PlayerInputComponent->BindAction( TEXT( "Fire" ) , IE_Pressed , this , &APlayerPawn::OnActionFire );

	PlayerInputComponent->BindAction(TEXT("AutoFire_1"), IE_Pressed, this, &APlayerPawn::onActionAutoFire_1 );
}

void APlayerPawn::OnAxisHorizontal( float value )
{
	h = value;
	UE_LOG( LogTemp , Warning , TEXT( "H : %f" ) , h );
}

void APlayerPawn::OnAxisVertical( float value )
{
	v = value;
	UE_LOG( LogTemp, Warning , TEXT( "V : %f" ) , v )
}

void APlayerPawn::onActionAutoFire_1()
{
	bAutoFire = !bAutoFire;
	CurrentTime = 0.0f;
}

void APlayerPawn::OnActionFire()
{
	// 클릭시 총알을 생성
	/*FTransform FirePos = FirePositionComp->GetComponentTransform();
	GetWorld()->SpawnActor<ABulletActor>(BulletFactory, FirePos);
	UGameplayStatics::PlaySound2D( GetWorld() , FireSound );*/
	
	MakeBullet();
}

void APlayerPawn::SetDamage( int32 damage )
{
	HP -= damage;

	GM->SetHP( HP , MaxHP );

	// Magazine pool에 총알 20개 추가하고 싶다.
	for (int32 i = 0; i < MaxBulletCount; i++) 
	{
		FActorSpawnParameters params;
		// 항상 스폰도ㅣ게 한다
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABulletActor* bullet = GetWorld()->SpawnActor<ABulletActor>( BulletFactory, params);
		bullet->SetActive( false );
		Magazine.Add( bullet );
	}
	FActorSpawnParameters params;

	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SubFlight_L = GetWorld()->SpawnActor<ASubFlightActor>( SubFlightFactory , params );
	SubFlight_L->SetActorRelativeLocation( FVector( 0.0f , 0.0f , 1000.0f ) );
	SubFlight_L->Target = Sub_L_PositionComp;
	
	SubFlight_R = GetWorld()->SpawnActor<ASubFlightActor>( SubFlightFactory , params );
	SubFlight_R->SetActorRelativeLocation( FVector( 0.0f , 0.0f , 1000.0f ) );
	SubFlight_L->Target = Sub_R_PositionComp;
}

void APlayerPawn::MakeBullet()
{
	bool FindResult = false;

	FTransform t = FirePositionComp->GetComponentTransform();

	// 탄창을 전부 검사해서 비활성화된 총알을 찾고 싶다.
	for (int i = 0; i < Magazine.Num(); i++) 
	{
		// 찾았다면 그 총알을 활성화 하고 총구 위치에 배치하고싶다
		if (Magazine[i]->MeshComp->GetVisibleFlag() == false) 
		{
			FindResult = true;

			// 활성화 시키고 총구 위치에 배치하고 싶다.
			Magazine[i]->SetActive( true );
			Magazine[i]->SetActorTransform(t);

			// 소리를 재생하고 싶다.
			UGameplayStatics::PlaySound2D( GetWorld() , FireSound );
			break;
		}
	}

	if (FindResult == false) 
	{
		// 추가로 총알을 더 만든다.

	}
}

