#include "ZpeopleHttpRequestHelper.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"
#include "Http.h"

DEFINE_LOG_CATEGORY_STATIC(LogHttpRequest, Warning, All);


UZpeopleHttpRequestHelper::UZpeopleHttpRequestHelper(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UZpeopleHttpRequestHelper* UZpeopleHttpRequestHelper::HttpRequestString(FString url, ZpeopleHttpRequestType Type, FString Content)
{
	UZpeopleHttpRequestHelper* Helper = NewObject<UZpeopleHttpRequestHelper>();
	Helper->Start(url,Type,Content);
	return Helper;
}
void UZpeopleHttpRequestHelper::Start(FString url, ZpeopleHttpRequestType Type, FString Content)
{
	TSharedRef<class IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UZpeopleHttpRequestHelper::HandleRequestComplete);
	HttpRequest->SetURL(url);
	if (Type==ZpeopleHttpRequestType::GET)
	{
		HttpRequest->SetVerb(TEXT("GET"));
	}
	else {
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("text/html; charset=utf-8"));
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContentAsString(Content);
	}
	HttpRequest->ProcessRequest();
}
void UZpeopleHttpRequestHelper::HandleRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded)
	{
		OnSuccess.Broadcast(HttpResponse->GetContentAsString(), HttpResponse->GetResponseCode());
	}
	else
	{
		if (HttpResponse.IsValid())
		{
			OnFail.Broadcast("", HttpResponse->GetResponseCode());
		}
		else {
			OnFail.Broadcast("Network error",0 );
		}
	}
}