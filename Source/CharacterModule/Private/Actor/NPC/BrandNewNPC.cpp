


#include "Actor/NPC/BrandNewNPC.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"


ABrandNewNPC::ABrandNewNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	SetRootComponent(SkeletalMeshComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetSphereRadius(250.f);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget Component"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	
}

FName ABrandNewNPC::GetFirstDialogueId()
{
	return FirstDialogueId;
}


// Called when the game starts or when spawned
void ABrandNewNPC::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereEndOverlap);

	InteractionWidgetComponent->SetVisibility(false);
	
}

void ABrandNewNPC::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	if (const ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (Player->IsLocallyControlled())
		{
			InteractionWidgetComponent->SetVisibility(true);
		}
	}

	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor))
	{
		PlayerInterface->AddOverlappedNPC(this);
	}
	
}

void ABrandNewNPC::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	if (const ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (Player->IsLocallyControlled())
		{
			InteractionWidgetComponent->SetVisibility(false);
		}
	}

	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor))
	{
		PlayerInterface->RemoveOverlappedNPC(this);
	}
	
}
