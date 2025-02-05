// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_PROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_TPS;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_LookUp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Turn;

	// 좌우 회전
	void Turn(const FInputActionValue& inputValue);

	// 상하 회전
	void LookUp(const FInputActionValue& inputValue);

	// 사용자의 좌우 입력을 받아서 이동하고 싶다
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Jump;

	void InputJump(const FInputActionValue& inputValue);

	// 이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float WalkSpeed = 600.0f;

	// 이동 방향
	FVector Direction;

	void Move(const FInputActionValue& inputValue);
	
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_Fire;

	// 총알 발사 처리함수
	void InputFire(const FInputActionValue& inputValue);



};
