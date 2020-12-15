#ifndef PAWNITEM_HPP
#define PAWNITEM_HPP

#include "genericpawnitem.hpp"

class PawnItem : public GenericPawnItem
{
public:
    PawnItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;

    CHESS_FIGURE getFigure() const override;
};

#endif // PAWNITEM_HPP
