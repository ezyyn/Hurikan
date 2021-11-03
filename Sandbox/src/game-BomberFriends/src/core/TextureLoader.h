#pragma once
#include <Hurikan.h>
using namespace Hurikan;

class TextureLoader
{
public:
	static void Init();
	
	static TextureLoader* Get();

	Ref<Texture2D> Load(const std::string name);
private:
	TextureLoader() 
	{
		m_Map["PlayerDown"] = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/facingdown/1.png");
		m_Map["PlayerDown1"] = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/facingdown/step1.png");
		m_Map["PlayerDown2"] = Texture2D::Create("src/game-BomberFriends/assets/textures/player_animation/facingdown/step2.png");

		m_Map["Bomb-Frame1"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/1.png");
		m_Map["Bomb-Frame2"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/2.png");
		m_Map["Bomb-Frame3"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/3.png");
		m_Map["Bomb-Frame4"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/4.png");
		m_Map["Bomb-Frame5"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/5.png");
		m_Map["Bomb-Frame6"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/6.png");
		m_Map["Bomb-Frame7"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/7.png");
		m_Map["Bomb-Frame8"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/8.png");
		m_Map["Bomb-Frame9"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/9.png");
		m_Map["Bomb-Frame10"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/10.png");
		m_Map["Bomb-Frame11"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/11.png");
		m_Map["Bomb-Frame12"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/12.png");
		m_Map["Bomb-Frame13"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/13.png");
		m_Map["Bomb-Frame14"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/14.png");
		m_Map["Bomb-Frame15"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/15.png");
		m_Map["Bomb-Frame16"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/16.png");
		m_Map["Bomb-Frame17"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/17.png");
		m_Map["Bomb-Frame18"] = Texture2D::Create("src/game-BomberFriends/assets/textures/bomb_animation/18.png");
	}
	std::unordered_map<std::string, Ref<Texture2D>> m_Map;

	static TextureLoader* s_Instance;
};