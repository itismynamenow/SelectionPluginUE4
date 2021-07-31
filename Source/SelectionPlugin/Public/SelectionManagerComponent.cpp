#include "SelectionManagerComponent.h"

#include "SelectionComponent.h"
#include "SelectionHUDComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"

USelectionManagerComponent::USelectionManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USelectionManagerComponent::RegisterComponent(USelectionComponent* Component)
{
	if (!IsValid(Component))
		return;

	m_Components.Add(Component);
}

void USelectionManagerComponent::UnregisterComponent(USelectionComponent* Component)
{
	m_Components.Remove(Component);
}

TArray<USelectionComponent*> USelectionManagerComponent::GetAllSelectionComponents() const
{
	return m_Components;
}

TArray<AActor*> USelectionManagerComponent::GetAllActorsWithSelectionComponents() const
{
	TArray<AActor*> Result;
	for (auto Component : m_Components)
	{
		if (!IsValid(Component))
			continue;

		Result.Add(Component->GetOwner());
	}

	return Result;
}

TArray<USelectionComponent*> USelectionManagerComponent::GetSelectedSelectionComponents() const
{
	TArray<USelectionComponent*> Result;
	for (auto Component : m_Components)
	{
		if (!IsValid(Component))
			continue;

		if (Component->IsSelected())
			Result.Add(Component);
	}

	return Result;
}

TArray<AActor*> USelectionManagerComponent::GetAllSelectedActorsWithSelectionComponents() const
{
	TArray<AActor*> Result;
	for (auto Component : m_Components)
	{
		if (!IsValid(Component))
			continue;

		if (Component->IsSelected())
			Result.Add(Component->GetOwner());
	}

	return Result;
}

void USelectionManagerComponent::UnselectAll()
{
	for (auto Component : m_Components)
	{
		if(!IsValid(Component))
			continue;

		Component->SetSelected(false);
	}
}

bool USelectionManagerComponent::IsRectSelectionActive() const
{
	return IsKeyDown(m_MouseKey) && IsKeyDown(m_ModifierKey);
}

void USelectionManagerComponent::SelectActorsInRect(const FVector2D& FirstPoint, const FVector2D& SecondPoint)
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return;

	auto HUD = PlayerController->GetHUD();
	if (!IsValid(HUD))
		return;

	UnselectAllIfNeeded();

	TArray<AActor*> ActorsInSelection;
	HUD->GetActorsInSelectionRectangle(
		m_FilterClass, 
		FirstPoint, 
		SecondPoint, 
		ActorsInSelection, 
		m_IncludeNonCollidingComponents, 
		m_ActorMustBeFullyEnclosed
	);

	for (auto Actor : ActorsInSelection)
	{
		if(!IsValid(Actor))
			continue;

		auto SelectionComponent = Cast<USelectionComponent>(Actor->GetComponentByClass(USelectionComponent::StaticClass()));
		if (!IsValid(SelectionComponent))
			continue;

		SelectionComponent->SetSelected(true);
	}
}

void USelectionManagerComponent::UnselectAllIfNeeded()
{
	if (!IsKeyDown(m_AdditiveSelectionKey))
		UnselectAll();
}

void USelectionManagerComponent::OnMousePress()
{
	UnselectAllIfNeeded();

	if (IsKeyDown(m_ModifierKey))
		OnRectSelectionStart();
	else
		OnSingleSelection();
}

void USelectionManagerComponent::OnModifierPress()
{
	if (IsKeyDown(m_MouseKey))
		OnRectSelectionStart();

	if(m_BlockControllerRotationWhenModifierIsPressed)
		SetPlayerControllerIgnoreLookInput(true);
}

void USelectionManagerComponent::OnModifierRelease()
{
	if (m_BlockControllerRotationWhenModifierIsPressed)
		SetPlayerControllerIgnoreLookInput(false);
}

void USelectionManagerComponent::OnSingleSelection()
{
	auto World = GetWorld();
	if (!IsValid(World))
		return;

	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return;

	FVector MousePosition;
	FVector MouseDirection;

	PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection);

	const auto TraceStart = MousePosition;
	const auto TraceEnd = MousePosition + MouseDirection * m_LineTraceDistance;

	FHitResult HitResult;
	if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, m_CollisionChannel))
	{
		if (auto HitActor = HitResult.Actor.Get())
		{
			if (!IsValid(HitActor))
			{
				if(m_LogHitResult)
					UE_LOG(LogTemp, Log, TEXT("Hit result: Hit not valid actor"));
				return;
			}

			auto SelectionComponent = Cast<USelectionComponent>(HitActor->GetComponentByClass(USelectionComponent::StaticClass()));
			if (!IsValid(SelectionComponent))
			{
				if(m_LogHitResult)
					UE_LOG(LogTemp, Log, TEXT("Hit result: Hit object '%s' without selection component"), *HitActor->GetName());
				return;
			}

			if (m_LogHitResult)
				UE_LOG(LogTemp, Log, TEXT("Hit result: Hit object '%s' with selection component"), *HitActor->GetName());
			SelectionComponent->ToggleSelected();
		}
	}
}

void USelectionManagerComponent::OnRectSelectionStart()
{
	auto SelectionHUDComponent = GetSelectionHUDComponent();
	if (!IsValid(SelectionHUDComponent))
		UE_LOG(LogTemp, Error, TEXT("SelectionHUDComponent is not valid"));

	SelectionHUDComponent->UpdateStartPoint();
}

void USelectionManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return;

	auto InputComponent = PlayerController->InputComponent;
	if (!IsValid(InputComponent))
		return;

	InputComponent->BindKey(m_MouseKey, IE_Pressed, this, &USelectionManagerComponent::OnMousePress);
	InputComponent->BindKey(m_ModifierKey, IE_Pressed, this, &USelectionManagerComponent::OnModifierPress);
	InputComponent->BindKey(m_ModifierKey, IE_Released, this, &USelectionManagerComponent::OnModifierRelease);
}

bool USelectionManagerComponent::IsKeyDown(const FKey& Key) const
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return false;

	return PlayerController->IsInputKeyDown(Key);
}

APlayerController* USelectionManagerComponent::GetPlayerController() const
{
	auto World = GetWorld();
	if (!IsValid(World))
		return nullptr;

	return World->GetFirstPlayerController();
}

USelectionHUDComponent* USelectionManagerComponent::GetSelectionHUDComponent() const
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return nullptr;

	auto HUD = PlayerController->GetHUD();
	if (!IsValid(HUD))
		return nullptr;

	return Cast<USelectionHUDComponent>(HUD->GetComponentByClass(USelectionHUDComponent::StaticClass()));
}

void USelectionManagerComponent::SetPlayerControllerIgnoreLookInput(bool Ignore)
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return;

	PlayerController->SetIgnoreLookInput(Ignore);	
}

