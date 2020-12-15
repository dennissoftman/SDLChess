#include "bishopitem.hpp"
#include "gamewindow.hpp"

BishopItem::BishopItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void BishopItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"bishopb":"bishopw"), NULL, &t);
}

bool BishopItem::canMove(const vec2 &pos) const
{
    vec2 a = (oldPos - pos);

    if(abs(a.x) == abs(a.y))
        return true;

    return false;
}

bool BishopItem::canAttack(const vec2 &pos) const
{
    return canMove(pos);
}

CHESS_FIGURE BishopItem::getFigure() const
{
    return BISHOP_FIG;
}
