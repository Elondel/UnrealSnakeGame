// Snake Game, Copyright Yaroslav Tsiapkalo. All Rights Reserved

#include "Core/Snake.h"

using namespace SnakeGame;

Snake::Snake(const Settings::Snake& settings)
{
    checkf(settings.defaultSize >= 4, TEXT("Snake length is too small: &i"), settings.defaultSize);

    // add snake links horizontally to the left
    for (uint32 i{0}; i < settings.defaultSize; i++)
    {
        m_links.AddTail(Position{settings.startPosition.X - i, settings.startPosition.Y});
    }
}

void SnakeGame::Snake::move(const Input& input)
{
    // can't move opposite direction
    if (!m_lastInput.opposite(input))
    {
        m_lastInput = input;
    }

    m_links.MoveTail(m_links.GetTail(), m_links.GetHead(), m_links.GetHead()->GetValue());
    m_links.GetHead()->GetValue() += Position(m_lastInput.x, m_lastInput.y);
}
