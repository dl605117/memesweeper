#include "MemeField.h"
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

bool MemeField::Tile::IsHidden() const
{
	return state == State::Hidden;
}

void MemeField::Tile::Draw(const Vei2& screenPos, bool fucked, Graphics& gfx) const
{
	if (!fucked)
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
				SpriteCodex::DrawTileNumber(screenPos, memeCount, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
		}
	}
	else
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, memeCount, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
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
	if (!IsFlagged() && !IsRevealed())
	{
		state = State::Revealed;
	}
}

void MemeField::Tile::ToggleFlag()
{
	if (!IsRevealed())
	{
		if (IsFlagged())
		{
			state = State::Hidden;
		}
		else
		{
			state = State::Flagged;
		}
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

void MemeField::Tile::SetMemeCount(int memes)
{
	assert(memeCount == -1);
	memeCount = memes;
}

MemeField::MemeField(int nMemes)
{
	assert(nMemes >= 0 && nMemes < width * height);
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

	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).SetMemeCount(CountMemesAround(gridPos));
		}
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(RectI({ 0,0 }, Vei2 { width, height } * SpriteCodex::tileSize), SpriteCodex::baseColor);

	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, isFucked, gfx);
		}
	}
}

void MemeField::OnRevealClick(const Vei2 mousePos)
{
	if (!isFucked)
	{
		Tile& tile(TileAt(ScreenToGrid(mousePos)));
		tile.Reveal();
		if (tile.HasMeme())
		{
			isFucked = true;
		}
	}
}

void MemeField::OnFlagClick(const Vei2 mousePos)
{
	if (!isFucked)
	{
		TileAt(ScreenToGrid(mousePos)).ToggleFlag();
	}
}

Vei2 MemeField::ScreenToGrid(const Vei2 screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

RectI MemeField::GetRect() const
{
	return RectI(0, width * SpriteCodex::tileSize, 0, height * SpriteCodex::tileSize);
}

MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos)
{
	return tiles[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return tiles[gridPos.y * width + gridPos.x];
}

int MemeField::CountMemesAround(const Vei2 & gridPos)
{
	const int startX = std::max(0, gridPos.x - 1);
	const int startY = std::max(0, gridPos.y - 1);
	const int endX = std::min(width - 1, gridPos.x + 1);
	const int endY = std::min(height - 1, gridPos.y + 1);

	int countMemes = 0;
	for (Vei2 gridPos{ startX, startY }; gridPos.x <= endX; gridPos.x++)
	{
		for (gridPos.y = startY; gridPos.y <= endY; gridPos.y++)
		{
			if (TileAt( gridPos ).HasMeme())
			{
				countMemes++;
			}
		}
	}
	return countMemes;
}