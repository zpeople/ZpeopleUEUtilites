#pragma once
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ZpeopleHttpRequestHelper.generated.h"

using namespace UF;
using namespace UP;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FZpeopleHttpRequestDelegate, FString, Content, int32, StatusCode);

UENUM()
enum ZpeopleHttpRequestType
{
	GET,
	POST
};
UENUM()
enum ZpeopleHttpRequestContentType
{
	html,
	json,
	formdata
};
UCLASS()
class UZpeopleHttpRequestHelper : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Zpeople|Http", meta = (BlueprintInternalUseOnly = "true"))
	static UZpeopleHttpRequestHelper* HttpRequestString(FString url,ZpeopleHttpRequestType verbType, ZpeopleHttpRequestContentType ContentType  ,const TMap<FString, FString>&  Params, const TMap<FString, FString>& HeadMap, const FString& Body);


	UFUNCTION(BlueprintCallable, Category = "Zpeople|Http", meta = (BlueprintInternalUseOnly = "true"))
		static UZpeopleHttpRequestHelper* HttpUnloadFile(FString url,  const TMap<FString, FString> & Params, const TMap<FString, FString>& HeadMap, const FString& FilePath);
	UPROPERTY(BlueprintAssignable)
	FZpeopleHttpRequestDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FZpeopleHttpRequestDelegate OnFail;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString Boundary;
public:

private:
	static FString GetPairsStr_x_www_form( TMap<FString, FString> Params);
	void Start(FString url, ZpeopleHttpRequestType verbType, ZpeopleHttpRequestContentType ContentType,  TMap<FString, FString> Params,  TMap<FString, FString> HeadMap,  FString Body);
	void Start(FString url, ZpeopleHttpRequestType verbType, ZpeopleHttpRequestContentType ContentType,  TMap<FString, FString> Params,  TMap<FString, FString> HeadMap, TArray<uint8> RequestContent);
	void HandleRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
 	TArray<uint8> LoadFileToBytes(  FString filePATH);
};