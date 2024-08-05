// Copyright Zachary Brett, 2024. All rights reserved.

#pragma once

//UE
#include "CoreMinimal.h"
//Plugin
#include "Conditionals/Queries/Base/DialogueQueryInt.h"
#include "DialogueSpeakerComponent.h"
//Generated
#include "SpeakerQueryInt.generated.h"

class UDialogueSpeakerSocket;

/**
 * Abstract base class for dialogue queries that operate on a 
 * speaker component and return an integer value. Designed to be 
 * implemented by the user in blueprint. 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class DIALOGUETREERUNTIME_API USpeakerQueryInt : public UDialogueQueryInt
{
	GENERATED_BODY()

public:
	/** IDialogueQueryInt Impl. */
	virtual int32 ExecuteQuery() override;
	virtual bool IsValidQuery() const override;
	/** End IDialogueQueryInt */

	/**
	* User specified query. Implemented via blueprint. 
	* 
	* @param InSpeaker - FSpeakerActor, struct containing target speaker 
	* component and actor
	* @param OtherSpeakers - TArray<FSpeakerActor>, any additional speakers
	* @return int32 - the value of the query. 
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue")
	int32 QuerySpeaker(FSpeakerActorEntry InSpeaker, const TArray<FSpeakerActorEntry>& OtherSpeakers) const;

	/**
	* Fetches the dialogue query's speaker socket. 
	* 
	* @return UDialogueSpeakerSocket* - the socket.
	*/
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	UDialogueSpeakerSocket* GetSpeakerSocket() const;

	/**
	* Retrieves the list of optional additional speaker sockets for the query.
	*
	* @return TArray<UDialogueSpeakerSocket*> - the list of optional additional
	* speakers.
	*/
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	TArray<UDialogueSpeakerSocket*> GetAdditionalSpeakerSockets() const;

	/**
	* User defineable function to check if the query is valid. 
	* 
	* @return bool - True if valid according to terms user sets. False 
	* otherwise.
	*/
	UFUNCTION(BlueprintNativeEvent, Category="Dialogue")
	bool IsValidSpeakerQuery() const;
	virtual bool IsValidSpeakerQuery_Implementation() const;

private:
	/** The speaker socket for the target speaker */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDialogueSpeakerSocket* Speaker;

	/** Optional additional speakers to attach to the event */
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TArray<UDialogueSpeakerSocket*> AdditionalSpeakers;
};
