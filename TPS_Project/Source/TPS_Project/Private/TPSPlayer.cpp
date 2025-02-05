// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded())
	{
		// 로드한 메스를 넣어주고 싶다
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		
		// 위치값과 회전값을 반영해주고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	// TPS카메라를 붙이고 싶다
	// SpringArm컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f, 60.0f, 80.0f));
	springArmComp->TargetArmLength = 300.0f;
	springArmComp->bUsePawnControlRotation = true;

	// Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 총 스켈레탈 메시 컴포넌트 등록
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));

	// 부모 컴포너ㄴ트를 Mesh 컴포넌트로 연결
	GunMeshComp->SetupAttachment(GetMesh());

	// 스켈렡탈 메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	// 데이터 로드가 성공했다면 
	if ( TempGunMesh.Succeeded( ) )
	{
		// 스켈레탈 메시 데이터 할당
		GunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		GunMeshComp->SetRelativeLocation(FVector(0.0f, 60.0f, 120.0f));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);

	if ( pc )
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if ( subsystem )
		{
			subsystem->AddMappingContext(IMC_TPS, 0);
		}
	}
	JumpMaxCount = 100;

	FTimerHandle deathTimer;
	//GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
	FTimerDelegate::CreateLambda([this]( )->void
		{
			Destroy();
		}),
	
	2.0f, false);
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// P(결과 위치) = P0 + vt
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);

	/*FVector P0 = GetActorLocation();
	FVector vt = Direction * WalkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	*/

	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if ( PlayerInput )
	{
		PlayerInput->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		PlayerInput->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		PlayerInput->BindAction(IA_Jump, ETriggerEvent::Started, this, &ATPSPlayer::InputJump);
		PlayerInput->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATPSPlayer::InputFire);
	}
}

void ATPSPlayer::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>( );
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputJump(const FInputActionValue& inputValue)
{
	Jump();
}

void ATPSPlayer::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();

	// 상하 ㅇㅣㅂ력 처리
	Direction.X = value.X;

	// 좌우 입력 처리
	Direction.Y = value.Y;

}

void ATPSPlayer::InputFire(const FInputActionValue& inputValue)
{
	// 총알 발사 처리
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform firePosition = GunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition, spawnParams);
}

