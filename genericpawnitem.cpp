#include "pawnitem.hpp"

#include "gamewindow.hpp"

GenericPawnItem::GenericPawnItem(bool is_black, const vec2 &pos)
    : currentPos(pos), oldPos(pos),
      isBlack(is_black),
      isDead(false),
      hasMoved(false)
{

}

GenericPawnItem::~GenericPawnItem()
{

}

void GenericPawnItem::mousePressEvent()
{
    if(isDead)
        return;
    if((isBlack && GameWindow::winPtr->getTeam() != BLACK_TEAM) ||
       (!isBlack && GameWindow::winPtr->getTeam() != WHITE_TEAM))
        return;

    oldPos = currentPos;
    GameWindow::winPtr->showMoves(this);
}

void GenericPawnItem::mouseMoveEvent(const vec2 &mpos)
{
    if(isDead)
        return;
    if((isBlack && GameWindow::winPtr->getTeam() != BLACK_TEAM) ||
       (!isBlack && GameWindow::winPtr->getTeam() != WHITE_TEAM))
        return;

    vec2 np = (mpos - vec2(CHECKER_S*1.5, CHECKER_S*1.7))/CHECKER_S;

    if(np.x < 0 || np.x >= 8)
        return;
    if(np.y < 0 || np.y >= 8)
        return;

    setPos(np);
}

void GenericPawnItem::mouseReleaseEvent()
{
    GameWindow::winPtr->hideMoves();
    if(isDead)
        return;
    if((isBlack && GameWindow::winPtr->getTeam() != BLACK_TEAM) ||
       (!isBlack && GameWindow::winPtr->getTeam() != WHITE_TEAM))
        return;

    if(!GameWindow::winPtr->canGoBy(this, currentPos))
    {
        setPos(oldPos);
        return;
    }

    if((pos().x >= 8 or pos().x < 0) or
       (pos().y >= 8 or pos().y < 0))
    {
        setPos(oldPos);
        return;
    }

    if(canMove(currentPos))
    {
        if(GameWindow::winPtr->canMove(currentPos))
        {
            GameWindow::winPtr->changeOccupy(oldPos, currentPos);
            oldPos = currentPos;
            GameWindow::winPtr->endMove();
            if(!hasMoved)
                hasMoved = true;
            return;
        }
    }
    if(canAttack(currentPos))
    {
        if(GameWindow::winPtr->canAttack(isBlack, currentPos))
        {
            GameWindow::winPtr->killFigure(currentPos);
            GameWindow::winPtr->changeOccupy(oldPos, currentPos);
            oldPos = currentPos;
            GameWindow::winPtr->endMove();
            if(!hasMoved)
                hasMoved = true;
        }
        else
            setPos(oldPos);
    }
    else
    {
        if(getFigure() == KING_FIG)
        {
            bool is_short = (currentPos.x == 1) and (currentPos.y == (isBlack?7:0));
            bool is_long  = (currentPos.x == 6) and (currentPos.y == (isBlack?7:0));

            if((is_short || is_long) == true)
            {
                if(GameWindow::winPtr->canSwap(this, is_long))
                {
                    GameWindow::winPtr->doSwap(is_long);
                    oldPos = currentPos;
                    GameWindow::winPtr->endMove();
                    return;
                }
            }
        }
        setPos(oldPos);
    }
}

const vec2 &GenericPawnItem::pos() const
{
    return currentPos;
}

const vec2 &GenericPawnItem::getOldPos() const
{
    return oldPos;
}

void GenericPawnItem::setPos(const vec2 &newPos)
{
    currentPos = newPos;
}

void GenericPawnItem::die()
{
    isDead = true;
}

bool GenericPawnItem::moved() const
{
    return hasMoved;
}

void GenericPawnItem::setBlack(bool yes)
{
    isBlack = yes;
}

bool GenericPawnItem::getBlack() const
{
    return isBlack;
}


