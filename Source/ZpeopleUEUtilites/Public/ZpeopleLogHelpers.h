// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZpeopleLogHelpers.generated.h"

UENUM(BlueprintType)
enum class EZpeopleLogVerbosity : uint8
{
	/** Not used */
	NoLogging		= 0,

	/** Always prints a fatal error to console (and log file) and crashes (even if logging is disabled) */
	Fatal,

	/** 
		* Prints an error to console (and log file). 
		* Commandlets and the editor collect and report errors. Error messages result in commandlet failure.
		*/
	Error,

	/** 
		* Prints a warning to console (and log file).
		* Commandlets and the editor collect and report warnings. Warnings can be treated as an error.
		*/
	Warning,

	/** Prints a message to console (and log file) */
	Display,

	/** Prints a message to a log file (does not print to console) */
	Log,

	/** 
		* Prints a verbose message to a log file (if Verbose logging is enabled for the given category, 
		* usually used for detailed logging) 
		*/
	Verbose,

	/** 
		* Prints a verbose message to a log file (if VeryVerbose logging is enabled, 
		* usually used for detailed logging that would otherwise spam output) 
		*/
	VeryVerbose
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to logging
*/
UCLASS()
class ZPEOPLEUEUTILITES_API UZpeopleLogHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    // Prints a log string to the console and screen depending on request
    UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext, Keywords = "log print", AdvancedDisplay = "4", DevelopmentOnly), Category = "Zpeople|LogHelpers")
	static void PrintLogString(UObject* WorldContextObject, const FString& InString, const FString& CategoryName, EZpeopleLogVerbosity verbosity = EZpeopleLogVerbosity::Log, 
                               bool PrintToScreen = false, bool PrintToLog = true, FLinearColor TextColor = FLinearColor(0.0, 0.66, 1.0), const float Duration = 0.0f);
};
