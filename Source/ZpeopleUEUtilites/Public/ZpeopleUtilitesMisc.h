#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZpeopleUtilitesMisc.generated.h"

/**
 *
 */
using namespace UF;

UCLASS()
class  UZpeopleUtilitesMisc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "ZpeopleMisc")
	static void RestartGameClient();
};
