#include "../headers/structures.h"
#include "../headers/consts.h"


int checkCollision(Hitbox player, Hitbox enemy) {
    // zwraca 0 jezeli nie nachodza na siebie
    if (player.x + player.w < enemy.x) {
        return 0;
    } else if ( player.x > enemy.x + enemy.w) {
        return 0;
    } else if ( player.y + player.h < enemy.y) {
        return 0;
    } else if (player.y > enemy.y + enemy.h){
        return 0;
    };// zwraca 1 jezeli nachodza --- zwykly bool
    return 1; 
}