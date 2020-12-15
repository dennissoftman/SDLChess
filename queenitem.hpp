#ifndef QUEENITEM_HPP
#define QUEENITEM_HPP

#include "genericpawnitem.hpp"

class QueenItem : public GenericPawnItem
{
public:
    QueenItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;

    CHESS_FIGURE getFigure() const override;
};

#endif // QUEENITEM_HPP
