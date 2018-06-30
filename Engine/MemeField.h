#pragma once
#include "Vei2.h"
#include "Graphics.h"

class MemeField
{
private: 
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Revealed,
			Flagged
		};
	public:
		bool IsHidden() const;
		void Draw(const Vei2& screenPos, Graphics& gfx) const;
		void SpawnMeme();
		bool HasMeme() const;
		void Reveal();
	private:
		State state = State::Hidden;
		Vei2 pos;
		bool isHidden = true;
		bool hasMeme = false;
		bool isFlagged = false;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	void OnRevealClick(const Vei2 mousePos);
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
private:
	static constexpr int width = 30;
	static constexpr int height = 29;
	Tile tiles[width * height];
};