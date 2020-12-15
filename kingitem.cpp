#include "kingitem.hpp"
#include "gamewindow.hpp"
#include <cmath>

KingItem::KingItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void KingItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"kingb":"kingw"), NULL, &t);
}

bool KingItem::canMove(const vec2 &pos) const
{
    vec2 a = (oldPos - pos);

    float len = sqrtf(a.x*a.x + a.y*a.y);
    if(len == sqrtf(2.f) or len == 1.f)
        return true;
    return false;
}

bool KingItem::canAttack(const vec2 &pos) const
{
    return canMove(pos);
}

CHESS_FIGURE KingItem::getFigure() const
{
    return KING_FIG;
}
