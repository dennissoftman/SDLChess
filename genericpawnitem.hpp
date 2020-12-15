#ifndef GENERICPAWNITEM_HPP
#define GENERICPAWNITEM_HPP

#define CHECKER_S (64)

#include <cmath>
#include <SDL2/SDL2_gfxPrimitives.h>

struct vec2
{
    int x,y;
    vec2(int _x=0, int _y=0)
        : x(_x), y(_y)
    {}

    vec2 operator -(const vec2 &a) const
    {
        return vec2(x-a.x, y-a.y);
    }

    vec2 operator +(const vec2 &a) const
    {
        return vec2(x+a.x, y+a.y);
    }

    vec2 operator /(int a) const
    {
        return vec2(x/a, y/a);
    }
    
    bool operator ==(const vec2 &other) const
    {
        return (x==other.x) and (y==other.y);
    }

    bool operator !=(const vec2 &other) const
    {
        return (x != other.x) or (y != other.y);
    }

    vec2 &operator +=(const vec2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    vec2 &operator -=(const vec2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    bool isNul() const
    {
        return (x == 0.f) and (y == 0.f);
    }
};

enum CHESS_FIGURE
{
    PAWN_FIG,
    ROOK_FIG,
    KNIGHT_FIG,
    BISHOP_FIG,
    KING_FIG,
    QUEEN_FIG
};

enum CHESS_TEAM
{
    BLACK_TEAM,
    WHITE_TEAM
};

class GenericPawnItem
{
public:
    GenericPawnItem(bool is_black, const vec2 &pos);
    virtual ~GenericPawnItem();

    virtual void draw(SDL_Renderer *rend) = 0;

    void mousePressEvent();
    void mouseMoveEvent(const vec2 &mpos);
    void mouseReleaseEvent();

    virtual bool canMove(const vec2 &newPos) const = 0;
    virtual bool canAttack(const vec2 &attackPos) const = 0;
    virtual CHESS_FIGURE getFigure() const = 0;

    const vec2 &pos() const;
    const vec2 &getOldPos() const;
    void setPos(const vec2 &newPos);

    void die(void);
    bool moved(void) const;

    void setBlack(bool yes);
    bool getBlack(void) const;
protected:
    vec2 currentPos, oldPos;
    bool isBlack;
    bool isDead;
    bool hasMoved;
};


#endif // GENERICPAWNITEM_HPP
