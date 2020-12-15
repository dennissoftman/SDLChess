#include "knightitem.hpp"
#include "gamewindow.hpp"
#include <cmath>

KnightItem::KnightItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void KnightItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"knightb":"knightw"), NULL, &t);
}

bool KnightItem::canMove(const vec2 &pos) const
{
    vec2 a = (oldPos - pos);

    if(sqrtf(a.x*a.x + a.y*a.y) == sqrtf(5.f))
        return true;
    return false;
}

bool KnightItem::canAttack(const vec2 &pos) const
{
    return canMove(pos);
}

CHESS_FIGURE KnightItem::getFigure() const
{
    return KNIGHT_FIG;
}
