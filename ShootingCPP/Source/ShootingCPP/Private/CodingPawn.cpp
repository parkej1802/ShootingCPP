// Fill out your copyright notice in the Description page of Project Settings.


#include "CodingPawn.h"

// Sets default values
ACodingPawn::ACodingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodingPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));
	
	GEngine->AddOnScreenDebugMessage(0, 10, FColor::Cyan, TEXT("Hello World"));

	int32 b = 0;

	UE_LOG(LogTemp, Warning, TEXT("%d"), Number);
	UE_LOG(LogTemp, Warning, TEXT("%.2f"), Distance);
	UE_LOG(LogTemp, Warning, TEXT("%d"), bGood);
	
	// 문자열
	// FString : 문자열을 조합해서 생성, 주로 사용하는 문자열 자료형
	// FText
	// FName
	UE_LOG(LogTemp, Warning, TEXT("%s"), *MyName);

	if (Number < 10) {

	}
	else if (Number < 20) {

	}
	else {

	}

	switch (Number)
	{
	case 1:
		break;
	case 2: 
		break;
	case 5: 
		break;
	default:
		break;
	}

	TArray<int32> ar;
	ar.Empty(10);
	ar.Add(10);
	ar.Add(11);
	ar.Add(12);

	if (false == ar.Contains(5)) {

		ar.Add(5);
	}

	ar.Num();
	ar.GetSlack();

	for (int32 i = 0; i < ar.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT(" %d "), ar[i]);
	}

	/*for each */

	for (int32 item : ar)
	{
		UE_LOG(LogTemp, Warning, TEXT(" %d "), item);
	}

	TMap < FString, int32> map;

	map.Add("Coin", 10);
	if (!map.Contains("Coin")) {
		map.Add("Coin", 20);
	}
	else {
		map["Coin"] = 20;
	}

	UE_LOG(LogTemp, Warning, TEXT("%d"), map["Coin"]);

	for (TPair<FString, int32> pair : map) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s - %d"), *(pair.Key), pair.Value);
	}	

	UE_LOG(LogTemp, Warning, TEXT("Size of : %d"), sizeof(Number));

}

// Called every frame
void ACodingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACodingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int32 ACodingPawn::MyAddCallable( int32 a , int32 b )
{
	return a + b;
}

int32 ACodingPawn::MyAddPure( int32 a , int32 b )
{
	return a + b;
}	

int32 ACodingPawn::MyAddNativeEvent_Implementation( int32 a , int32 b )
{
	return a + b;
}

