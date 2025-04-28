#include "UI/HUD/RPGHUD.h"

#include "UI/WidgetControllers/AttributeMenuWidgetController.h"
#include "UI/WidgetControllers/OverlayWidgetController.h"
#include "UI/WidgetControllers/SpellMenuWidgetController.h"
#include "UI/Widgets/RPGUserWidget.h"

UOverlayWidgetController* ARPGHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!IsValid(OverlayWidgetController))
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbackDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ARPGHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!IsValid(AttributeMenuWidgetController))
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbackDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* ARPGHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!IsValid(SpellMenuWidgetController))
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbackDependencies();
	}
	return SpellMenuWidgetController;
}

void ARPGHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, fill it out in BP_RPGHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill it out in BP_RPGHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<URPGUserWidget>(Widget);
	
	FWidgetControllerParams WCParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WCParams);

	OverlayWidget->SetWidgetController(WidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
