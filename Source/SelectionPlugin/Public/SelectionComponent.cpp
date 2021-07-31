#include "SelectionComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"

#include "SelectionManagerComponent.h"


USelectionComponent::USelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetSimulatePhysics(false);
	SetCastShadow(false);
}

void USelectionComponent::BeginPlay()
{
	Super::BeginPlay();	
	RegisterWithSelectionManager();
	UpdateColor();
}

void USelectionComponent::RegisterWithSelectionManager()
{
	auto pGameMode = UGameplayStatics::GetGameMode(GetWorld());
	{

		auto SelectionManager = Cast<USelectionManagerComponent>(pGameMode->GetComponentByClass(USelectionManagerComponent::StaticClass()));
		if (!SelectionManager)
		{
			UE_LOG(LogTemp, Error, TEXT("SelectionManagerComponent is null"));
			return;
		}

		SelectionManager->RegisterSelectionComponent(this);
	}
}

void USelectionComponent::SetColor(const FColor& Color)
{
	CreateMaterialInstanceIfNeeded();
	if(!m_MaterialInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("MaterialInstance is null"));
		return;
	}

	m_MaterialInstance->SetVectorParameterValue("Color", Color);
}

void USelectionComponent::CreateMaterialInstanceIfNeeded()
{
	if (!m_MaterialInstance || m_MaterialInstance != GetMaterial(0))
	{
		if (!m_Material)
			return;

		m_MaterialInstance = UMaterialInstanceDynamic::Create(m_Material, this);
		SetMaterial(0, m_MaterialInstance);
	}
}

void USelectionComponent::UpdateColor()
{
	const auto Color = m_IsSelected ? m_SelectedColor : m_NotSelectedColor;
	SetColor(Color);
}

void USelectionComponent::SetSelected(bool IsSelected)
{
	m_IsSelected = IsSelected;
	UpdateColor();
}

bool USelectionComponent::IsSelected() const
{
	return m_IsSelected;
}

void USelectionComponent::ToggleSelected()
{
	SetSelected(!IsSelected());
}

void USelectionComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USelectionComponent::OnRegister()
{
	Super::OnRegister();
	UpdateColor();
}

void USelectionComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	UpdateColor();
}
