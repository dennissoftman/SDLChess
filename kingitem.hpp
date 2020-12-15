#ifndef KINGITEM_HPP
#define KINGITEM_HPP

#include "genericpawnitem.hpp"

class KingItem : public GenericPawnItem
{
public:
    KingItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;
    
    CHESS_FIGURE getFigure() const override;
};

#endif // KINGITEM_HPP
