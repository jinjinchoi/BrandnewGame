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
}