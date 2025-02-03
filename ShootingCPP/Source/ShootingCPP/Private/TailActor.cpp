// Fill out your copyright notice in the Description page of Project Settings.


#include "TailActor.h"
#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATailActor::ATailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MeshComp" ) );
}

// Called when the game starts or when spawned
void ATailActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Target == nullptr || Target->IsValidLowLevel()) {
		Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	}
	//Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//Target = UGameplayStatics::GetPlayerPawn(GetWorld(), APlayerPawn::StaticClass());

	//Target = GetPlayerPawn();
}


// Called every frame
void ATailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector dir = (Target->GetActorLocation() - this->GetActorLocation());

	// P = P0 + vt
	SetActorLocation( GetActorLocation() + dir.GetSafeNormal() * Speed * DeltaTime );

	float d = Target->GetDistanceTo( this );
	float dist = FVector::Dist( Target->GetActorLocation() , this->GetActorLocation() );
	float distance = dir.Size();
}

