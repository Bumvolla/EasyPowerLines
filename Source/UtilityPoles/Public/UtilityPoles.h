/*
 * Copyright (c) 2025 Sora Mas
 * All rights reserved.
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Declare the log category
DECLARE_LOG_CATEGORY_EXTERN(LogUtilityPoles, Log, All);

class FUtilityPolesModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};