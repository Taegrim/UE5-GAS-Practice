#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CCTags
{
    namespace CCAbilities
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);

        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Secondary);
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tertiary);

        namespace Enemy
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
        }
    }

    namespace Events
    {
        namespace Player
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
        }

        namespace Enemy
        {
            UE_DECLARE_GAMEPLAY_TAG_EXTERN(HitReact);
        }
    }
}
