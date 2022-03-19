// Nikita Kitov. All rights reserved


#include "STUGameModeBase.h"
#include "STUBaseCharacter.h"
#include "STUHUD.h"
#include "STUPlayerController.h"

ASTUGameModeBase::ASTUGameModeBase()
{
	DefaultPawnClass = ASTUBaseCharacter::StaticClass();
	PlayerControllerClass = ASTUPlayerController::StaticClass();
	HUDClass = ASTUHUD::StaticClass();
}
