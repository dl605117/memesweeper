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
		void Draw(const Vei2& screenPos, bool fucked, Graphics& gfx) const;
		void SpawnMeme();
		bool HasMeme() const;
		void Reveal();
		void ToggleFlag();
		bool IsFlagged() const;
		bool IsRevealed() const;
		void SetMemeCount(int memes);
	private:
		State state = State::Hidden;
		Vei2 pos;
		bool hasMeme = false;
		int memeCount = -1;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	void OnRevealClick(const Vei2 mousePos);
	void OnFlagClick(const Vei2 mousePos);
	RectI GetRect() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2 screenPos);
	int CountMemesAround(const Vei2& gridPos);
private:
	static constexpr int width = 30;
	static constexpr int height = 29;
	Tile tiles[width * height];
	bool isFucked = false;
};