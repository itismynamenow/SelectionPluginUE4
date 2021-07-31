
#include "SelectionHUDComponent.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SelectionComponent.h"
#include "SelectionManagerComponent.h"

USelectionHUDComponent::USelectionHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USelectionHUDComponent::ReceiveDrawHUD()
{
	if (!IsSelectionActive())
		return;

	m_EndPoint = GetCursorPosition();
	DrawSelectionRect();
	SelectActorsInRect();
}

void USelectionHUDComponent::SelectActorsInRect()
{
	auto SelectionManagerComponent = GetSelectionManagerComponent();
	if (IsValid(SelectionManagerComponent))
		SelectionManagerComponent->SelectActorsInRect(m_StartPoint, m_EndPoint);
	else
		UE_LOG(LogTemp, Error, TEXT("SelectionManagerComponent is not valid"));
}

void USelectionHUDComponent::DrawSelectionRect()
{
	auto HUD = Cast<AHUD>(GetOwner());
	if (!IsValid(HUD))
	{
		UE_LOG(LogTemp, Error, TEXT("HUD is not valid"));
		return;
	}

	const auto RectOrigin = FVector2D(
		std::min(m_EndPoint.X, m_StartPoint.X),
		std::min(m_EndPoint.Y, m_StartPoint.Y)
	);

	const auto RectSize = FVector2D(
		std::abs(m_EndPoint.X - m_StartPoint.X),
		std::abs(m_EndPoint.Y - m_StartPoint.Y)
	);

	const auto Corner1 = RectOrigin;
	const auto Corner2 = RectOrigin + FVector2D(RectSize.X, 0);
	const auto Corner3 = RectOrigin + FVector2D(RectSize.X, RectSize.Y);
	const auto Corner4 = RectOrigin + FVector2D(0, RectSize.Y);

	HUD->DrawLine(Corner1.X, Corner1.Y, Corner2.X, Corner2.Y, m_Color, m_LineWidth);
	HUD->DrawLine(Corner2.X, Corner2.Y, Corner3.X, Corner3.Y, m_Color, m_LineWidth);
	HUD->DrawLine(Corner3.X, Corner3.Y, Corner4.X, Corner4.Y, m_Color, m_LineWidth);
	HUD->DrawLine(Corner4.X, Corner4.Y, Corner1.X, Corner1.Y, m_Color, m_LineWidth);
}

void USelectionHUDComponent::BeginPlay()
{
	Super::BeginPlay();	
}

bool USelectionHUDComponent::IsSelectionActive() const
{
	auto SelectionManagerComponent = GetSelectionManagerComponent();
	if (!IsValid(SelectionManagerComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("SelectionManagerComponent is not valid"));
		return false;
	}

	return SelectionManagerComponent->IsRectSelectionActive();
}

void USelectionHUDComponent::UpdateStartPoint()
{
	m_StartPoint = GetCursorPosition();
}

FVector2D USelectionHUDComponent::GetCursorPosition() const
{
	auto PlayerController = GetPlayerController();
	if (!IsValid(PlayerController))
		return FVector2D();

	FVector2D Position;
	PlayerController->GetMousePosition(Position.X, Position.Y);
	return Position;
}

class APlayerController* USelectionHUDComponent::GetPlayerController() const
{
	auto World = GetWorld();
	if(!IsValid(World))
		return nullptr;

	return World->GetFirstPlayerController();
}

USelectionManagerComponent* USelectionHUDComponent::GetSelectionManagerComponent() const
{
	auto GameMode = UGameplayStatics::GetGameMode(GetWorld());
	if (!IsValid(GameMode))
		return nullptr;

	return Cast<USelectionManagerComponent>(GameMode->GetComponentByClass(USelectionManagerComponent::StaticClass()));
}

