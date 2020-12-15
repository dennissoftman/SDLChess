#include "queenitem.hpp"
#include "gamewindow.hpp"

QueenItem::QueenItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void QueenItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"queenb":"queenw"), NULL, &t);
}

bool QueenItem::canMove(const vec2 &pos) const
{
    vec2 a = (oldPos - pos);

    if(a.isNul())
        return false;
    if(((a.x == 0.f) xor (a.y == 0.f)) or (abs(a.x) == abs(a.y)))
        return true;
    return false;
}

bool QueenItem::canAttack(const vec2 &pos) const
{
    return canMove(pos);
}

CHESS_FIGURE QueenItem::getFigure() const
{
    return QUEEN_FIG;
}

