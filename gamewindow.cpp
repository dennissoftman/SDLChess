#include "gamewindow.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

//
#include "pawnitem.hpp"
#include "rookitem.hpp"
#include "knightitem.hpp"
#include "bishopitem.hpp"
#include "kingitem.hpp"
#include "queenitem.hpp"
//

GameWindow* GameWindow::winPtr = nullptr;

GameWindow::GameWindow()
    : m_quit(false), currentTeam(WHITE_TEAM), king_under_attack(false), paused_game(false),
      baseHandler(nullptr)
{
    winPtr = this;

    w_width = SCR_WIDTH;
    w_height = SCR_HEIGHT;

    IMG_Init(IMG_INIT_PNG);

    drawWindow = SDL_CreateWindow("SDL Chess",
                                  0,0,
                                  w_width,w_height,
                                  SDL_WINDOW_SHOWN);
    assert(drawWindow != nullptr);

    drawCanvas = SDL_CreateRenderer(drawWindow,
                                    -1,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(drawCanvas != nullptr);

    // PAWNS ===================================================================================
    blackPawnTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_pawn.png"));
    whitePawnTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_pawn.png"));
    // ROOKS ===================================================================================
    blackRookTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_rook.png"));
    whiteRookTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_rook.png"));
    // KNIGHTS =================================================================================
    blackKnightTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_knight.png"));
    whiteKnightTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_knight.png"));
    // BISHOPS =================================================================================
    blackBishopTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_bishop.png"));
    whiteBishopTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_bishop.png"));
    // KINGS ===================================================================================
    blackKingTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_king.png"));
    whiteKingTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_king.png"));
    // QUEENS ==================================================================================
    blackQueenTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_queen.png"));
    whiteQueenTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_queen.png"));
    // =========================================================================================
    scrTex    = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/table.png"));
    idTex     = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/backdrop.png"));
    moveTex   = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/move.png"));
    attackTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/attack.png"));
    swapTex   = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/swap.png"));
    warnTex   = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/warn.png"));
    restartTex= SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/restart.png"));
    
    blackLostTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/black_lost.png"));
    whiteLostTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/white_lost.png"));

    leaderboardTex = SDL_CreateTextureFromSurface(drawCanvas, IMG_Load("img/leaderboard.png"));
    //
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
            occupiedPlaces[i][j] = nullptr;
    }
    for(int i=0; i < 32; i++)
        deadPawns[i] = nullptr;
    
    restartGame();

    if(sqlite3_open("chess.db", &baseHandler) != 0)
    {
        fprintf(stderr, "Failed to open DB\n");
        fflush(stdout);
    }
}

void GameWindow::restartGame()
{
    currentTeam = WHITE_TEAM;
    paused_game = false;
    king_under_attack = false;
    deadCount = wdp = bdp = 0;
    hideMoves();
    init_figs();
    blackTurns = whiteTurns = 0;
    start_time = time(nullptr);
}

GameWindow::~GameWindow() 
{
    for(int i=0; i < 32; i++)
    {
        if(deadPawns[i] != nullptr)
            delete deadPawns[i];
    }
    
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            if(occupiedPlaces[i][j] != nullptr)
                delete occupiedPlaces[i][j];
        }
    }
    
    // free pawns
    SDL_DestroyTexture(blackPawnTex);
    SDL_DestroyTexture(whitePawnTex);
    // free rooks
    SDL_DestroyTexture(blackRookTex);
    SDL_DestroyTexture(whiteRookTex);
    // free knights
    SDL_DestroyTexture(blackKnightTex);
    SDL_DestroyTexture(whiteKnightTex);
    // free bishops
    SDL_DestroyTexture(blackBishopTex);
    SDL_DestroyTexture(whiteBishopTex);
    // free kings
    SDL_DestroyTexture(blackKingTex);
    SDL_DestroyTexture(whiteKingTex);
    // free queens
    SDL_DestroyTexture(blackQueenTex);
    SDL_DestroyTexture(whiteQueenTex);

    SDL_DestroyTexture(scrTex);
    SDL_DestroyTexture(idTex);
    
    SDL_DestroyTexture(moveTex);
    SDL_DestroyTexture(attackTex);
    SDL_DestroyTexture(swapTex);
    SDL_DestroyTexture(warnTex);
    SDL_DestroyTexture(leaderboardTex);
    
    SDL_DestroyRenderer(drawCanvas);
    SDL_DestroyWindow(drawWindow);
    IMG_Quit();
}

SDL_Texture* GameWindow::getTex(const char* name) const
{
    if(strcmp(name, "pawnb") == 0)
        return blackPawnTex;
    if(strcmp(name, "pawnw") == 0)
        return whitePawnTex;

    if(strcmp(name, "rookb") == 0)
        return blackRookTex;
    if(strcmp(name, "rookw") == 0)
        return whiteRookTex;

    if(strcmp(name, "knightb") == 0)
        return blackKnightTex;
    if(strcmp(name, "knightw") == 0)
        return whiteKnightTex;

    if(strcmp(name, "bishopb") == 0)
        return blackBishopTex;
    if(strcmp(name, "bishopw") == 0)
        return whiteBishopTex;

    if(strcmp(name, "kingb") == 0)
        return blackKingTex;
    if(strcmp(name, "kingw") == 0)
        return whiteKingTex;

    if(strcmp(name, "queenb") == 0)
        return blackQueenTex;
    if(strcmp(name, "queenw") == 0)
        return whiteQueenTex;

    return nullptr;
}

void GameWindow::draw_moves()
{
    SDL_Rect t;
    t.w = t.h = CHECKER_S;
    
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            t.x = 100+j*CHECKER_S;
            t.y = height() - 100 - CHECKER_S - 8 - i * CHECKER_S;

            if((available_moves >> (i*8+j)) & 1)
                SDL_RenderCopy(drawCanvas, moveTex, NULL, &t);

            if ((available_attacks >> (i * 8 + j)) & 1)
            {
                SDL_RenderCopy(drawCanvas, attackTex, NULL, &t);
            }
                                    
            if(occupiedPlaces[i][j] != nullptr)
            {
                if((occupiedPlaces[i][j]->getFigure() == KING_FIG) &&
                   (occupiedPlaces[i][j]->getBlack() == (currentTeam == BLACK_TEAM)))
                {
                    if(king_under_attack)
                    {
                        SDL_RenderCopy(drawCanvas, warnTex, NULL, &t);
                    }
                }
            }
        }
    }

    if(available_swaps & 1)
    {
        t.x = 164 - CHECKER_S + 1*CHECKER_S;
        t.y = height() - 100 - CHECKER_S - 8 - (currentTeam==WHITE_TEAM?0:7)*CHECKER_S;
        SDL_RenderCopy(drawCanvas, swapTex, NULL, &t);
    }
    
    if(available_swaps & 2)
    {
        t.x = 164 - CHECKER_S + 6*CHECKER_S;
        t.y = height() - 100 - CHECKER_S - 8 - (currentTeam==WHITE_TEAM?0:7)*CHECKER_S;
        SDL_RenderCopy(drawCanvas, swapTex, NULL, &t);
    }
}

void GameWindow::draw_figs()
{
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            if(occupiedPlaces[i][j] == nullptr)
                continue;
            occupiedPlaces[i][j]->draw(drawCanvas);
        }
    }
    //
    SDL_Rect t;
    t.x = 100 + 9*CHECKER_S;
    t.y = 100;
    t.w = 8*CHECKER_S;
    t.h = 2*CHECKER_S;

    if(scrTex == nullptr)
    {
        SDL_SetRenderDrawColor(drawCanvas, 0x01, 0x32, 0x20, 0xFF);
        SDL_RenderFillRect(drawCanvas, &t);
        
        t.y = 100 + 6*CHECKER_S;
        SDL_RenderFillRect(drawCanvas, &t);
    }
    //
    for(int i=0; i < 32; i++)
        if(deadPawns[i] != nullptr)
            deadPawns[i]->draw(drawCanvas);
}

void GameWindow::draw_chessboard()
{
    SDL_Rect scrRect;
    scrRect.x = scrRect.y = 0;
    scrRect.w = SCR_WIDTH;
    scrRect.h = SCR_HEIGHT;
    
    SDL_Rect idRect;
    idRect.x = 100 - CHECKER_S;
    idRect.y = 100 - CHECKER_S;
    idRect.w = idRect.h = 10*CHECKER_S;
    
    SDL_RenderCopy(drawCanvas, scrTex, NULL, &scrRect);
    SDL_RenderCopy(drawCanvas, idTex, NULL, &idRect);
    
    SDL_Rect t;
    t.x = width() - CHECKER_S;
    t.y = 0;
    t.w = t.h = CHECKER_S;
    
    SDL_RenderCopy(drawCanvas, restartTex, NULL, &t);

    t.x = width() - CHECKER_S*2;
    t.y = 0;
    t.w = t.h = CHECKER_S;

    SDL_RenderCopy(drawCanvas, whiteQueenTex, NULL, &t);
}

void GameWindow::init_figs()
{
    for(int i=0; i < 32; i++)
    {
        if(deadPawns[i] != nullptr)
        {
            delete deadPawns[i];
            deadPawns[i] = nullptr;
        }
    }
    
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            if(occupiedPlaces[i][j] != nullptr)
            {
                delete occupiedPlaces[i][j];
                occupiedPlaces[i][j] = nullptr;
            }
        }
    }

    // WHITE ===================================================================================
    GenericPawnItem *pawn;
    // pawns
    for(int i=0; i < 8; i++)
    {
        pawn = new PawnItem(false, vec2(i, 1));
        occupiedPlaces[1][i] = pawn;
    }

    // rooks
    pawn = new RookItem(false, vec2(0, 0));
    occupiedPlaces[0][0] = pawn;
    pawn = new RookItem(false, vec2(7, 0));
    occupiedPlaces[0][7] = pawn;

    // knights
    pawn = new KnightItem(false, vec2(1, 0));
    occupiedPlaces[0][1] = pawn;
    pawn = new KnightItem(false, vec2(6, 0));
    occupiedPlaces[0][6] = pawn;

    // bishops
    pawn = new BishopItem(false, vec2(2, 0));
    occupiedPlaces[0][2] = pawn;
    pawn = new BishopItem(false, vec2(5, 0));
    occupiedPlaces[0][5] = pawn;

    // king
    pawn = new KingItem(false, vec2(3, 0));
    occupiedPlaces[0][3] = pawn;

    // queen
    pawn = new QueenItem(false, vec2(4, 0));
    occupiedPlaces[0][4] = pawn;

    // BLACK ===================================================================================
    // pawns
    for(int i=0; i < 8; i++)
    {
        pawn = new PawnItem(true, vec2(i, 6));
        occupiedPlaces[6][i] = pawn;
    }

    // rooks
    pawn = new RookItem(true, vec2(0, 7));
    occupiedPlaces[7][0] = pawn;
    pawn = new RookItem(true, vec2(7, 7));
    occupiedPlaces[7][7] = pawn;

    // knights
    pawn = new KnightItem(true, vec2(1, 7));
    occupiedPlaces[7][1] = pawn;
    pawn = new KnightItem(true, vec2(6, 7));
    occupiedPlaces[7][6] = pawn;

    // bishops
    pawn = new BishopItem(true, vec2(2, 7));
    occupiedPlaces[7][2] = pawn;
    pawn = new BishopItem(true, vec2(5, 7));
    occupiedPlaces[7][5] = pawn;

    // king
    pawn = new KingItem(true, vec2(3, 7));
    occupiedPlaces[7][3] = pawn;

    // queen
    pawn = new QueenItem(true, vec2(4, 7));
    occupiedPlaces[7][4] = pawn;
    // =========================================================================================
}

void GameWindow::mainLoop()
{
    GenericPawnItem* grabbedPawn = nullptr;
    SDL_Event event;
    
    while(!m_quit)
    {
        if(!paused_game)
        {
            // Screen clear
            SDL_SetRenderDrawColor(drawCanvas, 0, 0, 0, 0xFF);
            SDL_RenderClear(drawCanvas);
            //
        }

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                m_quit = true;
                break;
            }

            if (event.type == SDL_MOUSEMOTION)
            {
                if(grabbedPawn != nullptr)
                    grabbedPawn->mouseMoveEvent(vec2(event.motion.x, height() - event.motion.y));
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mp;
                mp.x = event.motion.x;
                mp.y = event.motion.y;
                SDL_Rect pr;
                pr.w = pr.h = CHECKER_S;
                pr.x = width()-CHECKER_S;
                pr.y = 0;
                
                if(SDL_PointInRect(&mp, &pr))
                {
                    restartGame();
                    continue;
                }

                SDL_Rect pl;
                pl.w = pl.h = CHECKER_S;
                pl.x = width() - CHECKER_S*2;
                pl.y = 0;

                if(SDL_PointInRect(&mp, &pl))
                {
                    showLeaderboard();
                    continue;
                }
                
                for(int i=0; i < 8; i++)
                {
                    for(int j=0; j < 8; j++)
                    {
                        if(occupiedPlaces[i][j] == nullptr)
                            continue;

                        pr.x = 100 + occupiedPlaces[i][j]->pos().x*CHECKER_S;
                        pr.y = height() - 100 - occupiedPlaces[i][j]->pos().y*CHECKER_S - CHECKER_S - 8;

                        if(SDL_PointInRect(&mp, &pr))
                        {
                            grabbedPawn = occupiedPlaces[i][j];
                            grabbedPawn->mousePressEvent();
                            break;
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (grabbedPawn != nullptr)
                    grabbedPawn->mouseReleaseEvent();
                grabbedPawn = nullptr;
            }
        }
        
        if(!paused_game)
        {
            draw_chessboard();
            draw_figs();
            draw_moves();
            
            // draw over other figs
            if (grabbedPawn != nullptr)
                grabbedPawn->draw(drawCanvas);

            //
            SDL_RenderPresent(drawCanvas);
            //
        }
    }
}

void GameWindow::checkKingUnderAttack()
{
    king_under_attack = false;
    KingItem *king = nullptr;
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            if(occupiedPlaces[i][j] == nullptr)
                continue;
            if(occupiedPlaces[i][j]->getBlack() == (currentTeam == BLACK_TEAM))
            {
                if(occupiedPlaces[i][j]->getFigure() == KING_FIG)
                {
                    king = (KingItem*)occupiedPlaces[i][j];
                    break;
                }
            }
        }
    }
    if(king == nullptr)
        return;
    // ============================================================================
    available_attacks=0;
    
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            if(occupiedPlaces[i][j] == nullptr)
                continue;
            if(occupiedPlaces[i][j]->getBlack() == (currentTeam == BLACK_TEAM))
                continue;

            if(canGoBy(occupiedPlaces[i][j], king->pos()))
            {
                if(occupiedPlaces[i][j]->canAttack(king->pos()))
                {
                    king_under_attack = true;
                    break;
                }
            }
        }
    }
}

void GameWindow::showMoves(GenericPawnItem *pawn)
{
    available_swaps = 0;
    hideMoves();
    //
    if(canSwap(pawn, true)) // longswap
        available_swaps |= 2;
    if(canSwap(pawn, false)) // shortswap
        available_swaps |= 1;
    //
    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {   
            if(!canGoBy(pawn, vec2(j, i)))
                continue;

            if(occupiedPlaces[i][j] != nullptr)
            {
                if(occupiedPlaces[i][j]->getBlack() == pawn->getBlack())
                    continue;
                else if(!pawn->canAttack(occupiedPlaces[i][j]->pos()))
                    continue;
                else
                    setAvailableAttack(j, i);
                continue;
            }

            if(pawn->canMove(vec2(j, i)))
                setAvailableMove(j, i);
        }
    }
}

bool GameWindow::canSwap(GenericPawnItem *pawn, bool longswap)
{
    if(pawn->getFigure() != KING_FIG)
        return false;
    if(pawn->getBlack() != (currentTeam == BLACK_TEAM))
        return false;
    if(pawn->moved())
        return false;
    bool b = pawn->getBlack();
    if(occupiedPlaces[b?7:0][(longswap?7:0)] == nullptr)
        return false;
    if(occupiedPlaces[b?7:0][(longswap?7:0)]->getFigure() != ROOK_FIG)
        return false;
    if(occupiedPlaces[b?7:0][(longswap?7:0)]->moved())
        return false;
    
    if(occupiedPlaces[b?7:0][(longswap?6:1)] != nullptr)
        return false;
    if(occupiedPlaces[b?7:0][(longswap?5:2)] != nullptr)
        return false;
    if(longswap)
    {
        if(occupiedPlaces[b?7:0][4] != nullptr)
            return false;
    }
    return true;
}

void GameWindow::doSwap(bool is_long)
{
    int k_sx = 3, k_sy = (currentTeam==BLACK_TEAM)?7:0;
    int k_tx = is_long?6:1, k_ty = (currentTeam==BLACK_TEAM)?7:0;
    changeOccupy(vec2(k_sx, k_sy), vec2(k_tx, k_ty));
    occupiedPlaces[k_ty][k_tx]->setPos(vec2(k_tx, k_ty));
    
    int r_sx = is_long?7:0, r_sy = (currentTeam==BLACK_TEAM)?7:0;
    int r_tx = is_long?5:2, r_ty = (currentTeam==BLACK_TEAM)?7:0;
    changeOccupy(vec2(r_sx, r_sy), vec2(r_tx, r_ty));
    occupiedPlaces[r_ty][r_tx]->setPos(vec2(r_tx, r_ty));
}

void GameWindow::setAvailableMove(int c, int n)
{
    available_moves |= (1LL << (c + n*8));
}

void GameWindow::clearAvailableMove(int c, int n)
{
    available_moves = available_moves & ( (~0LL)^(1LL << (c+n*8)) );
}

void GameWindow::setAvailableAttack(int c, int n)
{
    available_attacks |= (1LL << (c + n * 8));
}

void GameWindow::clearAvailableAttack(int c, int n)
{
    available_attacks = available_attacks & ((~0LL) ^ (1LL << (c + n * 8)));
}

void GameWindow::hideMoves()
{
    available_moves = 0LL;
    available_attacks = 0LL;
    available_swaps = 0;
}

CHESS_TEAM GameWindow::getTeam() const
{
    return currentTeam;
}

void GameWindow::changeOccupy(const vec2 &_old, const vec2 &_new)
{
    int opx = _old.x, opy = _old.y;
    int npx = _new.x, npy = _new.y;
    occupiedPlaces[npy][npx] = occupiedPlaces[opy][opx];
    occupiedPlaces[opy][opx] = nullptr;
}

void GameWindow::checkPawnMorph()
{
    bool b = (currentTeam==WHITE_TEAM);
    vec2 pos = vec2(0,1);
    int py = b?7:0;
    for(int i=0; i < 8; i++)
    {
        if(occupiedPlaces[py][i] == nullptr)
            continue;
        if(occupiedPlaces[py][i]->getFigure() == PAWN_FIG)
        {
            pos = vec2(i, py);
            break;
        }
    }
    if(pos.y == 1)
        return;

    hideMoves();
    SDL_RenderClear(drawCanvas);
    draw_chessboard();
    draw_figs();
    SDL_RenderPresent(drawCanvas);
    
    //
    SDL_Event ev;
    bool selected = false;
    SDL_Rect t, f, currentRect;
    t.w = t.h = CHECKER_S;
    f.w = CHECKER_S*4;
    f.h = CHECKER_S;
    
    //
    t.y = b?36:(100+10*CHECKER_S);
    f.y = b?36:(100+10*CHECKER_S);
    //

    currentRect.w = currentRect.h = CHECKER_S;
    currentRect.x = 100+pos.x*CHECKER_S;
    currentRect.y = f.y;
    
    int chosen = 0;
    do
    {
        SDL_SetRenderDrawColor(drawCanvas, 0, 0, 0, 0xFF);
        SDL_RenderClear(drawCanvas);
        
        draw_chessboard();
        draw_figs();
        draw_moves();
        // backdrop
        f.x = 100 + pos.x*CHECKER_S;
        SDL_SetRenderDrawColor(drawCanvas, 0x01, 0x32, 0x20, 0xFF);
        SDL_RenderFillRect(drawCanvas, &f);
        //
        
        while(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_MOUSEMOTION)
            {
                SDL_Point mp;
                mp.x = ev.motion.x;
                mp.y = ev.motion.y;
                SDL_Rect pr;
                pr.y = f.y;
                pr.w = pr.h = CHECKER_S;
                
                for(int i=0; i < 4; i++)
                {
                    pr.x = 100 + (pos.x+i)*CHECKER_S;
                    if(SDL_PointInRect(&mp, &pr))
                    {
                        chosen = i;
                        currentRect.x = pr.x;
                        currentRect.w = currentRect.h = CHECKER_S;
                        break;
                    }
                }
            }
            else if(ev.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mp;
                mp.x = ev.motion.x;
                mp.y = ev.motion.y;
                if(SDL_PointInRect(&mp, &currentRect))
                {
                    delete occupiedPlaces[pos.y][pos.x];
                    assert(chosen < 4);
                    if(chosen == 0)
                        occupiedPlaces[pos.y][pos.x] = new KnightItem(!b, vec2(pos.x, pos.y));
                    else if(chosen == 1)
                        occupiedPlaces[pos.y][pos.x] = new RookItem(!b, vec2(pos.x, pos.y));
                    else if(chosen == 2)
                        occupiedPlaces[pos.y][pos.x] = new BishopItem(!b, vec2(pos.x, pos.y));
                    else if(chosen == 3)
                        occupiedPlaces[pos.y][pos.x] = new QueenItem(!b, vec2(pos.x, pos.y));
                    //
                    selected = true;
                    break;
                }
            }
        }
        //
        SDL_SetRenderDrawColor(drawCanvas, 180, 150, 0, 0xFF);
        SDL_RenderFillRect(drawCanvas, &currentRect);
                        
        t.x = 100 + pos.x*CHECKER_S;
        SDL_RenderCopy(drawCanvas, b?blackKnightTex:whiteKnightTex, NULL, &t);
        t.x = 100 + (pos.x+1)*CHECKER_S;
        SDL_RenderCopy(drawCanvas, b?blackRookTex:whiteRookTex, NULL, &t);
        t.x = 100 + (pos.x+2)*CHECKER_S;
        SDL_RenderCopy(drawCanvas, b?blackBishopTex:whiteBishopTex, NULL, &t);
        t.x = 100 + (pos.x+3)*CHECKER_S;
        SDL_RenderCopy(drawCanvas, b?blackQueenTex:whiteQueenTex, NULL, &t);
        //
        SDL_RenderPresent(drawCanvas);
    } while(!selected);
    //
}

void GameWindow::endMove()
{
    checkPawnMorph();
    //
    if (currentTeam == WHITE_TEAM)
    {
        ++whiteTurns;
        currentTeam = BLACK_TEAM;
    }
    else
    {
        ++blackTurns;
        currentTeam = WHITE_TEAM;
    }
    checkKingUnderAttack();
}

void GameWindow::killFigure(const vec2 &pos)
{
    int px = pos.x, py = pos.y;
    if(occupiedPlaces[py][px] != nullptr)
    {
        if(occupiedPlaces[py][px]->getFigure() == KING_FIG)
        {
            king_under_attack = true;
            for(int i=0; i < 8; i++)
            {
                for(int j=0; j < 8; j++)
                {
                    if(occupiedPlaces[i][j] != nullptr)
                        occupiedPlaces[i][j]->die();
                }
            }
            
            draw_chessboard();
            SDL_Rect t;
            t.x = width()/2 - 320;
            t.y = height()/2 - 128;
            t.w = 640;
            t.h = 256;
            bool blackLost = occupiedPlaces[py][px]->getBlack();
            SDL_RenderCopy(drawCanvas, blackLost?blackLostTex:whiteLostTex, NULL, &t);
            SDL_RenderPresent(drawCanvas);
            paused_game = true;

            // INSERT new record
            if(baseHandler)
            {
                char query[1024];
                char *errMsg;
                sprintf(query, "INSERT INTO results (side,turns,time) VALUES(\"%s\",%d,%ld);",
                        (blackLost?"white":"black"),
                        (blackLost?whiteTurns:blackTurns)+1, // + current turn
                        time(nullptr)-start_time);
                int r = sqlite3_exec(baseHandler,
                                     query,
                                     GameWindow::sqlCallback,
                                     NULL,
                                     &errMsg);
                if(r != SQLITE_OK)
                {
                    fprintf(stderr, "SQL error: %s\n", errMsg);
                    sqlite3_free(errMsg);
                }
            }
            // =================================================================================
            return;
        }
        deadPawns[deadCount++] = occupiedPlaces[py][px];
        if(occupiedPlaces[py][px]->getBlack())
        {
            occupiedPlaces[py][px]->setPos(vec2(9+bdp - 8*(bdp>8), 7-1*(bdp>=8)));
            bdp++;
        }
        else
        {
            occupiedPlaces[py][px]->setPos(vec2(9+wdp - 8*(wdp>8), 1*(wdp>=8)));
            wdp++;
        }
        occupiedPlaces[py][px]->die();
        occupiedPlaces[py][px] = nullptr;
    }
}

bool GameWindow::canAttack(bool col, const vec2 &pos)
{
    int py = pos.y, px = pos.x;
    if (occupiedPlaces[py][px] == nullptr)
        return false;

    if (occupiedPlaces[py][px]->getBlack() != col)
        return true;
    return false;
}

bool GameWindow::canMove(const vec2 &pos)
{
    int py = pos.y, px = pos.x;
    if (occupiedPlaces[py][px] == nullptr)
        return true;
    return false;
}

bool GameWindow::canGoBy(GenericPawnItem *pawn, const vec2 &pos)
{
    const CHESS_FIGURE &fig = pawn->getFigure();

    if(fig == PAWN_FIG)
    {
        vec2 a = pawn->getOldPos() - pos;
        if(abs(a.y) > 1)
        {
            bool b = pawn->getBlack();
            if(pawn->getOldPos().y == (b?6:1))
            {
                if(occupiedPlaces[pawn->getOldPos().y+1*(b?-1:1)][pos.x] != nullptr)
                    return false;
            }
        }
    }
    else if(fig == ROOK_FIG)
    {
        vec2 d;
        if(pawn->getOldPos().x > pos.x)
            d = vec2(-1,0);
        else if(pawn->getOldPos().x < pos.x)
            d = vec2(1,0);
        else if(pawn->getOldPos().y > pos.y)
            d = vec2(0,-1);
        else if(pawn->getOldPos().y < pos.y)
            d = vec2(0,1);
        else
            return false;

        for(vec2 p = pawn->getOldPos()+d; p != pos; p += d)
        {
            if(occupiedPlaces[p.y][p.x] != nullptr)
                return false;
        }
    }
    else if(fig == BISHOP_FIG)
    {
        vec2 d;
        if(pawn->getOldPos().x > pos.x)
            d = vec2(-1,0);
        else if(pawn->getOldPos().x < pos.x)
            d = vec2(1,0);

        if(pawn->getOldPos().y > pos.y)
            d += vec2(0,-1);
        else if(pawn->getOldPos().y < pos.y)
            d += vec2(0,1);
        else
            return false;

        for(vec2 p = pawn->getOldPos()+d; p != pos; p += d)
        {
            if(occupiedPlaces[p.y][p.x] != nullptr)
                return false;
        }
    }
    else if(fig == QUEEN_FIG)
    {
        vec2 d = pos - pawn->getOldPos();
        if((d.x!=0.f) ^ (d.y != 0.f))
        {
            if(pawn->getOldPos().x > pos.x)
                d = vec2(-1,0);
            else if(pawn->getOldPos().x < pos.x)
                d = vec2(1,0);
            else if(pawn->getOldPos().y > pos.y)
                d = vec2(0,-1);
            else if(pawn->getOldPos().y < pos.y)
                d = vec2(0,1);
            else
                return false;
        }
        else
        {
            if(pawn->getOldPos().x > pos.x)
                d = vec2(-1,0);
            else if(pawn->getOldPos().x < pos.x)
                d = vec2(1,0);

            if(pawn->getOldPos().y > pos.y)
                d += vec2(0,-1);
            else if(pawn->getOldPos().y < pos.y)
                d += vec2(0,1);
            else
                return false;
        }

        for(vec2 p = pawn->getOldPos()+d; p != pos; p += d)
        {
            if(occupiedPlaces[p.y][p.x] != nullptr)
                return false;
        }
    }
    
    return true;
}

uint16_t GameWindow::width() const
{
    return w_width;
}

uint16_t GameWindow::height() const
{
    return w_height;
}

void GameWindow::showLeaderboard()
{
    SDL_Event ev;
    bool quitLeaders = false;
    SDL_Rect boardRect;
    boardRect.x = width()>>2;
    boardRect.y = height()>>2;
    boardRect.w = width()>>1;
    boardRect.h = height()>>1;

    SDL_Rect textOrigin;
    textOrigin.x = boardRect.x + 8;
    textOrigin.y = boardRect.y + 8;
    textOrigin.w = boardRect.w;
    textOrigin.h = 12;

    int animIter = 0;
    char *errMsg;
    std::vector<std::string> leaderBoards;
    int r = sqlite3_exec(baseHandler,
                         "SELECT side,turns,time FROM results;",
                         GameWindow::sqlCallback,
                         &leaderBoards,
                         &errMsg);
    if(r != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
        return;
    }

    size_t startIter = 0;
    const size_t stopIter = (leaderBoards.size() < MAX_TEXT_ROWS ? leaderBoards.size() : MAX_TEXT_ROWS);
    do
    {
        SDL_SetRenderDrawColor(drawCanvas, 0, 0, 0, 0xFF);
        SDL_RenderClear(drawCanvas);

        draw_chessboard();
        draw_figs();
        draw_moves();

        while(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_KEYDOWN)
            {
                if(ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    quitLeaders = true;
                    break;
                }
            }
            else if(ev.type == SDL_QUIT)
            {
                quitLeaders = true;
                m_quit = true;
                break;
            }
        }

        // Draw leaderboard
        SDL_RenderCopy(drawCanvas, leaderboardTex, NULL, &boardRect);

        stringRGBA(drawCanvas,
                   textOrigin.x, textOrigin.y,
                   "Leaderboard",
                   0xcc, 0xcc, 0xff, 0xff);

        if(animIter >= 90) // 1.5 second
        {
            animIter = 0;
            if(leaderBoards.size() >= MAX_TEXT_ROWS)
            {
                if((leaderBoards.size()-startIter) == stopIter)
                    startIter = 0;
                else
                    startIter++;
            }
        }

        for(size_t i = 0; i < stopIter; i++)
        {
            stringRGBA(drawCanvas,
                       textOrigin.x, textOrigin.y+textOrigin.h*(i+1),
                       leaderBoards[i+startIter].c_str(),
                       0xcc, 0xcc, 0xff, 0xff);
        }
        //

        SDL_RenderPresent(drawCanvas);
        ++animIter;
    } while(!quitLeaders);
}

int GameWindow::sqlCallback(void *userPtr, int argc, char **argv, char **colName)
{
    if(argc != 3) // just ignore
        return 0;
    (void)colName;
    // this is callback for "SELECT side,turns,time FROM results
    // we will hardcode there
    std::vector<std::string> *leaderLines = (std::vector<std::string>*)userPtr;
    char line[256];
//        printf("%s | %s | %s\n", argv[i], argv[i+1], argv[i+2]);
    argv[0][0] = toupper(argv[0][0]);
    sprintf(line, "%4ld | %s won in %2s turns (%3s seconds elapsed)",
            leaderLines->size(),
            argv[0],
            argv[1],
            argv[2]);
    leaderLines->push_back(line);
    return 0;
}
