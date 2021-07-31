#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SelectionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SELECTIONPLUGIN_API USelectionComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	USelectionComponent();
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool IsSelected);
	UFUNCTION(BlueprintCallable)
	bool IsSelected() const;
	UFUNCTION(BlueprintCallable, CallInEditor)
	void ToggleSelected();

	void OnRegister() override;
	void BeginDestroy() override;
	void OnComponentCreated() override;
	void InitializeComponent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor m_SelectedColor = FColor(0, 255, 0, 255);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor m_NotSelectedColor = FColor(0, 255, 255, 255);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* m_Material = nullptr;

protected:
	virtual void BeginPlay() override;

private:
	void RegisterWithSelectionManager();
	void SetColor(const FColor& Color);
	void CreateMaterialInstanceIfNeeded();
	void UpdateColor();

private:
	UPROPERTY()
	UMaterialInstanceDynamic* m_MaterialInstance = nullptr;

	bool m_IsSelected = false;
};
