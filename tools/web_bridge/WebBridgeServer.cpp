#include <algorithm>
#include <arpa/inet.h>
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

static const double GRAVITY = 1480.0;
static const double MAX_FALL = 760.0;
static const double WALK_SPEED = 315.0;
static const double RUN_ACCEL = 2450.0;
static const double RUN_FRICTION = 3100.0;
static const double JUMP_SPEED = -805.0;
static const double FLOAT_FALL = 82.0;
static const double WORLD_HEIGHT = 650.0;

struct Rect {
    double x;
    double y;
    double w;
    double h;
};

struct Player {
    double x;
    double y;
    double w;
    double h;
    double vx;
    double vy;
    double targetVx;
    int facing;
    bool onGround;
    double coyoteTime;
    int health;
    int maxHealth;
    int score;
    std::string state;
    std::string ability;
    double abilityTime;
    double floatTime;
    double inhaleTime;
    double fireCooldown;
    double invuln;
};

struct Enemy {
    double x;
    double y;
    double w;
    double h;
    double vx;
    double vy;
    std::string type;
    std::string ability;
    int health;
    int maxHealth;
    bool active;
    bool onGround;
    double patrolLeft;
    double patrolRight;
    double hurtTimer;
    double shotTimer;
    double minionTimer;
};

struct Food {
    double x;
    double y;
    double w;
    double h;
    std::string type;
    bool active;
    double bob;
};

struct Projectile {
    double x;
    double y;
    double w;
    double h;
    double vx;
    double vy;
    int damage;
    std::string type;
    bool active;
};

struct LevelData {
    std::string name;
    double width;
    double goalX;
    std::vector<Rect> platforms;
    std::vector<Enemy> enemies;
    std::vector<Food> foods;
    bool hasBoss;
    Enemy boss;
};

struct GameState {
    bool running;
    bool gameOver;
    bool won;
    bool aiMode;
    int levelIndex;
    LevelData level;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Food> foods;
    std::vector<Projectile> projectiles;
    std::vector<Projectile> bossShots;
    double cameraX;
    double itemTimer;
    double aiDecisionTimer;
    double aiTargetX;
    double levelBannerTimer;
};

struct InputState {
    bool left;
    bool right;
    bool jump;
    bool inhale;
    bool stopInhale;
    bool fire;
    bool quit;
};

static pthread_mutex_t gameMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static GameState game;
static InputState inputState;
static bool serverRunning = true;

static double randomDouble(double minValue, double maxValue) {
    double t = (double)std::rand() / (double)RAND_MAX;
    return minValue + (maxValue - minValue) * t;
}

static bool overlaps(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh) {
    return ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by;
}

static bool overlaps(const Rect& a, const Rect& b) {
    return overlaps(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h);
}

template <typename Actor>
static bool overlapsActor(const Actor& a, const Rect& b) {
    return overlaps(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h);
}

template <typename A, typename B>
static bool overlapsActors(const A& a, const B& b) {
    return overlaps(a.x, a.y, a.w, a.h, b.x, b.y, b.w, b.h);
}

static Player createPlayer(int score = 0, int health = 5) {
    Player player;
    player.x = 90;
    player.y = 420;
    player.w = 54;
    player.h = 48;
    player.vx = 0;
    player.vy = 0;
    player.targetVx = 0;
    player.facing = 1;
    player.onGround = false;
    player.coyoteTime = 0;
    player.health = health;
    player.maxHealth = 5;
    player.score = score;
    player.state = "normal";
    player.ability = "none";
    player.abilityTime = 0;
    player.floatTime = 0;
    player.inhaleTime = 0;
    player.fireCooldown = 0;
    player.invuln = 0;
    return player;
}

static Enemy makeEnemy(double x, double y, const std::string& type) {
    bool fire = type == "fire";
    Enemy enemy;
    enemy.x = x;
    enemy.y = y;
    enemy.w = fire ? 54 : 48;
    enemy.h = 44;
    enemy.vx = fire ? -70 : -55;
    enemy.vy = 0;
    enemy.type = type;
    enemy.ability = fire ? "fire" : "star";
    enemy.health = fire ? 2 : 1;
    enemy.maxHealth = enemy.health;
    enemy.active = true;
    enemy.onGround = false;
    enemy.patrolLeft = x - 190;
    enemy.patrolRight = x + 190;
    enemy.hurtTimer = 0;
    enemy.shotTimer = 0;
    enemy.minionTimer = 0;
    return enemy;
}

static Food makeFood(double x, double y, const std::string& type) {
    Food food;
    food.x = x;
    food.y = y;
    food.w = 34;
    food.h = 34;
    food.type = type;
    food.active = true;
    food.bob = randomDouble(0, 6.28);
    return food;
}

static Enemy makeBoss(double x, double y) {
    Enemy boss = makeEnemy(x, y, "boss");
    boss.w = 128;
    boss.h = 120;
    boss.vx = -95;
    boss.health = 28;
    boss.maxHealth = 28;
    boss.ability = "none";
    boss.shotTimer = 1.4;
    boss.minionTimer = 4.8;
    return boss;
}

static std::vector<LevelData> createLevels() {
    std::vector<LevelData> levels;

    LevelData level1;
    level1.name = "Pradera Dulce";
    level1.width = 3220;
    level1.goalX = 3010;
    level1.hasBoss = false;
    level1.platforms = {
        {0, 540, 950, 120}, {1110, 540, 820, 120}, {2100, 540, 1120, 120},
        {330, 388, 250, 28}, {820, 315, 250, 28}, {1250, 388, 250, 28},
        {1730, 300, 240, 28}, {2310, 370, 260, 28}, {2730, 305, 240, 28}
    };
    level1.enemies = {
        makeEnemy(620, 470, "normal"),
        makeEnemy(1320, 470, "fire"),
        makeEnemy(2320, 470, "normal")
    };
    level1.foods = {
        makeFood(420, 340, "cake"),
        makeFood(1460, 340, "coin"),
        makeFood(2460, 492, "tomato")
    };
    levels.push_back(level1);

    LevelData level2;
    level2.name = "Colinas Brillantes";
    level2.width = 3420;
    level2.goalX = 3210;
    level2.hasBoss = false;
    level2.platforms = {
        {0, 540, 620, 120}, {820, 540, 380, 120}, {1480, 540, 420, 120},
        {2380, 540, 1040, 120}, {260, 420, 230, 28}, {900, 345, 260, 28},
        {1420, 410, 230, 28}, {1980, 330, 250, 28}, {2580, 410, 260, 28},
        {2940, 350, 230, 28}
    };
    level2.enemies = {
        makeEnemy(380, 470, "normal"),
        makeEnemy(1000, 470, "normal"),
        makeEnemy(1700, 470, "fire"),
        makeEnemy(2680, 470, "normal")
    };
    level2.foods = {
        makeFood(360, 372, "coin"),
        makeFood(1040, 296, "cake"),
        makeFood(2750, 362, "tomato")
    };
    levels.push_back(level2);

    LevelData bossLevel;
    bossLevel.name = "Jefe final";
    bossLevel.width = 1860;
    bossLevel.goalX = 1760;
    bossLevel.hasBoss = true;
    bossLevel.platforms = {
        {0, 540, 1860, 120}, {420, 390, 240, 28}, {930, 350, 260, 28},
        {1330, 405, 260, 28}
    };
    bossLevel.foods = {
        makeFood(500, 342, "tomato"),
        makeFood(1160, 302, "cake")
    };
    bossLevel.boss = makeBoss(1420, 420);
    levels.push_back(bossLevel);

    return levels;
}

static void loadLevel(int index, const Player* keepPlayer = nullptr) {
    std::vector<LevelData> levels = createLevels();
    game.levelIndex = index;
    game.level = levels[index];
    game.enemies = game.level.enemies;
    game.enemies.reserve(64);
    game.foods = game.level.foods;
    game.projectiles.clear();
    game.bossShots.clear();
    game.cameraX = 0;
    game.itemTimer = 0;
    game.levelBannerTimer = 2.2;

    if (game.level.hasBoss) {
        game.enemies.push_back(game.level.boss);
    }

    int score = keepPlayer ? keepPlayer->score : 0;
    int health = keepPlayer ? keepPlayer->health : 5;
    std::string ability = keepPlayer ? keepPlayer->ability : "none";
    double abilityTime = keepPlayer ? keepPlayer->abilityTime : 0;
    game.player = createPlayer(score, health);
    game.player.ability = ability;
    game.player.abilityTime = abilityTime;
}

static void startGame(bool aiMode) {
    pthread_mutex_lock(&gameMutex);
    game.running = true;
    game.gameOver = false;
    game.won = false;
    game.aiMode = aiMode;
    game.aiDecisionTimer = 0;
    game.aiTargetX = 400;
    loadLevel(0);
    pthread_mutex_unlock(&gameMutex);
}

static void finishGame(bool won) {
    game.running = false;
    game.gameOver = true;
    game.won = won;
}

static void grantAbility(const std::string& ability) {
    game.player.ability = ability;
    game.player.abilityTime = ability == "fire" ? 18.0 : 22.5;
}

static void damagePlayer(int amount) {
    Player& player = game.player;
    if (player.invuln > 0 || game.gameOver) {
        return;
    }

    player.health -= amount;
    player.invuln = 1.1;
    player.vy = -360;
    player.x -= player.facing * 34;

    if (player.health <= 0) {
        finishGame(false);
    }
}

static void jumpOrFloat() {
    Player& player = game.player;
    if (player.onGround || player.coyoteTime > 0) {
        player.vy = JUMP_SPEED;
        player.onGround = false;
        player.coyoteTime = 0;
        player.state = "normal";
    } else if (player.floatTime <= 0) {
        player.floatTime = 1.55;
        player.state = "floating";
    }
}

static void tryAbsorb() {
    Player& player = game.player;
    player.state = "inhaling";
    player.inhaleTime = 0.35;

    for (Enemy& enemy : game.enemies) {
        if (!enemy.active || enemy.type == "boss") {
            continue;
        }

        bool inFront = player.facing > 0
            ? enemy.x >= player.x
            : enemy.x + enemy.w <= player.x + player.w;
        double distance = std::abs((enemy.x + enemy.w / 2) - (player.x + player.w / 2));
        double vertical = std::abs((enemy.y + enemy.h / 2) - (player.y + player.h / 2));

        if (inFront && distance < 130 && vertical < 64) {
            enemy.active = false;
            player.score += 150;
            grantAbility(enemy.ability);
            return;
        }
    }
}

static void fireProjectile() {
    Player& player = game.player;
    if (player.ability == "none" || player.fireCooldown > 0) {
        return;
    }

    bool fire = player.ability == "fire";
    Projectile projectile;
    projectile.x = player.facing > 0 ? player.x + player.w - 6 : player.x - 24;
    projectile.y = player.y + 18;
    projectile.w = fire ? 34 : 26;
    projectile.h = fire ? 20 : 24;
    projectile.vx = player.facing * (fire ? 620 : 520);
    projectile.vy = 0;
    projectile.damage = fire ? 2 : 1;
    projectile.type = player.ability;
    projectile.active = true;
    game.projectiles.push_back(projectile);
    player.fireCooldown = fire ? 0.32 : 0.24;
}

template <typename Actor>
static void moveActor(Actor& actor, double dt) {
    double previousX = actor.x;
    double previousY = actor.y;

    actor.x += actor.vx * dt;
    for (const Rect& platform : game.level.platforms) {
        if (overlapsActor(actor, platform)) {
            if (actor.vx > 0 && previousX + actor.w <= platform.x) {
                actor.x = platform.x - actor.w;
            } else if (actor.vx < 0 && previousX >= platform.x + platform.w) {
                actor.x = platform.x + platform.w;
            }
        }
    }

    actor.vy = std::max(-900.0, std::min(MAX_FALL, actor.vy + GRAVITY * dt));
    actor.y += actor.vy * dt;
    actor.onGround = false;

    for (const Rect& platform : game.level.platforms) {
        if (!overlapsActor(actor, platform)) {
            continue;
        }

        double previousBottom = previousY + actor.h;
        if (actor.vy >= 0 && previousBottom <= platform.y + 8) {
            actor.y = platform.y - actor.h;
            actor.vy = 0;
            actor.onGround = true;
        } else if (actor.vy < 0 && previousY >= platform.y + platform.h - 8) {
            actor.y = platform.y + platform.h;
            actor.vy = 0;
        }
    }

    if (actor.x < 0) actor.x = 0;
    if (actor.x > game.level.width - actor.w) actor.x = game.level.width - actor.w;
}

static void updateComputerInput(double dt, InputState& input) {
    Player& player = game.player;
    Enemy* nearestEnemy = nullptr;
    Food* nearestFood = nullptr;
    double nearestEnemyDistance = 999999;
    double nearestFoodDistance = 999999;

    for (Enemy& enemy : game.enemies) {
        if (!enemy.active) continue;
        double distance = std::abs(enemy.x - player.x);
        if (distance < nearestEnemyDistance) {
            nearestEnemyDistance = distance;
            nearestEnemy = &enemy;
        }
    }

    for (Food& food : game.foods) {
        if (!food.active) continue;
        double distance = std::abs(food.x - player.x);
        if (distance < nearestFoodDistance) {
            nearestFoodDistance = distance;
            nearestFood = &food;
        }
    }

    game.aiDecisionTimer -= dt;
    if (game.aiDecisionTimer <= 0) {
        bool wantsFood = nearestFood != nullptr && player.health < 5 && randomDouble(0, 1) < 0.55;
        game.aiTargetX = wantsFood ? nearestFood->x : player.x + 420 + randomDouble(0, 420);
        if (nearestEnemy != nullptr && nearestEnemyDistance < 360 && randomDouble(0, 1) < 0.55) {
            game.aiTargetX = nearestEnemy->x;
        }
        game.aiDecisionTimer = 0.55 + randomDouble(0, 0.85);
    }

    player.targetVx = 0;
    if (nearestEnemy != nullptr && nearestEnemyDistance < 130) {
        if (player.ability != "none" && player.fireCooldown <= 0 && randomDouble(0, 1) < 0.7) {
            player.facing = nearestEnemy->x > player.x ? 1 : -1;
            input.fire = true;
        } else if (randomDouble(0, 1) < 0.45) {
            player.facing = nearestEnemy->x > player.x ? 1 : -1;
            input.inhale = true;
        } else {
            player.targetVx = nearestEnemy->x > player.x ? -WALK_SPEED : WALK_SPEED;
            player.facing = player.targetVx > 0 ? 1 : -1;
        }
    } else if (player.x < game.aiTargetX - 12) {
        player.targetVx = WALK_SPEED;
        player.facing = 1;
    } else if (player.x > game.aiTargetX + 12) {
        player.targetVx = -WALK_SPEED;
        player.facing = -1;
    }

    double aheadX = player.x + player.facing * 90;
    bool hasGroundAhead = false;
    for (const Rect& platform : game.level.platforms) {
        if (aheadX > platform.x && aheadX < platform.x + platform.w &&
            player.y + player.h <= platform.y + 16 && player.y + player.h >= platform.y - 90) {
            hasGroundAhead = true;
            break;
        }
    }

    if ((player.onGround && (!hasGroundAhead || randomDouble(0, 1) < 0.012)) ||
        (nearestEnemy != nullptr && nearestEnemyDistance < 240 && randomDouble(0, 1) < 0.02)) {
        input.jump = true;
    }
}

static void updatePlayer(double dt, InputState input) {
    Player& player = game.player;

    if (game.aiMode) {
        updateComputerInput(dt, input);
    } else {
        player.targetVx = 0;
        if (input.left) {
            player.targetVx = -WALK_SPEED;
            player.facing = -1;
        }
        if (input.right) {
            player.targetVx = WALK_SPEED;
            player.facing = 1;
        }
    }

    if (input.jump) jumpOrFloat();
    if (input.inhale) tryAbsorb();
    if (input.stopInhale) {
        player.state = "normal";
        player.inhaleTime = 0;
    }
    if (input.fire) fireProjectile();

    double acceleration = std::abs(player.targetVx) > 0 ? RUN_ACCEL : RUN_FRICTION;
    if (player.vx < player.targetVx) {
        player.vx = std::min(player.targetVx, player.vx + acceleration * dt);
    } else if (player.vx > player.targetVx) {
        player.vx = std::max(player.targetVx, player.vx - acceleration * dt);
    }

    if (player.floatTime > 0 && !player.onGround) {
        player.floatTime -= dt;
        player.state = "floating";
        if (player.vy > FLOAT_FALL) {
            player.vy = FLOAT_FALL;
        }
    } else if (player.state == "floating") {
        player.state = "normal";
    }

    moveActor(player, dt);

    if (player.onGround) {
        player.coyoteTime = 0.12;
    } else {
        player.coyoteTime = std::max(0.0, player.coyoteTime - dt);
    }

    if (player.inhaleTime > 0) {
        player.inhaleTime -= dt;
        if (player.inhaleTime <= 0 && player.state == "inhaling") {
            player.state = "normal";
        }
    }

    if (player.abilityTime > 0) {
        player.abilityTime -= dt;
        if (player.abilityTime <= 0) {
            player.ability = "none";
            player.abilityTime = 0;
        }
    }

    player.fireCooldown = std::max(0.0, player.fireCooldown - dt);
    player.invuln = std::max(0.0, player.invuln - dt);

    if (player.y > WORLD_HEIGHT + 160) {
        player.health -= 1;
        if (player.health <= 0) {
            finishGame(false);
            return;
        }
        player.x = std::max(80.0, game.cameraX + 90);
        player.y = 350;
        player.vx = 0;
        player.vy = 0;
        player.invuln = 1.4;
    }
}

static void updateBoss(Enemy& boss, double dt) {
    Player& player = game.player;
    boss.x += boss.vx * dt;
    if (boss.x < 760) {
        boss.x = 760;
        boss.vx = std::abs(boss.vx);
    }
    if (boss.x > game.level.width - 220) {
        boss.x = game.level.width - 220;
        boss.vx = -std::abs(boss.vx);
    }

    boss.y = 408 + std::sin((double)std::clock() / 180000.0) * 18;
    boss.shotTimer -= dt;
    boss.minionTimer -= dt;
    boss.hurtTimer = std::max(0.0, boss.hurtTimer - dt);

    if (boss.shotTimer <= 0) {
        double direction = player.x < boss.x ? -1 : 1;
        Projectile shot;
        shot.x = boss.x + boss.w / 2;
        shot.y = boss.y + 55;
        shot.w = 28;
        shot.h = 28;
        shot.vx = direction * 310;
        shot.vy = -80;
        shot.damage = 1;
        shot.type = "boss";
        shot.active = true;
        game.bossShots.push_back(shot);
        boss.shotTimer = std::max(0.55, 1.15 - (1 - (double)boss.health / boss.maxHealth) * 0.45);
    }

    int activeMinions = 0;
    for (const Enemy& enemy : game.enemies) {
        if (enemy.active && enemy.type != "boss") activeMinions++;
    }
    if (boss.minionTimer <= 0 && activeMinions < 3) {
        game.enemies.push_back(makeEnemy(boss.x - 180, 470, randomDouble(0, 1) < 0.35 ? "fire" : "normal"));
        boss.minionTimer = 5.5;
    }

    if (overlapsActors(player, boss)) {
        damagePlayer(2);
    }
}

static void updateEnemies(double dt) {
    Player& player = game.player;
    size_t initialSize = game.enemies.size();
    for (size_t i = 0; i < initialSize && i < game.enemies.size(); i++) {
        Enemy& enemy = game.enemies[i];
        if (!enemy.active) continue;

        if (enemy.type == "boss") {
            updateBoss(enemy, dt);
            continue;
        }

        double distance = player.x - enemy.x;
        if (std::abs(distance) < 420) {
            enemy.vx = (distance >= 0 ? 1 : -1) * (enemy.type == "fire" ? 105 : 85);
        } else {
            if (enemy.x < enemy.patrolLeft) enemy.vx = std::abs(enemy.vx);
            if (enemy.x > enemy.patrolRight) enemy.vx = -std::abs(enemy.vx);
        }

        moveActor(enemy, dt);
        enemy.hurtTimer = std::max(0.0, enemy.hurtTimer - dt);

        if (overlapsActors(player, enemy)) {
            damagePlayer(enemy.type == "fire" ? 2 : 1);
        }
    }
}

static void updateProjectiles(double dt) {
    for (Projectile& projectile : game.projectiles) {
        if (!projectile.active) continue;
        projectile.x += projectile.vx * dt;

        if (projectile.x < -80 || projectile.x > game.level.width + 80) {
            projectile.active = false;
            continue;
        }

        for (Enemy& enemy : game.enemies) {
            if (!enemy.active || !overlapsActors(projectile, enemy)) {
                continue;
            }

            enemy.health -= projectile.damage;
            enemy.hurtTimer = 0.18;
            projectile.active = false;

            if (enemy.health <= 0) {
                enemy.active = false;
                game.player.score += enemy.type == "boss" ? 1800 : 100;
                if (enemy.type == "boss") {
                    finishGame(true);
                }
            } else {
                game.player.score += 40;
            }
            break;
        }
    }

    for (Projectile& shot : game.bossShots) {
        if (!shot.active) continue;
        shot.vy += 280 * dt;
        shot.x += shot.vx * dt;
        shot.y += shot.vy * dt;

        if (shot.x < -80 || shot.x > game.level.width + 80 || shot.y > WORLD_HEIGHT + 120) {
            shot.active = false;
        }
        if (shot.active && overlapsActors(shot, game.player)) {
            shot.active = false;
            damagePlayer(1);
        }
    }

    game.projectiles.erase(
        std::remove_if(game.projectiles.begin(), game.projectiles.end(), [](const Projectile& p) { return !p.active; }),
        game.projectiles.end()
    );
    game.bossShots.erase(
        std::remove_if(game.bossShots.begin(), game.bossShots.end(), [](const Projectile& p) { return !p.active; }),
        game.bossShots.end()
    );
}

static Rect platformAtRandom() {
    std::vector<Rect> candidates;
    for (const Rect& platform : game.level.platforms) {
        if (platform.w > 180) candidates.push_back(platform);
    }
    if (candidates.empty()) return game.level.platforms[0];
    return candidates[std::rand() % candidates.size()];
}

static void spawnRandomItem() {
    int activeFoods = 0;
    for (const Food& food : game.foods) {
        if (food.active) activeFoods++;
    }
    if (activeFoods >= 6 || (game.levelIndex == 2 && activeFoods >= 3)) {
        return;
    }

    Rect platform = platformAtRandom();
    double roll = randomDouble(0, 1);
    std::string type = roll < 0.45 ? "cake" : (roll < 0.75 ? "coin" : "tomato");
    game.foods.push_back(makeFood(
        platform.x + 40 + randomDouble(0, std::max(40.0, platform.w - 90)),
        platform.y - 40,
        type
    ));
}

static void updateFoods(double dt) {
    for (Food& food : game.foods) {
        if (!food.active) continue;
        food.bob += dt * 5;
        if (overlapsActors(game.player, food)) {
            food.active = false;
            if (food.type == "coin") {
                game.player.score += 80;
            } else {
                int heal = food.type == "tomato" ? 2 : 1;
                game.player.health = std::min(game.player.maxHealth, game.player.health + heal);
                game.player.score += food.type == "tomato" ? 70 : 45;
            }
        }
    }

    game.itemTimer -= dt;
    if (game.itemTimer <= 0) {
        spawnRandomItem();
        game.itemTimer = 3.2 + randomDouble(0, 2.4);
    }
}

static void updateCamera() {
    double viewportWidth = 960;
    double target = game.player.x - viewportWidth * 0.34;
    double maxCamera = std::max(0.0, game.level.width - viewportWidth);
    game.cameraX += (std::max(0.0, std::min(maxCamera, target)) - game.cameraX) * 0.14;
}

static void updateGame(double dt) {
    if (!game.running || game.gameOver) {
        return;
    }

    InputState input;
    pthread_mutex_lock(&inputMutex);
    input = inputState;
    inputState.jump = false;
    inputState.stopInhale = false;
    inputState.fire = false;
    inputState.quit = false;
    pthread_mutex_unlock(&inputMutex);

    if (input.quit) {
        finishGame(false);
        return;
    }

    game.levelBannerTimer = std::max(0.0, game.levelBannerTimer - dt);
    updatePlayer(dt, input);
    if (game.gameOver) return;
    updateEnemies(dt);
    updateProjectiles(dt);
    updateFoods(dt);
    updateCamera();

    if (game.levelIndex < 2 && game.player.x >= game.level.goalX) {
        int nextLevel = game.levelIndex + 1;
        Player previousPlayer = game.player;
        loadLevel(nextLevel, &previousPlayer);
    }
}

static void* gameThread(void*) {
    while (serverRunning) {
        pthread_mutex_lock(&gameMutex);
        updateGame(1.0 / 60.0);
        pthread_mutex_unlock(&gameMutex);
        usleep(16666);
    }
    return nullptr;
}

static std::string jsonEscape(const std::string& value) {
    std::ostringstream out;
    for (char c : value) {
        if (c == '"' || c == '\\') {
            out << '\\' << c;
        } else if (c == '\n') {
            out << "\\n";
        } else {
            out << c;
        }
    }
    return out.str();
}

static void appendRect(std::ostringstream& out, const Rect& rect) {
    out << "{\"x\":" << rect.x << ",\"y\":" << rect.y
        << ",\"w\":" << rect.w << ",\"h\":" << rect.h << "}";
}

static void appendPlayer(std::ostringstream& out, const Player& player) {
    out << "{\"x\":" << player.x << ",\"y\":" << player.y
        << ",\"w\":" << player.w << ",\"h\":" << player.h
        << ",\"vx\":" << player.vx << ",\"vy\":" << player.vy
        << ",\"targetVx\":" << player.targetVx
        << ",\"facing\":" << player.facing
        << ",\"onGround\":" << (player.onGround ? "true" : "false")
        << ",\"coyoteTime\":" << player.coyoteTime
        << ",\"health\":" << player.health << ",\"maxHealth\":" << player.maxHealth
        << ",\"score\":" << player.score
        << ",\"state\":\"" << jsonEscape(player.state) << "\""
        << ",\"ability\":\"" << jsonEscape(player.ability) << "\""
        << ",\"abilityTime\":" << player.abilityTime
        << ",\"floatTime\":" << player.floatTime
        << ",\"inhaleTime\":" << player.inhaleTime
        << ",\"fireCooldown\":" << player.fireCooldown
        << ",\"invuln\":" << player.invuln << "}";
}

static void appendEnemy(std::ostringstream& out, const Enemy& enemy) {
    out << "{\"x\":" << enemy.x << ",\"y\":" << enemy.y
        << ",\"w\":" << enemy.w << ",\"h\":" << enemy.h
        << ",\"vx\":" << enemy.vx << ",\"vy\":" << enemy.vy
        << ",\"type\":\"" << jsonEscape(enemy.type) << "\""
        << ",\"ability\":\"" << jsonEscape(enemy.ability) << "\""
        << ",\"health\":" << enemy.health << ",\"maxHealth\":" << enemy.maxHealth
        << ",\"active\":" << (enemy.active ? "true" : "false")
        << ",\"onGround\":" << (enemy.onGround ? "true" : "false")
        << ",\"patrolLeft\":" << enemy.patrolLeft << ",\"patrolRight\":" << enemy.patrolRight
        << ",\"hurtTimer\":" << enemy.hurtTimer
        << ",\"shotTimer\":" << enemy.shotTimer
        << ",\"minionTimer\":" << enemy.minionTimer << "}";
}

static void appendFood(std::ostringstream& out, const Food& food) {
    out << "{\"x\":" << food.x << ",\"y\":" << food.y
        << ",\"w\":" << food.w << ",\"h\":" << food.h
        << ",\"type\":\"" << jsonEscape(food.type) << "\""
        << ",\"active\":" << (food.active ? "true" : "false")
        << ",\"bob\":" << food.bob << "}";
}

static void appendProjectile(std::ostringstream& out, const Projectile& projectile) {
    out << "{\"x\":" << projectile.x << ",\"y\":" << projectile.y
        << ",\"w\":" << projectile.w << ",\"h\":" << projectile.h
        << ",\"vx\":" << projectile.vx << ",\"vy\":" << projectile.vy
        << ",\"damage\":" << projectile.damage
        << ",\"type\":\"" << jsonEscape(projectile.type) << "\""
        << ",\"active\":" << (projectile.active ? "true" : "false") << "}";
}

static std::string snapshotJson() {
    pthread_mutex_lock(&gameMutex);
    std::ostringstream out;
    out << "{\"backend\":\"kirby-cpp\"";
    out << ",\"screen\":\"" << (game.gameOver ? "gameover" : (game.running ? "playing" : "menu")) << "\"";
    out << ",\"mode\":\"" << (game.aiMode ? "ai" : "player") << "\"";
    out << ",\"finishedWon\":" << (game.won ? "true" : "false");
    out << ",\"levelIndex\":" << game.levelIndex;
    out << ",\"cameraX\":" << game.cameraX;
    out << ",\"levelBannerTimer\":" << game.levelBannerTimer;

    out << ",\"level\":{\"name\":\"" << jsonEscape(game.level.name)
        << "\",\"width\":" << game.level.width
        << ",\"goalX\":" << game.level.goalX
        << ",\"platforms\":[";
    for (size_t i = 0; i < game.level.platforms.size(); i++) {
        if (i) out << ",";
        appendRect(out, game.level.platforms[i]);
    }
    out << "]}";

    out << ",\"player\":";
    appendPlayer(out, game.player);

    out << ",\"enemies\":[";
    for (size_t i = 0; i < game.enemies.size(); i++) {
        if (i) out << ",";
        appendEnemy(out, game.enemies[i]);
    }
    out << "]";

    out << ",\"foods\":[";
    for (size_t i = 0; i < game.foods.size(); i++) {
        if (i) out << ",";
        appendFood(out, game.foods[i]);
    }
    out << "]";

    out << ",\"projectiles\":[";
    for (size_t i = 0; i < game.projectiles.size(); i++) {
        if (i) out << ",";
        appendProjectile(out, game.projectiles[i]);
    }
    out << "]";

    out << ",\"bossShots\":[";
    for (size_t i = 0; i < game.bossShots.size(); i++) {
        if (i) out << ",";
        appendProjectile(out, game.bossShots[i]);
    }
    out << "]}";
    pthread_mutex_unlock(&gameMutex);
    return out.str();
}

static std::string readFile(const std::string& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file) return "";
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
}

static std::string contentType(const std::string& path) {
    if (path.find(".html") != std::string::npos) return "text/html; charset=utf-8";
    if (path.find(".css") != std::string::npos) return "text/css; charset=utf-8";
    if (path.find(".js") != std::string::npos) return "application/javascript; charset=utf-8";
    return "text/plain; charset=utf-8";
}

static void sendResponse(int client, const std::string& body, const std::string& type, int status = 200) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << (status == 200 ? " OK" : " Not Found") << "\r\n";
    response << "Content-Type: " << type << "\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "Connection: close\r\n\r\n";
    response << body;
    std::string text = response.str();
    send(client, text.c_str(), text.size(), 0);
}

static std::map<std::string, std::string> parseQuery(const std::string& rawPath) {
    std::map<std::string, std::string> query;
    size_t question = rawPath.find('?');
    if (question == std::string::npos) return query;

    std::string text = rawPath.substr(question + 1);
    std::stringstream stream(text);
    std::string pair;
    while (std::getline(stream, pair, '&')) {
        size_t equals = pair.find('=');
        if (equals == std::string::npos) {
            query[pair] = "1";
        } else {
            query[pair.substr(0, equals)] = pair.substr(equals + 1);
        }
    }
    return query;
}

static std::string cleanPath(const std::string& rawPath) {
    size_t question = rawPath.find('?');
    std::string path = question == std::string::npos ? rawPath : rawPath.substr(0, question);
    if (path == "/") return "/index.html";
    return path;
}

static bool truthy(const std::map<std::string, std::string>& query, const std::string& key) {
    std::map<std::string, std::string>::const_iterator found = query.find(key);
    return found != query.end() && found->second != "0" && found->second != "false";
}

static void handleRequest(int client, const std::string& request) {
    std::stringstream stream(request);
    std::string method;
    std::string rawPath;
    stream >> method >> rawPath;

    if (method == "OPTIONS") {
        sendResponse(client, "", "text/plain; charset=utf-8");
        return;
    }

    std::string path = cleanPath(rawPath);
    std::map<std::string, std::string> query = parseQuery(rawPath);

    if (path == "/api/state") {
        sendResponse(client, snapshotJson(), "application/json; charset=utf-8");
        return;
    }

    if (path == "/api/start") {
        bool aiMode = query["mode"] == "ai";
        startGame(aiMode);
        sendResponse(client, snapshotJson(), "application/json; charset=utf-8");
        return;
    }

    if (path == "/api/input") {
        pthread_mutex_lock(&inputMutex);
        inputState.left = truthy(query, "left");
        inputState.right = truthy(query, "right");
        inputState.inhale = truthy(query, "inhale");
        inputState.jump = inputState.jump || truthy(query, "jump");
        inputState.stopInhale = inputState.stopInhale || truthy(query, "stop");
        inputState.fire = inputState.fire || truthy(query, "fire");
        inputState.quit = inputState.quit || truthy(query, "quit");
        pthread_mutex_unlock(&inputMutex);
        sendResponse(client, "{\"ok\":true}", "application/json; charset=utf-8");
        return;
    }

    std::string filePath = "web" + path;
    std::string body = readFile(filePath);
    if (body.empty()) {
        sendResponse(client, "Not found", "text/plain; charset=utf-8", 404);
        return;
    }

    sendResponse(client, body, contentType(filePath));
}

static void* clientThread(void* arg) {
    int client = *(int*)arg;
    delete (int*)arg;

    char buffer[8192];
    std::memset(buffer, 0, sizeof(buffer));
    ssize_t received = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        handleRequest(client, std::string(buffer, received));
    }
    close(client);
    return nullptr;
}

static void stopServer(int) {
    serverRunning = false;
}

int main() {
    std::srand((unsigned int)std::time(nullptr));
    std::signal(SIGINT, stopServer);
    game.running = false;
    game.gameOver = false;
    game.won = false;
    game.aiMode = false;
    loadLevel(0);

    pthread_t loopThread;
    pthread_create(&loopThread, nullptr, gameThread, nullptr);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        std::cerr << "No se pudo crear el socket.\n";
        return 1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8080);

    if (bind(server, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "No se pudo abrir http://127.0.0.1:8080. Puede que el puerto este ocupado.\n";
        close(server);
        return 1;
    }

    if (listen(server, 16) < 0) {
        std::cerr << "No se pudo escuchar conexiones.\n";
        close(server);
        return 1;
    }

    std::cout << "Kirby C++ web server listo en http://127.0.0.1:8080\n";

    while (serverRunning) {
        sockaddr_in clientAddress;
        socklen_t clientLength = sizeof(clientAddress);
        int client = accept(server, (sockaddr*)&clientAddress, &clientLength);
        if (client < 0) {
            continue;
        }

        pthread_t thread;
        int* clientPtr = new int(client);
        pthread_create(&thread, nullptr, clientThread, clientPtr);
        pthread_detach(thread);
    }

    close(server);
    pthread_join(loopThread, nullptr);
    return 0;
}
