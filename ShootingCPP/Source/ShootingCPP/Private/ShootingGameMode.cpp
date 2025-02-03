// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingSaveGame.h"

void AShootingGameMode::AddScore( int32 point )
{
	// 매개변수 pointㄹㅡㄹ 통해 ㄴㅓㅁ겨받은 점수를 현재 점수에 누적한다
	currentScore += point;
	//
	PrintScore();

	// 현재 점수가 최고 점수를 넘어가면
	// 최고 점수를 현재 점수로 설정한다

	if (currentScore > highScore) {
		highScore = currentScore;

		// 최고 점수를 저장해 놓자
		SaveGameData();

		// 최고 점수 UI를 갱신해준다.
		PrintHighScore();
	}
}

void AShootingGameMode::SetHP( float Cur , float Max )
{
	if (mainUI == nullptr) return;
	mainUI->SetHP( Cur , Max );

}

void AShootingGameMode::SaveGameData()
{
	// ShootingSaveGame 객체를 만들고
	// 객체에 HighScoreSave 가ㅄ을 HighScore로 갱신한다
	// 파일로 저장을 한다.

	UShootingSaveGame* sg = Cast<UShootingSaveGame>( UGameplayStatics::CreateSaveGameObject( UShootingSaveGame::StaticClass() ) );

	sg->HighScoreSave = highScore;

	UGameplayStatics::SaveGameToSlot(sg, SaveSlotName, UserIndex);
}

void AShootingGameMode::LoadGameData()
{
	// 파이ㄹ이 존재하는지 확인
	bool isExist = UGameplayStatics::DoesSaveGameExist( SaveSlotName , UserIndex );
	if (isExist == false) return;
	// 파일이 있으면 읽어서 ShootingSaveGame 개ㄱ체를 생ㅅㅓㅇ하고
	UShootingSaveGame* sg = Cast<UShootingSaveGame>( UGameplayStatics::LoadGameFromSlot( SaveSlotName , UserIndex ) );
	if (sg == nullptr) return;
	// highScore의 값을 파일에서 읽어온 값으로 갱신한다.
	highScore = sg->HighScoreSave;
	
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D( GetWorld() , BGM );

	LoadGameData();


	if (mainWidget != nullptr)
	{
		// mainWidget 블루프린트 파일을 메모리에 로드한다
		mainUI = CreateWidget<UMainWidget>( GetWorld(), mainWidget );

		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
			
			mainUI->SetHighScore( highScore );
		}
	}
}

void AShootingGameMode::ShowGameOver( bool bShow )
{
	
	if (mainUI == nullptr) return;

	// 게임을 일시 정지 상태로 만든다
	UGameplayStatics::SetGamePaused( GetWorld() , bShow );

	// 마우스 커서를 보이게 한다
	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor( bShow );

	if (bShow == true) {
		pc->SetInputMode( FInputModeUIOnly() );
	}
	else {
		pc->SetInputMode( FInputModeGameOnly() );
	}
	
	mainUI->ShowGameOver( bShow );

}

void AShootingGameMode::PrintScore()
{
	if (mainUI != nullptr)
	{
		//mainUI->scoreData->SetText( FText::AsNumber(currentScore) );
		mainUI->SetScore( currentScore );
	}

}

void AShootingGameMode::PrintHighScore()
{
	if (mainUI == nullptr) return;
	mainUI->SetHighScore( highScore );
}
