#include "RPGAssetManager.h"
#include "RPGGameplayTags.h"
#include "AbilitySystemGlobals.h"

const URPGAssetManager& URPGAssetManager::Get()
{
	check(GEngine);
	return *CastChecked<URPGAssetManager>(GEngine->AssetManager);
}

void URPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FRPGGameplayTags::Get().InitializeNativeGameplayTags();
	
	// Needs to be called for gameplay abilities to replicate data (like target data)
	UAbilitySystemGlobals::Get().InitGlobalData();
}
