// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()

public :
	void AddScore( int32 point );
	void SetHP( float Cur , float Max );

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMainWidget> mainWidget;

	//저장하기/ 불러오기
	// HighScore갱신될때 저장하고 싶다.
	// 게임을 시작될때 HighScore 값을 불러오고 싶다.
	// SlotName, UserIndex



	void SaveGameData();
	void LoadGameData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName = TEXT("HighScore");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserIndex = 0;


	void ShowGameOver( bool bShow );

protected:
	virtual void BeginPlay() override;


private:
	int32 currentScore = 0;
	int32 highScore = 0;

	class UMainWidget* mainUI;

	void PrintScore();
	void PrintHighScore();

	UPROPERTY(EditAnywhere)
	class USoundBase* BGM;
	
};
