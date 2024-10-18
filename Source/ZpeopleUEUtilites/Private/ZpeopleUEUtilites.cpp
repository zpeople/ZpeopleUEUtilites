// Copyright 1998-2015 Zpeople Games, Inc. All Rights Reserved.
#include "ZpeopleUEUtilites.h"
#include "ZpeopleUEUtilitesPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

static const FName ZpeopleUEUtilitesTabName("ZpeopleUEUtilites");

#define LOCTEXT_NAMESPACE "FZpeopleUEUtilitesModule"

void FZpeopleUEUtilitesModule::StartupModule()
{

}

void FZpeopleUEUtilitesModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FZpeopleUEUtilitesModule, ZpeopleUEUtilites)