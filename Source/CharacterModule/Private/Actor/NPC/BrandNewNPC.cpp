


#include "Actor/NPC/BrandNewNPC.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/Character/BrandNewPlayerInterface.h"


ABrandNewNPC::ABrandNewNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComponent->InitCapsuleSize(42.f, 96.f); // Radius, HalfHeight
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	SetRootComponent(CapsuleComponent);
	
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetupAttachment(GetRootComponent());
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetSphereRadius(250.f);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget Component"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());

	NPCNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPC Name Widget Component"));
	NPCNameWidgetComponent->SetupAttachment(GetRootComponent());
	
	LocationWidgetComponent->SetupAttachment(GetRootComponent());
	
}

FName ABrandNewNPC::GetFirstDialogueId() const
{
	return FirstDialogueId;
}

void ABrandNewNPC::HideInteractionWidget() const
{
	InteractionWidgetComponent->SetVisibility(false);
}


// Called when the game starts or when spawned
void ABrandNewNPC::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereBeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnSphereEndOverlap);

	InteractionWidgetComponent->SetVisibility(false);
	NPCNameWidgetComponent->SetVisibility(false);

}

void ABrandNewNPC::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player")) return;
	
	if (const ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (Player->IsLocallyControlled())
		{
			InteractionWidgetComponent->SetVisibility(true);
			NPCNameWidgetComponent->SetVisibility(true);

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
			NPCNameWidgetComponent->SetVisibility(false);
		}
	}

	if (IBrandNewPlayerInterface* PlayerInterface = Cast<IBrandNewPlayerInterface>(OtherActor))
	{
		PlayerInterface->RemoveOverlappedNPC(this);
	}
	
}
