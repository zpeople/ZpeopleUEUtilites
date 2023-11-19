// Fill out your copyright notice in the Description page of Project Settings.

#include "ZpeopleStringUtilityes.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"

TArray<FString> UZpeopleStringUtilityes::SplitStringToArray(FString SRC, FString Arg)
{
	TArray<FString>result;
	if (SRC.Contains(Arg, ESearchCase::CaseSensitive))
	{
		FString leftStr;
		FString rightStr;
		SRC.Split(Arg, &leftStr, &rightStr, ESearchCase::CaseSensitive);
		if (leftStr.Len() > 0)
		{
			result.Add(leftStr);
		}
		result.Append(SplitStringToArray(rightStr, Arg));
		return result;
	}
	else {
		if (SRC.Len() > 0)
		{
			result.Add(SRC);
		}
		return result;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool UZpeopleStringUtilityes::IsNone(const FName name)
{
	return name.IsNone();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool UZpeopleStringUtilityes::IsEmpty(const FString& string)
{
	return string.IsEmpty();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::SetChar(FString& sourceString, const int32 charIndex, const FString& charIn)
{
	if (sourceString.Len() <= charIndex || charIn.Len() == 0 || charIndex < 0)
	{
		return;
	}

	TArray<TCHAR>& chars = sourceString.GetCharArray();
	chars[charIndex] = **charIn;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::FillString(FString& sourceString, const int32 numChars, const FString& charIn)
{
	if (charIn.Len() == 0 || numChars <= 0)
	{
		return;
	}

	const TCHAR c = **charIn;
	TArray<TCHAR>& chars = sourceString.GetCharArray();
	chars.SetNumUninitialized(numChars + 1);
	for (int32 cIndex = 0; cIndex < numChars; ++cIndex)
	{
		chars[cIndex] = c;
	}
	chars[numChars] = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::SetReserved(FString& sourceString, const int32 reserveNumChars)
{
	if (reserveNumChars > 0)
	{
		sourceString.Reserve(reserveNumChars);
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::AppendTo(FString& sourceString, const FString& appendage)
{
	sourceString.Reserve(sourceString.Len() + appendage.Len());
	sourceString += appendage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::AppendStringsTo(FString& sourceString, const TArray<FString>& appendages)
{
	int32 numNewChars = 0;
	for (int32 appendageIndex = 0; appendageIndex < appendages.Num(); ++appendageIndex)
	{
		numNewChars += appendages[appendageIndex].Len();
	}

	sourceString.Reserve(sourceString.Len() + numNewChars);

	for (int32 appendageIndex = 0; appendageIndex < appendages.Num(); ++appendageIndex)
	{
		sourceString += appendages[appendageIndex];
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::CharToBytes(const FString& sourceString, const int32 charIndex, uint8& byte1, uint8& byte2)
{
	if (sourceString.Len() <= charIndex || charIndex < 0)
	{
		byte1 = 0;
		byte2 = 0;
		return;
	}

	const TArray<TCHAR>& chars = sourceString.GetCharArray();
	const int32 cSize = sizeof(TCHAR);

	if (cSize > 1)
	{
#if PLATFORM_LITTLE_ENDIAN
		byte1 = ((uint8*)(&chars[charIndex]))[1];
		byte2 = ((uint8*)(&chars[charIndex]))[0];
#else
		byte1 = ((uint8*)(&chars[charIndex]))[0];
		byte2 = ((uint8*)(&chars[charIndex]))[1];
#endif
	}
	else
	{
		byte1 = 0;
		byte2 = ((uint8*)(&chars[charIndex]))[0];
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void UZpeopleStringUtilityes::ToTitleStringInline(FString& inOutString)
{
	bool makeUpper = true;
	const int32 stringLength = inOutString.Len();
	TArray<TCHAR>& rawData = inOutString.GetCharArray();
	for (int32 i = 0; i < stringLength; ++i)
	{
		if (FChar::IsWhitespace(rawData[i]))
		{
			makeUpper = true;
			continue;
		}

		if (makeUpper)
		{
			rawData[i] = FChar::ToUpper(rawData[i]);
			makeUpper = false;
		}
		else
		{
			rawData[i] = FChar::ToLower(rawData[i]);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FString UZpeopleStringUtilityes::ToTitleString(const FString& inString)
{
	FString titleStrOut = inString;
	ToTitleStringInline(titleStrOut);
	return titleStrOut;
}

void UZpeopleStringUtilityes::GetJsonStringValue(FString Descriptor, FString FieldName, FString & StringValue, bool & Success)
{
	StringValue = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Descriptor);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		const TSharedPtr<FJsonObject>* JsonObjectPtr = &JsonObject;

		if (FieldName.Contains(TEXT(".")))
		{
			TArray<FString> FieldComponents;
			FieldName.ParseIntoArray(FieldComponents, TEXT("."));
			FieldName = FieldComponents.Pop();

			for (const FString& FieldComponent : FieldComponents)
			{
				if (!(*JsonObjectPtr)->TryGetObjectField(FieldComponent, JsonObjectPtr))
				{
					Success = false;
					return;
				}
			}
		}

		Success = (*JsonObjectPtr)->TryGetStringField(FieldName, StringValue);

	}
	else
	{

		Success = false;
	}
}
void UZpeopleStringUtilityes::GetJsonArrayValue(FString Descriptor, FString FieldName, TArray<FString>& OutArray, bool & Success)
{

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Descriptor);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		const TSharedPtr<FJsonObject>* JsonObjectPtr = &JsonObject;

		if (FieldName.Contains(TEXT(".")))
		{
			TArray<FString> FieldComponents;
			FieldName.ParseIntoArray(FieldComponents, TEXT("."));
			FieldName = FieldComponents.Pop();

			for (const FString& FieldComponent : FieldComponents)
			{
				if (!(*JsonObjectPtr)->TryGetObjectField(FieldComponent, JsonObjectPtr))
				{
					Success = false;
					return;
				}
			}
		}

		Success = (*JsonObjectPtr)->TryGetStringArrayField(FieldName, OutArray);

	}
	else
	{

		Success = false;
	}
}

void UZpeopleStringUtilityes::GetJsonArrayObject(FString Descriptor, FString FieldName, TArray<FJsonMapStruct>& OutArray, bool & Success)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Descriptor);
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		const TSharedPtr<FJsonObject>* JsonObjectPtr = &JsonObject;

		if (FieldName.Contains(TEXT(".")))
		{
			TArray<FString> FieldComponents;
			FieldName.ParseIntoArray(FieldComponents, TEXT("."));
			FieldName = FieldComponents.Pop();

			for (const FString& FieldComponent : FieldComponents)
			{
				if (!(*JsonObjectPtr)->TryGetObjectField(FieldComponent, JsonObjectPtr))
				{
					Success = false;
					return;
				}
			}
		}
		const TArray< TSharedPtr<FJsonValue> >* members;
		if ((*JsonObjectPtr)->TryGetArrayField(FieldName, members)) {
			for (int32 i = 0; i < (*members).Num(); ++i)
			{
				TSharedPtr<FJsonObject> tempObj = (*members)[i]->AsObject();
				TMap<FString, TSharedPtr<FJsonValue>> Values = tempObj->Values;
				FJsonMapStruct tempMap;
				for (TMap<FString, TSharedPtr<FJsonValue>>::TConstIterator iter = Values.CreateConstIterator(); iter; ++iter)
				{

					tempMap.maps.Emplace(iter->Key, iter->Value->AsString());
				}
				OutArray.Add(tempMap);
			}
			Success = true;
			return;
		}
		Success = false;
	}
	else
	{

		Success = false;
	}
}
