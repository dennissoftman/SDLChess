#ifndef KNIGHTITEM_HPP
#define KNIGHTITEM_HPP

#include "genericpawnitem.hpp"

class KnightItem : public GenericPawnItem
{
public:
    KnightItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;
    
    CHESS_FIGURE getFigure() const override;
};

#endif // KNIGHTITEM_HPP
