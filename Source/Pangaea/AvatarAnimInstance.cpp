#include "AvatarAnimInstance.h"
#include "Avatar.h"

void UAvatarAnimInstance::OnStateAnimationEnds()
{
	if(AvatarState == EAvatarState::Attack)
	{
		AvatarState = EAvatarState::Locomotion;
	}
	else
	{
		AAvatar* avatar = GetOwningAvatar();
		if(avatar != nullptr)
		{
			if(AvatarState == EAvatarState::Hit)
			{
				if(!avatar->IsKilled())
				{
					AvatarState = EAvatarState::Locomotion;
				}
				else
				{
					AvatarState = EAvatarState::Die;
					avatar->DieProcess();
				}
			}
		}
	}
}

AAvatar* UAvatarAnimInstance::GetOwningAvatar()
{
	auto owningAvatar = Cast<AAvatar>(GetOwningActor());
	return owningAvatar;
}
