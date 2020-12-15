#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

#include <sqlite3.h>

#include "genericpawnitem.hpp"

#define SCR_WIDTH  (1280)
#define SCR_HEIGHT (720)

#define MAX_TEXT_ROWS (28)

class GameWindow
{
public:
    GameWindow();
    ~GameWindow();

    //
    void mainLoop();

    void draw_moves();
    void draw_chessboard();
    void draw_figs();

    void init_figs();
    void restartGame();

    SDL_Texture* getTex(const char *name) const;
    //

    void checkKingUnderAttack();
    void checkPawnMorph();
    
    void showMoves(GenericPawnItem *pawn);
    void setAvailableMove(int c, int n);
    void clearAvailableMove(int c, int n);
    void hideMoves();

    void setAvailableAttack(int c, int n);
    void clearAvailableAttack(int c, int n);

    CHESS_TEAM getTeam() const;

    void changeOccupy(const vec2 &_old, const vec2 &_new);
    void endMove();
    void killFigure(const vec2 &pos);

    bool canAttack(bool col, const vec2 &pos);
    bool canMove(const vec2 &pos);
    bool canGoBy(GenericPawnItem *pawn, const vec2 &pos);
    bool canSwap(GenericPawnItem *pawn, bool longswap);
    void doSwap(bool longswap);

    uint16_t width() const;
    uint16_t height() const;

    void showLeaderboard(); // blocking function

    //
    static int sqlCallback(void *userPtr, int argc, char **argv, char **colName);
    //

    static GameWindow *winPtr;
private:
    SDL_Window *drawWindow;
    SDL_Renderer *drawCanvas;
    uint16_t w_width, w_height;
    bool m_quit;
    //
    CHESS_TEAM currentTeam;

    //
    SDL_Texture *blackPawnTex, *whitePawnTex;
    SDL_Texture *blackRookTex, *whiteRookTex;
    SDL_Texture *blackKnightTex, *whiteKnightTex;
    SDL_Texture *blackBishopTex, *whiteBishopTex;
    SDL_Texture *blackKingTex, *whiteKingTex;
    SDL_Texture *blackQueenTex, *whiteQueenTex;
    SDL_Texture *scrTex, *idTex;
    SDL_Texture *moveTex, *attackTex, *swapTex, *warnTex, *restartTex;
    SDL_Texture *blackLostTex, *whiteLostTex;
    SDL_Texture *leaderboardTex;
    //
    uint64_t available_moves, available_attacks;
    uint8_t available_swaps;
    bool king_under_attack;
    GenericPawnItem *occupiedPlaces[8][8];
    GenericPawnItem *deadPawns[32];
    int deadCount, wdp, bdp;
    bool paused_game;
    //
    uint32_t blackTurns, whiteTurns;
    time_t start_time;
    sqlite3 *baseHandler;
};

#endif // GAMEWINDOW_HPP
