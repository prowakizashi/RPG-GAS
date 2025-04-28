#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetControllers/WidgetControllerBase.h"
#include "OverlayWidgetController.generated.h"

class URPGUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URPGUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, const FUIWidgetRow&, Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPG_API UOverlayWidgetController : public UWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	
	virtual void BindCallbackDependencies() const override;
	virtual void BroadcastInitialValues() const override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FAttributeChangedSignature OnXpPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FPlayerStatChangedSignature OnPlayerLevelChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

protected:
	
	void OnXpChanged(int32 NewXp) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	const T* GetDatatableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;
};

template <typename T>
const T* UOverlayWidgetController::GetDatatableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
{
	return DataTable->FindRow<T>(Tag.GetTagName(), FString("GetDatatableRowByTag"));
}