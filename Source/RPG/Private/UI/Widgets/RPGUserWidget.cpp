#include "UI/Widgets/RPGUserWidget.h"

void URPGUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
