#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZpeopleStringUtilityes.generated.h"

/**
 *
 */
using namespace UF;

USTRUCT(BlueprintType)
struct FJsonMapStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		TMap<FString, FString> maps;


	FJsonMapStruct() {
	}
};

UCLASS()
class ZPEOPLEUEUTILITES_API UZpeopleStringUtilityes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**把String转化为Array，根据一个标志*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static TArray<FString> SplitStringToArray(FString SRC, FString Arg);

	/**
 * Is the name None?
*/
	UFUNCTION(BlueprintPure, Category = "Zpeople|Name")
		static bool IsNone(const FName name);

	/**
	 * Is the string empty? (Has no characters)
	*/
	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static bool IsEmpty(const FString& string);

	/**
	 * Sets a char by index in the string
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void SetChar(UPARAM(ref) FString& sourceString, const int32 charIndex, const FString& charIn);

	/**
	 * Fills a string with a specific character overwriting the original string. Good for initializing a string with characters to start.
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void FillString(UPARAM(ref) FString& sourceString, const int32 numChars, const FString& charIn);

	/**
	 * Sets the reserved number of characters in a string.
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void SetReserved(UPARAM(ref) FString& sourceString, const int32 reserveNumChars);

	/**
	 * Append a string to a source string
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void AppendTo(UPARAM(ref) FString& sourceString, const FString& appendage);

	/**
	 * Append many strings to a source string
	*/
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void AppendStringsTo(UPARAM(ref) FString& sourceString, const TArray<FString>& appendages);

	/**
	 * Returns the bytes representing a character in a string. Two bytes to represent a UTF-16 TCHAR.
	 * If TCHAR is 8bits on this system, byte2 will contain the character, byte1 will be zero.
	*/
	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static void CharToBytes(const FString& sourceString, const int32 charIndex, uint8& byte1, uint8& byte2);

	// Returns a string where the first character in every word is upper case. Like a header, or a title.
	UFUNCTION(BlueprintCallable, Category = "Zpeople|String")
		static void ToTitleStringInline(UPARAM(ref) FString& inOutString);

	// Returns a string where the first character in every word is upper case. Like a header, or a title.
	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static FString ToTitleString(const FString& inString);

	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static void GetJsonStringValue(FString Descriptor, FString FieldName, FString& StringValue, bool& Success);
	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static void GetJsonArrayValue(FString Descriptor, FString FieldName, TArray<FString>& OutArray, bool& Success);
	UFUNCTION(BlueprintPure, Category = "Zpeople|String")
		static void GetJsonArrayObject(FString Descriptor, FString FieldName, TArray<FJsonMapStruct>& OutArray, bool& Success);
};

