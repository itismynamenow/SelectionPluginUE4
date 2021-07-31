#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionManagerComponent.generated.h"

class USelectionHUDComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SELECTIONPLUGIN_API USelectionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USelectionManagerComponent();

	UFUNCTION(BlueprintCallable)
	void RegisterComponent(USelectionComponent* Component);
	UFUNCTION(BlueprintCallable)
	void UnregisterComponent(USelectionComponent* Component);
	UFUNCTION(BlueprintCallable)
	TArray<USelectionComponent*> GetAllSelectionComponents() const;
	UFUNCTION(BlueprintCallable)
	TArray<USelectionComponent*> GetSelectedSelectionComponents() const;
	UFUNCTION(BlueprintCallable)
	void UnselectAll();
	UFUNCTION(BlueprintCallable)
	bool IsRectSelectionActive() const;
	UFUNCTION(BlueprintCallable)
	void SelectActorsInRect(const FVector2D& FirstPoint, const FVector2D& SecondPoint);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey m_MouseKey = EKeys::LeftMouseButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey m_ModifierKey = EKeys::LeftAlt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey m_AdditiveSelectionKey = EKeys::LeftShift;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> m_FilterClass = AActor::StaticClass();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_IncludeNonCollidingComponents = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_ActorMustBeFullyEnclosed = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool m_BlockControllerRotationWhenModifierIsPressed = false;

protected:
	virtual void BeginPlay() override;

private:
	bool IsKeyDown(const FKey& Key) const;
	APlayerController* GetPlayerController() const;
	USelectionHUDComponent* GetSelectionHUDComponent() const;
	void SetPlayerControllerIgnoreLookInput(bool Ignore);
	void UnselectAllIfNeeded();


	void OnMousePress();
	void OnModifierPress();
	void OnModifierRelease();
	void OnSingleSelection();
	void OnRectSelectionStart();

private:
	TArray<USelectionComponent*> m_SelectionComponents;

};
