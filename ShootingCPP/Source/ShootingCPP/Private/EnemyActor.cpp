// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "PlayerPawn.h"
#include "ShootingGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>( TEXT( "BoxComp" ) );
	SetRootComponent( BoxComp );

	BoxComp->SetBoxExtent( FVector( 50 , 50 , 50 ) );
\
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
	MeshComp->SetupAttachment( RootComponent );

	/*ConstructorHelpers::FObjectFinder<UStaticMesh> tmpMesh(
		TEXT( "/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'" ) );
	if (tmpMesh.Succeeded()) {
		MeshComp->SetStaticMesh( tmpMesh.Object );
	}*/

	BoxComp->SetCollisionProfileName(FName("Enemy"));

	BoxComp->OnComponentBeginOverlap.AddDynamic( this , &AEnemyActor::OnEnemyOverlap );


}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	int32 randValue = FMath::RandRange( 1 , 100 );
	if (randValue <= RandomRate) {

		auto* player = GetWorld()->GetFirstPlayerController()->GetPawn();

		if (player->IsValidLowLevel()) {
			Direction = (player->GetActorLocation() - this->GetActorLocation()).GetSafeNormal();
		}
		
		else {
			Direction = GetActorForwardVector();
		}
	}
	else {
		Direction = GetActorForwardVector();
	}
	
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	{
		SetActorLocation( GetActorLocation() + Direction  * Speed * DeltaTime );
	}


	
}

void AEnemyActor::OnEnemyOverlap( UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweeptResult )
{
	APlayerPawn* player = Cast<APlayerPawn>( OtherActor );

	if (player != nullptr)
	{
		// 플레이어의 체력을 1 감소한다
		player->SetDamage( 1 );

		// 플레이어가 사망했다면
		
		if (player->HP <= 0) {
			// 충돌한 액터를 제거한다
			OtherActor->Destroy();
			AShootingGameMode* currentGameMode = Cast<AShootingGameMode>( GetWorld()->GetAuthGameMode() );

			if (currentGameMode != nullptr) {
				

				// GameOver UI를 보여준다
				currentGameMode->ShowGameOver( true );
			}
		}
		
		this->Destroy();
		UGameplayStatics::PlaySound2D( GetWorld() , ExplosionSound );
		UGameplayStatics::SpawnEmitterAtLocation( GetWorld() , ExplosionVFX , GetActorTransform() );
	}
}

