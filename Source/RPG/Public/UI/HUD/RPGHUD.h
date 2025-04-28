#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPGHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class URPGUserWidget;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class RPG_API ARPGHUD : public AHUD
{
	GENERATED_BODY()
public:

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URPGUserWidget> OverlayWidgetClass;
	UPROPERTY()
	TObjectPtr<URPGUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
};
