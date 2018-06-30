#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

bool MemeField::Tile::IsHidden() const
{
	return state == State::Hidden;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	switch (state)
	{
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!HasMeme())
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
	}
}

void MemeField::Tile::SpawnMeme()
{
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Reveal()
{
	state = State::Revealed;
}

MemeField::MemeField(int nMemes)
{
	assert(nMemes >= 0);
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int i = 0; i < nMemes; i++)
	{
		Vei2 spawnLoc;
		do
		{
			spawnLoc = { xDist(rng), yDist(rng) };
		} while (TileAt(spawnLoc).HasMeme());
		TileAt(spawnLoc).SpawnMeme();
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(RectI({ 0,0 }, Vei2 { width, height } * SpriteCodex::tileSize), SpriteCodex::baseColor);
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt( gridPos ).Draw( gridPos * SpriteCodex::tileSize, gfx );
		}
	}
}

void MemeField::OnRevealClick(const Vei2 mousePos)
{
	TileAt(mousePos / SpriteCodex::tileSize).Reveal();
}

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return tiles[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return tiles[gridPos.y * width + gridPos.x];
}