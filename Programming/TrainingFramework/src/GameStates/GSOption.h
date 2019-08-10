#pragma once
#include "GameStatebase.h"
#include "GameButton.h"

class Sprite2D;
class Sprite3D;
class Text;

class GSOption :
	public GameStateBase
{
public:
	GSOption();
	~GSOption();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void HandleMouseEvents(int x, int y) override;
	void Update(float deltaTime);
	void Draw();

private:

	std::shared_ptr<Sprite2D> m_BackGround;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
//	std::shared_ptr<Text>  m_Text_gameName;

};

