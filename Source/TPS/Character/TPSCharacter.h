// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FuncLibrary/Types.h"
#include "Weapons/WeaponDefault.h"
#include "Character/TPSInventoryComponent.h"
#include "Character/TPSCharacterHealthComponent.h"
#include "Interface/TPS_IGameActor.h"
#include "StateEffects/TPS_StateEffect.h"

//#include "Components/WidgetComponent.h"

#include "TPSCharacter.generated.h"

//USTRUCT(BlueprintType)
//struct FCharacterSpeedInfo
//{
//	GENERATED_BODY()
//
//
//};
UCLASS(Blueprintable)
class ATPSCharacter : public ACharacter, public ITPS_IGameActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	

public:
	ATPSCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UTPSInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	class UTPSCharacterHealthComponent* CharHealthComponent;
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	//Cursor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);


	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementSpeedInfo;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintRunEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<UAnimMontage*> DeadsAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UTPS_StateEffect> AbilityEffect;
	
	
	//Weapon
	AWeaponDefault* CurrentWeapon = nullptr;

	UDecalComponent* CurrentCursor = nullptr;

	//Effect
	TArray<UTPS_StateEffect*> Effects;

	//Inputs
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	float AxisX = 0.0f;
	float AxisY = 0.0f;

	// Tick Func
	UFUNCTION()
	void MovementTick(float DeltaTime);

	//Func
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();

	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable) //VisualOnly
	void RemoveCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);

	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);

	UFUNCTION(BlueprintCallable)
	UDecalComponent* GetCursorToWorld();

	//Inventory Func
	
	void TrySwicthNextWeapon();
	void TrySwitchPreviosWeapon();
	//ability func
	void TryAbilityEnabled();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 CurrentIndexWeapon = 0;

	//Interface
	EPhysicalSurface GetSurfuceType() override;
	TArray<UTPS_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UTPS_StateEffect* RemoveEffect)override;
	void AddEffect(UTPS_StateEffect* newEffect)override;
	//End Interface

	UFUNCTION()
	void CharDead();
	void EnableRagdoll();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};

