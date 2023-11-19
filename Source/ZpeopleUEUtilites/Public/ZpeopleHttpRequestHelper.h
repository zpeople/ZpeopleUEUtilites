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

UCLASS()
class UZpeopleHttpRequestHelper : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Zpeople|Http", meta = (BlueprintInternalUseOnly = "true"))
	static UZpeopleHttpRequestHelper* HttpRequestString(FString url,ZpeopleHttpRequestType Type,FString Content);

	UPROPERTY(BlueprintAssignable)
	FZpeopleHttpRequestDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FZpeopleHttpRequestDelegate OnFail;

public:

private:
	void Start(FString URL, ZpeopleHttpRequestType Type, FString Content);
	void HandleRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
};