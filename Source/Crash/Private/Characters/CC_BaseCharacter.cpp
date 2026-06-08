#include "Crash/Public/Characters/CC_BaseCharacter.h"


ACC_BaseCharacter::ACC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    // 렌더링 여부와 관계 없이 뼈대는 데디케이트 서버에서 업데이트 됨
    // 애니메이션에서 히트박스를 트리거하면 서버에서 업데이트 됨
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}
