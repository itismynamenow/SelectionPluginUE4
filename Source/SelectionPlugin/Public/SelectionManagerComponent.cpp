#include "SelectionManagerComponent.h"

#include "SelectionComponent.h"
#include "SelectionHUDComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

USelectionManagerComponent::USelectionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USelectionManagerComponent::RegisterSelectionComponent(USelectionComponent* Component)
{
	m_SelectionComponents.Add(Component);
}

void USelectionManagerComponent::UnregisterSelectionComponent(USelectionComponent* Component)
{
	m_SelectionComponents.Remove(Component);
}

TArray<USelectionComponent*> USelectionManagerComponent::GetAllSelectionComponents() const
{
	return m_SelectionComponents;
}

TArray<USelectionComponent*> USelectionManagerComponent::GetSelectedSelectionComponents() const
{
	TArray<USelectionComponent*> Result;
	for (auto Component : m_SelectionComponents)
	{
		if (Component->IsSelected())
			Result.Add(Component);
	}

	return Result;
}

void USelectionManagerComponent::UnselectAll()
{
	for (auto Component : m_SelectionComponents)
		Component->SetSelected(false);
}

bool USelectionManagerComponent::IsRectSelectionActive() const
{
	return IsKeyDown(m_MouseKey) && IsKeyDown(m_ModifierKey);
}

void USelectionManagerComponent::SelectActorsInRect(const FVector2D& FirstPoint, const FVector2D& SecondPoint)
{
	auto PlayerController = GetPlayerController();
	if (!PlayerController || !PlayerController->GetHUD())
		return;

	TArray<AActor*> ActorsInSelection;
	PlayerController->GetHUD()->GetActorsInSelectionRectangle(m_FilterClass, FirstPoint, SecondPoint, ActorsInSelection);
	for (auto Actor : ActorsInSelection)
	{
		if (auto SelectionComponent = Cast<USelectionComponent>(Actor->GetComponentByClass(USelectionComponent::StaticClass())))
		{
			SelectionComponent->SetSelected(true);
		}
	}
}

void USelectionManagerComponent::OnMousePress()
{
	if (!IsKeyDown(m_AdditiveSelectionKey))
		UnselectAll();

	if (IsKeyDown(m_ModifierKey))
		OnRectSelectionStart();
	else
		OnSingleSelection();
}

void USelectionManagerComponent::OnModifierPress()
{
	if (IsKeyDown(m_MouseKey))
		OnRectSelectionStart();
}

void USelectionManagerComponent::OnSingleSelection()
{
	if (!GetPlayerController())
		return;
	FHitResult HitResult;
	if (GetPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
	{
		if (auto HitActor = HitResult.Actor.Get())
		{
			if (auto SelectionComponent = Cast<USelectionComponent>(HitActor->GetComponentByClass(USelectionComponent::StaticClass())))
				SelectionComponent->ToggleSelected();
		}
	}

}

void USelectionManagerComponent::OnRectSelectionStart()
{
	if (auto SelectionHUDComponent = GetSelectionHUDComponent())
		SelectionHUDComponent->UpdateStartPoint();
}

void USelectionManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	auto InputComponent = GetPlayerController()->InputComponent;
	if (InputComponent)
	{
		InputComponent->BindKey(m_MouseKey, IE_Pressed, this, &USelectionManagerComponent::OnMousePress);
		InputComponent->BindKey(m_ModifierKey, IE_Pressed, this, &USelectionManagerComponent::OnModifierPress);
	}	
}

bool USelectionManagerComponent::IsKeyDown(const FKey& Key) const
{
	if (auto PlayerController = GetPlayerController())
		return PlayerController->IsInputKeyDown(Key);
	return false;
}

APlayerController* USelectionManagerComponent::GetPlayerController() const
{
	return GetWorld()->GetFirstPlayerController();
}

USelectionHUDComponent* USelectionManagerComponent::GetSelectionHUDComponent() const
{
	auto PlayerController = GetPlayerController();
	if(!PlayerController || !PlayerController->GetHUD())
		return nullptr;

	return Cast<USelectionHUDComponent>(PlayerController->GetHUD()->GetComponentByClass(USelectionHUDComponent::StaticClass()));
}

