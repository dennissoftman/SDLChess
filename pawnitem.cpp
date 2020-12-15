#include "pawnitem.hpp"
#include "gamewindow.hpp"

PawnItem::PawnItem(bool black, const vec2 &pos)
    : GenericPawnItem(black, pos)
{

}

void PawnItem::draw(SDL_Renderer *rend)
{
    SDL_Rect t;
    t.x = 100 + currentPos.x*CHECKER_S;
    t.y = GameWindow::winPtr->height() - 100 - CHECKER_S - 8 - currentPos.y * CHECKER_S;
    t.w = t.h = CHECKER_S;
    SDL_RenderCopy(rend, GameWindow::winPtr->getTex(isBlack?"pawnb":"pawnw"), NULL, &t);
}

bool PawnItem::canMove(const vec2 &newPos) const
{
    vec2 a;
    if(isBlack)
        a = (oldPos - newPos);
    else
        a = (newPos - oldPos);

    if(a.x != 0.f)
        return false;

    if(oldPos.y == (isBlack?6:1))
    {
        if(a.y == 1.f or a.y == 2.f)
            return true;
    }
    else if(a.y == 1.f)
        return true;

    return false;
}

bool PawnItem::canAttack(const vec2 &attackPos) const
{
    vec2 a;
    if(isBlack)
        a = (oldPos - attackPos);
    else
        a = (attackPos - oldPos);

    if(abs(a.x) == 1 and a.y == 1.f)
        return true;
    return false;
}

CHESS_FIGURE PawnItem::getFigure() const
{
    return PAWN_FIG;
}
