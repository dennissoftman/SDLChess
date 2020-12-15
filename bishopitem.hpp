#ifndef BISHOPITEM_HPP
#define BISHOPITEM_HPP

#include "genericpawnitem.hpp"

class BishopItem : public GenericPawnItem
{
public:
    BishopItem(bool black, const vec2 &pos);

    void draw(SDL_Renderer *rend) override;

    bool canMove(const vec2 &pos) const override;
    bool canAttack(const vec2 &pos) const override;

    CHESS_FIGURE getFigure() const override;
};

#endif // BISHOPITEM_HPP
