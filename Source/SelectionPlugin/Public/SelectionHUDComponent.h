#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SelectionHUDComponent.generated.h"

class USelectionManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USelectionHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USelectionHUDComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor m_Color = FColor(0,255,0,255);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_LineWidth = 1;

	UFUNCTION(BlueprintCallable)
	void ReceiveDrawHUD();

	void SelectActorsInRect();
	void DrawSelectionRect();
	void UpdateStartPoint();

protected:
	virtual void BeginPlay() override;

private:
	bool IsSelectionActive() const;
	FVector2D GetCursorPosition() const;
	APlayerController* GetPlayerController() const;
	USelectionManagerComponent* GetSelectionManagerComponent() const;

private:
	FVector2D m_StartPoint;
	FVector2D m_EndPoint;
		
};
