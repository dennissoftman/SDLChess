#include "rookitem.hpp"
#include "gamewindow.hpp"

RookItem::RookItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void RookItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"rookb":"rookw"), NULL, &t);
}

bool RookItem::canMove(const vec2 &pos) const
{
    vec2 a = (oldPos - pos);
    if((a.x == 0.f) xor (a.y == 0.f))
        return true;
    return false;
}

bool RookItem::canAttack(const vec2 &pos) const
{
    return canMove(pos);
}

CHESS_FIGURE RookItem::getFigure() const
{
    return ROOK_FIG;
}
