#ifndef ROOKITEM_HPP
#define ROOKITEM_HPP

#include "genericpawnitem.hpp"

class RookItem : public GenericPawnItem
{
public:
    RookItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;

    CHESS_FIGURE getFigure() const override;
};

#endif // ROOKITEM_HPP
