#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZpeoplePaths.generated.h"
using namespace UF;
/**
获取一些相关路径的类
*/
UCLASS()
class UZpeoplePaths :public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category = "Zpeople|Paths")
	static FString GetEngineDir();

	UFUNCTION(BlueprintPure, Category = "Zpeople|Paths")
	static FString GetGameSaveDir();

	UFUNCTION(BlueprintPure, Category = "Zpeople|Paths")
	static FString GetGameContentDir();

	UFUNCTION(BlueprintPure, Category = "Zpeople|Paths")
	static FString GetGamePaksDir();
	/**
	获取截图默认保存的路径
	*/
	UFUNCTION(BlueprintPure, Category = "Zpeople|Paths")
	static FString GetBaseScreenShotPath();
};