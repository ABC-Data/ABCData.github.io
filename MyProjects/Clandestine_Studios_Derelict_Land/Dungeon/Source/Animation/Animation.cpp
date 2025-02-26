/******************************************************************************/
/*!
\file    Animation.h
\author  Warren Ang Jun Xuan
\par     email: a.warrenjunxuan@digipen.edu
\date    February 2, 2023
\brief   Animation System

		Copyright (C) 2022 DigiPen Institute of Technology.
		Reproduction or disclosure of this file or its contents without the
		prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Animation.h"

Animation::Animation(){

}

Animation::~Animation() {

}

void Animation::Init() {
	time_elapsed = 0;
	sprite_counter = 0;
	time = 0;
	anim_dt = 0;

	for (size_t i = 1; i < this->GetLevel()->GetGameObjectArray().size(); ++i)
	{
		Animate* Anim = dynamic_cast<Animate*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Animate));
		if (Anim != nullptr) {
			Anim->SetTotalFrame(this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).frames_per_row - 1);
			Anim->SetCurrentFrame(this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx);
		}
	}
}

void Animation::Update(float dt) {
	anim_dt = dt;
	time_elapsed += dt;
	time = time_elapsed / SPRITE_DURATION;
	if (static_cast<int>(time) > sprite_counter) {
		sprite_counter = static_cast<int>(time);
		for (size_t i = 1; i < this->GetLevel()->GetGameObjectArray().size(); ++i)
		{
			Animate* Anim = dynamic_cast<Animate*>((this->GetLevel()->GetGameObjectArray())[i]->GetComponent(ComponentType::Animate));			
			if (Anim != nullptr) {				
				//Anim->SetCurrentFrame(this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx);
				//Anim->SetTotalFrame(this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).frames_per_row);
				if (Anim->GetVisibility() && Anim->GetIsPlaying()) {
					Anim->delay += (int)(dt*1000);
					if (Anim->delay > Anim->GetSpriteSpeed()) {
						Anim->delay = 0;
						this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx
							= this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx == this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).frames_per_row - 1
							? 0 : this->GetLevel()->GetAssets()->GetSprite(Anim->GetTexHandle()).idx + 1;
					}
				}
			}
		}
	}
}

void Animation::Shutdown() {

}

bool Animation::PlayAnimation(Animate* Object) {
	if (Object != nullptr) {
		//std::cout << "CurrentFrame: " << Object->GetCurrentFrame() << "\nTotalFrame: " << Object->GetTotalFrame() << "\n";
		if (!Object->GetVisibility())
			Object->SetVisibility(true);
		if (!Object->GetIsPlaying())
			Object->SetIsPlaying(true);

		//std::cout << "Frame: " << Object->GetCurrentFrame() << "\n";
		if (Object->GetCurrentFrame() < Object->GetTotalFrame() - 1)
			return 1;
	}
	return 0;
}
