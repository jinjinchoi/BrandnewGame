#pragma once

namespace DebugHelper
{
	static void Print(const FString& Message, const FColor& Color = FColor::MakeRandomColor(), const float Time = 7.f,  const int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, Time, Color, Message);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
		}
	}

	static void Print(const UObject* WorldContext, const FString& Message, const FColor& Color = FColor::MakeRandomColor(), const float Time = 7.f, const int32 InKey = -1)
	{
		if (!WorldContext || !GEngine) return;

		const UWorld* World = WorldContext->GetWorld();
		if (!World) return;

		FString Prefix;
		if (World->GetNetMode() == NM_DedicatedServer || World->GetNetMode() == NM_ListenServer)
		{
			Prefix = TEXT("[Server] ");
		}
		else
		{
			Prefix = TEXT("[Client] ");
		}

		const FString FinalMessage = Prefix + Message;

		GEngine->AddOnScreenDebugMessage(InKey, Time, Color, FinalMessage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMessage);
	}
	
}