#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionManagerComponent.generated.h"

class USelectionHUDComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USelectionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USelectionManagerComponent();

	UFUNCTION(BlueprintCallable)
	void RegisterSelectionComponent(USelectionComponent* Component);
	UFUNCTION(BlueprintCallable)
	void UnregisterSelectionComponent(USelectionComponent* Component);
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

protected:
	virtual void BeginPlay() override;

private:
	bool IsKeyDown(const FKey& Key) const;
	APlayerController* GetPlayerController() const;
	USelectionHUDComponent* GetSelectionHUDComponent() const;


	void OnMousePress();
	void OnModifierPress();
	void OnSingleSelection();
	void OnRectSelectionStart();

private:
	TArray<USelectionComponent*> m_SelectionComponents;

};
