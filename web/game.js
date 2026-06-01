const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d");

const overlays = {
    menu: document.getElementById("menuOverlay"),
    instructions: document.getElementById("instructionsOverlay"),
    scores: document.getElementById("scoresOverlay"),
    gameOver: document.getElementById("gameOverOverlay"),
    pause: document.getElementById("pauseOverlay")
};

const hud = {
    health: document.getElementById("hudHealth"),
    score: document.getElementById("hudScore"),
    level: document.getElementById("hudLevel"),
    ability: document.getElementById("hudAbility"),
    boss: document.getElementById("hudBoss"),
    mode: document.getElementById("hudMode")
};

const scoreList = document.getElementById("scoreList");
const toast = document.getElementById("toast");
const nameInput = document.getElementById("playerName");
const gameOverTitle = document.getElementById("gameOverTitle");
const gameOverScore = document.getElementById("gameOverScore");
const gameOverEyebrow = document.getElementById("gameOverEyebrow");

const STORE_KEY = "kirbyGraphicScores";
const GRAVITY = 1480;
const MAX_FALL = 760;
const WALK_SPEED = 315;
const RUN_ACCEL = 2450;
const RUN_FRICTION = 3100;
const JUMP_SPEED = -805;
const FLOAT_FALL = 82;
const WORLD_HEIGHT = 650;

const keys = new Set();
const pressed = new Set();

let view = { w: 960, h: 540, dpr: 1 };
let lastTime = performance.now();
let toastTimer = null;

const state = {
    screen: "menu",
    mode: "player",
    levelIndex: 0,
    level: null,
    player: null,
    enemies: [],
    foods: [],
    projectiles: [],
    particles: [],
    bossShots: [],
    cameraX: 0,
    scoreSaved: false,
    finishedWon: false,
    levelBannerTimer: 0,
    itemTimer: 0,
    aiDecisionTimer: 0,
    aiTargetX: 400
};

const backend = {
    baseUrl: window.location.protocol === "file:" ? "http://127.0.0.1:8080" : "",
    connected: false,
    stateInFlight: false,
    inputInFlight: false,
    lastPoll: 0,
    gameOverShown: false
};

function resizeCanvas() {
    const rect = canvas.getBoundingClientRect();
    view.dpr = Math.max(1, Math.min(window.devicePixelRatio || 1, 2));
    view.w = Math.max(320, rect.width);
    view.h = Math.max(420, rect.height);
    canvas.width = Math.floor(view.w * view.dpr);
    canvas.height = Math.floor(view.h * view.dpr);
    ctx.setTransform(view.dpr, 0, 0, view.dpr, 0, 0);
}

window.addEventListener("resize", resizeCanvas);
resizeCanvas();

function showOverlay(name) {
    Object.values(overlays).forEach((overlay) => overlay.classList.remove("is-visible"));
    if (name && overlays[name]) {
        overlays[name].classList.add("is-visible");
    }
}

function showToast(message) {
    toast.textContent = message;
    toast.classList.add("is-visible");
    clearTimeout(toastTimer);
    toastTimer = setTimeout(() => toast.classList.remove("is-visible"), 2200);
}

async function backendFetch(path) {
    const response = await fetch(`${backend.baseUrl}${path}`, { cache: "no-store" });
    if (!response.ok) {
        throw new Error(`Backend HTTP ${response.status}`);
    }
    return response.json();
}

function applyBackendSnapshot(snapshot) {
    if (!snapshot || snapshot.backend !== "kirby-cpp") {
        return;
    }

    state.mode = snapshot.mode;
    state.levelIndex = snapshot.levelIndex;
    state.level = snapshot.level;
    state.player = snapshot.player;
    state.enemies = snapshot.enemies || [];
    state.foods = snapshot.foods || [];
    state.projectiles = snapshot.projectiles || [];
    state.bossShots = snapshot.bossShots || [];
    state.cameraX = snapshot.cameraX || 0;
    state.levelBannerTimer = snapshot.levelBannerTimer || 0;
    state.finishedWon = !!snapshot.finishedWon;

    if (snapshot.screen === "playing") {
        state.screen = "playing";
        backend.gameOverShown = false;
    }

    if (snapshot.screen === "gameover" && !backend.gameOverShown) {
        backend.gameOverShown = true;
        finishGame(!!snapshot.finishedWon);
    }
}

async function detectBackend() {
    try {
        const snapshot = await backendFetch("/api/state");
        backend.connected = snapshot.backend === "kirby-cpp";
        if (backend.connected) {
            applyBackendSnapshot(snapshot);
            showToast("Frontend conectado al backend C++.");
        }
    } catch {
        backend.connected = false;
    }
}

function clamp(value, min, max) {
    return Math.max(min, Math.min(max, value));
}

function rectsOverlap(a, b) {
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

function createLevels() {
    return [
        {
            name: "Pradera Dulce",
            width: 3220,
            goalX: 3010,
            platforms: [
                { x: 0, y: 540, w: 950, h: 120 },
                { x: 1110, y: 540, w: 820, h: 120 },
                { x: 2100, y: 540, w: 1120, h: 120 },
                { x: 330, y: 388, w: 250, h: 28 },
                { x: 820, y: 315, w: 250, h: 28 },
                { x: 1250, y: 388, w: 250, h: 28 },
                { x: 1730, y: 300, w: 240, h: 28 },
                { x: 2310, y: 370, w: 260, h: 28 },
                { x: 2730, y: 305, w: 240, h: 28 }
            ],
            enemies: [
                { x: 620, y: 470, type: "normal" },
                { x: 1320, y: 470, type: "fire" },
                { x: 2320, y: 470, type: "normal" }
            ],
            foods: [
                { x: 420, y: 340, type: "cake" },
                { x: 1460, y: 340, type: "coin" },
                { x: 2460, y: 492, type: "tomato" }
            ]
        },
        {
            name: "Colinas Brillantes",
            width: 3420,
            goalX: 3210,
            platforms: [
                { x: 0, y: 540, w: 620, h: 120 },
                { x: 820, y: 540, w: 380, h: 120 },
                { x: 1480, y: 540, w: 420, h: 120 },
                { x: 2380, y: 540, w: 1040, h: 120 },
                { x: 260, y: 420, w: 230, h: 28 },
                { x: 900, y: 345, w: 260, h: 28 },
                { x: 1420, y: 410, w: 230, h: 28 },
                { x: 1980, y: 330, w: 250, h: 28 },
                { x: 2580, y: 410, w: 260, h: 28 },
                { x: 2940, y: 350, w: 230, h: 28 }
            ],
            enemies: [
                { x: 380, y: 470, type: "normal" },
                { x: 1000, y: 470, type: "normal" },
                { x: 1700, y: 470, type: "fire" },
                { x: 2680, y: 470, type: "normal" }
            ],
            foods: [
                { x: 360, y: 372, type: "coin" },
                { x: 1040, y: 296, type: "cake" },
                { x: 2750, y: 362, type: "tomato" }
            ]
        },
        {
            name: "Jefe final",
            width: 1860,
            goalX: 1760,
            platforms: [
                { x: 0, y: 540, w: 1860, h: 120 },
                { x: 420, y: 390, w: 240, h: 28 },
                { x: 930, y: 350, w: 260, h: 28 },
                { x: 1330, y: 405, w: 260, h: 28 }
            ],
            enemies: [],
            foods: [
                { x: 500, y: 342, type: "tomato" },
                { x: 1160, y: 302, type: "cake" }
            ],
            boss: { x: 1420, y: 420 }
        }
    ];
}

function createPlayer(keepScore = 0, keepHealth = 5) {
    return {
        x: 90,
        y: 420,
        w: 54,
        h: 48,
        vx: 0,
        vy: 0,
        targetVx: 0,
        facing: 1,
        onGround: false,
        coyoteTime: 0,
        health: keepHealth,
        maxHealth: 5,
        score: keepScore,
        state: "normal",
        floatTime: 0,
        inhaleTime: 0,
        ability: "none",
        abilityTime: 0,
        fireCooldown: 0,
        invuln: 0
    };
}

function makeEnemy(template) {
    const fire = template.type === "fire";
    return {
        x: template.x,
        y: template.y,
        w: fire ? 54 : 48,
        h: 44,
        vx: fire ? -70 : -55,
        vy: 0,
        type: template.type,
        health: fire ? 2 : 1,
        maxHealth: fire ? 2 : 1,
        active: true,
        onGround: false,
        patrolLeft: template.x - 190,
        patrolRight: template.x + 190,
        ability: fire ? "fire" : "star",
        hurtTimer: 0
    };
}

function makeFood(template) {
    return {
        x: template.x,
        y: template.y,
        w: 34,
        h: 34,
        type: template.type,
        active: true,
        bob: Math.random() * Math.PI * 2
    };
}

function makeBoss(template) {
    return {
        x: template.x,
        y: template.y,
        w: 128,
        h: 120,
        vx: -95,
        vy: 0,
        health: 28,
        maxHealth: 28,
        active: true,
        shotTimer: 1.4,
        minionTimer: 4.8,
        hurtTimer: 0
    };
}

function loadLevel(index, keepPlayer = null) {
    const levels = createLevels();
    state.levelIndex = index;
    state.level = levels[index];
    state.enemies = state.level.enemies.map(makeEnemy);
    state.foods = state.level.foods.map(makeFood);
    state.projectiles = [];
    state.bossShots = [];
    state.particles = [];
    state.itemTimer = 0;
    state.levelBannerTimer = 2.2;
    state.cameraX = 0;

    if (state.level.boss) {
        state.enemies.push({ ...makeBoss(state.level.boss), type: "boss" });
    }

    const score = keepPlayer ? keepPlayer.score : 0;
    const health = keepPlayer ? keepPlayer.health : 5;
    const ability = keepPlayer ? keepPlayer.ability : "none";
    const abilityTime = keepPlayer ? keepPlayer.abilityTime : 0;
    state.player = createPlayer(score, health);
    state.player.ability = ability;
    state.player.abilityTime = abilityTime;

    showToast(index === 2 ? "El jefe final aparecio." : `Nivel ${index + 1}: ${state.level.name}`);
}

function startGame(mode) {
    if (backend.connected) {
        startBackendGame(mode);
        return;
    }

    state.mode = mode;
    state.screen = "playing";
    state.scoreSaved = false;
    state.finishedWon = false;
    state.aiDecisionTimer = 0;
    loadLevel(0);
    showOverlay(null);
}

async function startBackendGame(mode) {
    try {
        const snapshot = await backendFetch(`/api/start?mode=${encodeURIComponent(mode)}`);
        state.mode = mode;
        state.screen = "playing";
        state.scoreSaved = false;
        state.finishedWon = false;
        backend.gameOverShown = false;
        applyBackendSnapshot(snapshot);
        showOverlay(null);
        showToast("La logica ahora corre desde C++.");
    } catch {
        backend.connected = false;
        showToast("No se encontro backend C++; usando modo web local.");
        startGame(mode);
    }
}

function finishGame(won) {
    state.screen = "gameover";
    state.finishedWon = won;
    state.scoreSaved = false;
    gameOverEyebrow.textContent = won ? "Victoria" : "Partida terminada";
    gameOverTitle.textContent = won ? "Jefe derrotado" : "Game Over";
    gameOverScore.textContent = `Score: ${state.player.score}`;
    nameInput.value = "";
    showOverlay("gameOver");
}

function saveCurrentScore() {
    if (state.scoreSaved) {
        return;
    }

    const name = (nameInput.value || "INVITADO").trim().slice(0, 16) || "INVITADO";
    const scores = getScores();
    scores.push({
        name,
        score: state.player.score,
        date: new Date().toISOString(),
        result: state.finishedWon ? "Victoria" : "Derrota"
    });
    scores.sort((a, b) => b.score - a.score);
    localStorage.setItem(STORE_KEY, JSON.stringify(scores.slice(0, 10)));
    state.scoreSaved = true;
}

function getScores() {
    try {
        return JSON.parse(localStorage.getItem(STORE_KEY) || "[]");
    } catch {
        return [];
    }
}

function renderScores() {
    const scores = getScores();
    scoreList.innerHTML = "";

    if (scores.length === 0) {
        const empty = document.createElement("li");
        empty.textContent = "Aun no hay puntajes guardados.";
        scoreList.appendChild(empty);
        return;
    }

    scores.forEach((entry) => {
        const row = document.createElement("li");
        row.textContent = `${entry.name} - ${entry.score} (${entry.result})`;
        scoreList.appendChild(row);
    });
}

function actionFromButton(action) {
    if (action === "start-player") startGame("player");
    if (action === "start-ai") startGame("ai");
    if (action === "instructions") showOverlay("instructions");
    if (action === "scores") {
        renderScores();
        showOverlay("scores");
    }
    if (action === "menu") {
        state.screen = "menu";
        showOverlay("menu");
    }
    if (action === "resume") {
        state.screen = "playing";
        showOverlay(null);
    }
    if (action === "save-restart") {
        saveCurrentScore();
        startGame(state.mode);
    }
    if (action === "save-menu") {
        saveCurrentScore();
        state.screen = "menu";
        showOverlay("menu");
    }
    if (action === "clear-scores") {
        localStorage.removeItem(STORE_KEY);
        renderScores();
    }
}

document.addEventListener("click", (event) => {
    const button = event.target.closest("[data-action]");
    if (button) {
        actionFromButton(button.dataset.action);
    }
});

window.addEventListener("keydown", (event) => {
    const key = event.key.toLowerCase();
    keys.add(key);
    pressed.add(key);

    if ([" ", "arrowup", "arrowdown", "arrowleft", "arrowright"].includes(key)) {
        event.preventDefault();
    }

    if (key === "p" && state.screen === "playing") {
        state.screen = "paused";
        showOverlay("pause");
    } else if (key === "p" && state.screen === "paused") {
        state.screen = "playing";
        showOverlay(null);
    }

    if (key === "q" && state.screen === "playing" && backend.connected) {
        fetch(`${backend.baseUrl}/api/input?quit=1`, { method: "POST", cache: "no-store" }).catch(() => {});
        state.screen = "menu";
        showOverlay("menu");
    } else if (key === "q" && state.screen === "playing") {
        state.screen = "menu";
        showOverlay("menu");
    }
});

window.addEventListener("keyup", (event) => {
    keys.delete(event.key.toLowerCase());
});

function consumePress(key) {
    if (!pressed.has(key)) {
        return false;
    }
    pressed.delete(key);
    return true;
}

function buildBackendInputQuery() {
    const jump = consumePress("w") || consumePress("arrowup") || consumePress(" ");
    const stop = consumePress("h");
    const fire = consumePress("k");
    const quit = consumePress("q");

    return new URLSearchParams({
        left: keys.has("a") || keys.has("arrowleft") ? "1" : "0",
        right: keys.has("d") || keys.has("arrowright") ? "1" : "0",
        jump: jump ? "1" : "0",
        inhale: keys.has("j") ? "1" : "0",
        stop: stop ? "1" : "0",
        fire: fire ? "1" : "0",
        quit: quit ? "1" : "0"
    }).toString();
}

function updateBackend() {
    if (!backend.connected || state.screen !== "playing") {
        return;
    }

    if (!backend.inputInFlight) {
        backend.inputInFlight = true;
        const query = buildBackendInputQuery();
        fetch(`${backend.baseUrl}/api/input?${query}`, { method: "POST", cache: "no-store" })
            .catch(() => {
                backend.connected = false;
                showToast("Se perdio conexion con C++; usando modo web local.");
            })
            .finally(() => {
                backend.inputInFlight = false;
            });
    }

    const now = performance.now();
    if (!backend.stateInFlight && now - backend.lastPoll > 32) {
        backend.lastPoll = now;
        backend.stateInFlight = true;
        backendFetch("/api/state")
            .then(applyBackendSnapshot)
            .catch(() => {
                backend.connected = false;
                showToast("Se perdio conexion con C++; usando modo web local.");
            })
            .finally(() => {
                backend.stateInFlight = false;
            });
    }
}

function getAbilityName(ability) {
    if (ability === "star") return "Estrella";
    if (ability === "fire") return "Fuego";
    return "Ninguna";
}

function grantAbility(ability) {
    state.player.ability = ability;
    state.player.abilityTime = ability === "fire" ? 18 : 22.5;
    showToast(`Kirby copio ${getAbilityName(ability)}.`);
}

function damagePlayer(amount) {
    const player = state.player;
    if (player.invuln > 0) {
        return;
    }

    player.health -= amount;
    player.invuln = 1.1;
    player.vy = -360;
    player.x -= player.facing * 34;
    burst(player.x + player.w / 2, player.y + player.h / 2, "#f05ca8", 14);

    if (player.health <= 0) {
        finishGame(false);
    }
}

function burst(x, y, color, count) {
    for (let i = 0; i < count; i++) {
        const angle = Math.random() * Math.PI * 2;
        const speed = 80 + Math.random() * 220;
        state.particles.push({
            x,
            y,
            vx: Math.cos(angle) * speed,
            vy: Math.sin(angle) * speed,
            life: 0.45 + Math.random() * 0.35,
            maxLife: 0.8,
            color
        });
    }
}

function fireProjectile() {
    const player = state.player;
    if (player.ability === "none" || player.fireCooldown > 0) {
        if (player.ability === "none") {
            showToast("Kirby no tiene habilidad copiada.");
        }
        return;
    }

    const fire = player.ability === "fire";
    state.projectiles.push({
        x: player.facing > 0 ? player.x + player.w - 6 : player.x - 24,
        y: player.y + 18,
        w: fire ? 34 : 26,
        h: fire ? 20 : 24,
        vx: player.facing * (fire ? 620 : 520),
        damage: fire ? 2 : 1,
        type: player.ability,
        active: true
    });
    player.fireCooldown = fire ? 0.32 : 0.24;
}

function tryAbsorb() {
    const player = state.player;
    player.state = "inhaling";
    player.inhaleTime = 0.35;

    for (const enemy of state.enemies) {
        if (!enemy.active || enemy.type === "boss") {
            continue;
        }

        const inFront = player.facing > 0
            ? enemy.x >= player.x
            : enemy.x + enemy.w <= player.x + player.w;
        const distance = Math.abs((enemy.x + enemy.w / 2) - (player.x + player.w / 2));
        const vertical = Math.abs((enemy.y + enemy.h / 2) - (player.y + player.h / 2));

        if (inFront && distance < 130 && vertical < 64) {
            enemy.active = false;
            player.score += 150;
            grantAbility(enemy.ability);
            burst(enemy.x + enemy.w / 2, enemy.y + enemy.h / 2, "#ffd166", 18);
            return;
        }
    }
}

function jumpOrFloat() {
    const player = state.player;
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

function platformAtRandom() {
    const platforms = state.level.platforms.filter((platform) => platform.w > 180);
    return platforms[Math.floor(Math.random() * platforms.length)];
}

function spawnRandomItem() {
    const activeFoods = state.foods.filter((food) => food.active).length;
    if (activeFoods >= 6 || state.levelIndex === 2 && activeFoods >= 3) {
        return;
    }

    const platform = platformAtRandom();
    const typeRoll = Math.random();
    const type = typeRoll < 0.45 ? "cake" : typeRoll < 0.75 ? "coin" : "tomato";
    state.foods.push(makeFood({
        x: platform.x + 40 + Math.random() * Math.max(40, platform.w - 90),
        y: platform.y - 40,
        type
    }));
}

function updatePlayerControls(dt) {
    const player = state.player;

    if (state.mode === "ai") {
        updateComputerInput(dt);
    } else {
        player.targetVx = 0;
        if (keys.has("a") || keys.has("arrowleft")) {
            player.targetVx = -WALK_SPEED;
            player.facing = -1;
        }
        if (keys.has("d") || keys.has("arrowright")) {
            player.targetVx = WALK_SPEED;
            player.facing = 1;
        }
        if (consumePress("w") || consumePress("arrowup") || consumePress(" ")) {
            jumpOrFloat();
        }
        if (keys.has("j")) {
            tryAbsorb();
        }
        if (consumePress("h")) {
            player.state = "normal";
            player.inhaleTime = 0;
        }
        if (consumePress("k")) {
            fireProjectile();
        }
    }
}

function updateComputerInput(dt) {
    const player = state.player;
    const activeEnemies = state.enemies.filter((enemy) => enemy.active);
    const nearestEnemy = activeEnemies
        .filter((enemy) => enemy.type !== "boss" || enemy.health > 0)
        .sort((a, b) => Math.abs(a.x - player.x) - Math.abs(b.x - player.x))[0];
    const nearestFood = state.foods
        .filter((food) => food.active)
        .sort((a, b) => Math.abs(a.x - player.x) - Math.abs(b.x - player.x))[0];

    state.aiDecisionTimer -= dt;
    if (state.aiDecisionTimer <= 0) {
        const wantsFood = nearestFood && player.health < 5 && Math.random() < 0.55;
        state.aiTargetX = wantsFood ? nearestFood.x : player.x + 420 + Math.random() * 420;
        if (nearestEnemy && Math.abs(nearestEnemy.x - player.x) < 360 && Math.random() < 0.55) {
            state.aiTargetX = nearestEnemy.x;
        }
        state.aiDecisionTimer = 0.55 + Math.random() * 0.85;
    }

    player.targetVx = 0;
    if (nearestEnemy && Math.abs(nearestEnemy.x - player.x) < 130) {
        if (player.ability !== "none" && player.fireCooldown <= 0 && Math.random() < 0.7) {
            player.facing = nearestEnemy.x > player.x ? 1 : -1;
            fireProjectile();
        } else if (Math.random() < 0.45) {
            player.facing = nearestEnemy.x > player.x ? 1 : -1;
            tryAbsorb();
        } else {
            player.targetVx = nearestEnemy.x > player.x ? -WALK_SPEED : WALK_SPEED;
            player.facing = player.targetVx > 0 ? 1 : -1;
        }
    } else if (player.x < state.aiTargetX - 12) {
        player.targetVx = WALK_SPEED;
        player.facing = 1;
    } else if (player.x > state.aiTargetX + 12) {
        player.targetVx = -WALK_SPEED;
        player.facing = -1;
    }

    const aheadX = player.x + player.facing * 90;
    const hasGroundAhead = state.level.platforms.some((platform) => {
        return aheadX > platform.x && aheadX < platform.x + platform.w &&
            player.y + player.h <= platform.y + 16 && player.y + player.h >= platform.y - 90;
    });

    if ((player.onGround && (!hasGroundAhead || Math.random() < 0.012)) ||
        (nearestEnemy && Math.abs(nearestEnemy.x - player.x) < 240 && Math.random() < 0.02)) {
        jumpOrFloat();
    }
}

function moveActor(actor, dt) {
    const previousX = actor.x;
    const previousY = actor.y;

    actor.x += actor.vx * dt;
    for (const platform of state.level.platforms) {
        if (rectsOverlap(actor, platform)) {
            if (actor.vx > 0 && previousX + actor.w <= platform.x) {
                actor.x = platform.x - actor.w;
            } else if (actor.vx < 0 && previousX >= platform.x + platform.w) {
                actor.x = platform.x + platform.w;
            }
        }
    }

    actor.vy = clamp(actor.vy + GRAVITY * dt, -900, MAX_FALL);
    actor.y += actor.vy * dt;
    actor.onGround = false;

    for (const platform of state.level.platforms) {
        if (!rectsOverlap(actor, platform)) {
            continue;
        }

        const previousBottom = previousY + actor.h;
        if (actor.vy >= 0 && previousBottom <= platform.y + 8) {
            actor.y = platform.y - actor.h;
            actor.vy = 0;
            actor.onGround = true;
        } else if (actor.vy < 0 && previousY >= platform.y + platform.h - 8) {
            actor.y = platform.y + platform.h;
            actor.vy = 0;
        }
    }

    actor.x = clamp(actor.x, 0, state.level.width - actor.w);
}

function updatePlayer(dt) {
    const player = state.player;
    updatePlayerControls(dt);

    const acceleration = Math.abs(player.targetVx) > 0 ? RUN_ACCEL : RUN_FRICTION;
    if (player.vx < player.targetVx) {
        player.vx = Math.min(player.targetVx, player.vx + acceleration * dt);
    } else if (player.vx > player.targetVx) {
        player.vx = Math.max(player.targetVx, player.vx - acceleration * dt);
    }

    if (player.floatTime > 0 && !player.onGround) {
        player.floatTime -= dt;
        player.state = "floating";
        if (player.vy > FLOAT_FALL) {
            player.vy = FLOAT_FALL;
        }
    } else if (player.state === "floating") {
        player.state = "normal";
    }

    moveActor(player, dt);

    if (player.onGround) {
        player.coyoteTime = 0.12;
    } else {
        player.coyoteTime = Math.max(0, player.coyoteTime - dt);
    }

    if (player.inhaleTime > 0) {
        player.inhaleTime -= dt;
        if (player.inhaleTime <= 0 && player.state === "inhaling") {
            player.state = "normal";
        }
    }

    if (player.abilityTime > 0) {
        player.abilityTime -= dt;
        if (player.abilityTime <= 0) {
            player.ability = "none";
            player.abilityTime = 0;
            showToast("La habilidad se acabo.");
        }
    }

    player.fireCooldown = Math.max(0, player.fireCooldown - dt);
    player.invuln = Math.max(0, player.invuln - dt);

    if (player.y > WORLD_HEIGHT + 160) {
        player.health -= 1;
        if (player.health <= 0) {
            finishGame(false);
            return;
        }
        player.x = Math.max(80, state.cameraX + 90);
        player.y = 350;
        player.vx = 0;
        player.vy = 0;
        player.invuln = 1.4;
        showToast("Kirby cayo por un hueco y perdio vida.");
    }
}

function updateEnemies(dt) {
    for (const enemy of state.enemies) {
        if (!enemy.active) {
            continue;
        }

        if (enemy.type === "boss") {
            updateBoss(enemy, dt);
            continue;
        }

        const player = state.player;
        const distance = player.x - enemy.x;
        if (Math.abs(distance) < 420) {
            enemy.vx = Math.sign(distance) * (enemy.type === "fire" ? 105 : 85);
        } else {
            if (enemy.x < enemy.patrolLeft) enemy.vx = Math.abs(enemy.vx);
            if (enemy.x > enemy.patrolRight) enemy.vx = -Math.abs(enemy.vx);
        }

        moveActor(enemy, dt);
        enemy.hurtTimer = Math.max(0, enemy.hurtTimer - dt);

        if (rectsOverlap(player, enemy)) {
            damagePlayer(enemy.type === "fire" ? 2 : 1);
        }
    }
}

function updateBoss(boss, dt) {
    const player = state.player;
    boss.x += boss.vx * dt;
    if (boss.x < 760) {
        boss.x = 760;
        boss.vx = Math.abs(boss.vx);
    }
    if (boss.x > state.level.width - 220) {
        boss.x = state.level.width - 220;
        boss.vx = -Math.abs(boss.vx);
    }

    boss.y = 408 + Math.sin(performance.now() / 180) * 18;
    boss.shotTimer -= dt;
    boss.minionTimer -= dt;
    boss.hurtTimer = Math.max(0, boss.hurtTimer - dt);

    if (boss.shotTimer <= 0) {
        const direction = player.x < boss.x ? -1 : 1;
        state.bossShots.push({
            x: boss.x + boss.w / 2,
            y: boss.y + 55,
            w: 28,
            h: 28,
            vx: direction * 310,
            vy: -80,
            active: true
        });
        boss.shotTimer = Math.max(0.55, 1.15 - (1 - boss.health / boss.maxHealth) * 0.45);
    }

    if (boss.minionTimer <= 0 && state.enemies.filter((enemy) => enemy.active && enemy.type !== "boss").length < 3) {
        state.enemies.push(makeEnemy({
            x: boss.x - 180,
            y: 470,
            type: Math.random() < 0.35 ? "fire" : "normal"
        }));
        showToast("El jefe llamo refuerzos.");
        boss.minionTimer = 5.5;
    }

    if (rectsOverlap(player, boss)) {
        damagePlayer(2);
    }
}

function updateProjectiles(dt) {
    for (const projectile of state.projectiles) {
        if (!projectile.active) continue;
        projectile.x += projectile.vx * dt;

        if (projectile.x < -80 || projectile.x > state.level.width + 80) {
            projectile.active = false;
            continue;
        }

        for (const enemy of state.enemies) {
            if (!enemy.active || !rectsOverlap(projectile, enemy)) {
                continue;
            }

            enemy.health -= projectile.damage;
            enemy.hurtTimer = 0.18;
            projectile.active = false;
            burst(projectile.x + projectile.w / 2, projectile.y + projectile.h / 2, projectile.type === "fire" ? "#ff6b35" : "#ffd166", 16);

            if (enemy.health <= 0) {
                enemy.active = false;
                state.player.score += enemy.type === "boss" ? 1800 : 100;
                burst(enemy.x + enemy.w / 2, enemy.y + enemy.h / 2, "#f05ca8", 26);
                if (enemy.type === "boss") {
                    finishGame(true);
                }
            } else {
                state.player.score += 40;
            }
            break;
        }
    }

    for (const shot of state.bossShots) {
        if (!shot.active) continue;
        shot.vy += 280 * dt;
        shot.x += shot.vx * dt;
        shot.y += shot.vy * dt;

        if (shot.x < -80 || shot.x > state.level.width + 80 || shot.y > WORLD_HEIGHT + 120) {
            shot.active = false;
        }
        if (shot.active && rectsOverlap(shot, state.player)) {
            shot.active = false;
            damagePlayer(1);
        }
    }

    state.projectiles = state.projectiles.filter((projectile) => projectile.active);
    state.bossShots = state.bossShots.filter((shot) => shot.active);
}

function updateFoods(dt) {
    for (const food of state.foods) {
        if (!food.active) continue;
        food.bob += dt * 5;
        if (rectsOverlap(state.player, food)) {
            food.active = false;
            if (food.type === "coin") {
                state.player.score += 80;
                showToast("Kirby recolecto un item de puntos.");
            } else {
                const heal = food.type === "tomato" ? 2 : 1;
                state.player.health = Math.min(state.player.maxHealth, state.player.health + heal);
                state.player.score += food.type === "tomato" ? 70 : 45;
                showToast("Kirby recupero vida.");
            }
            burst(food.x + food.w / 2, food.y + food.h / 2, "#53b96d", 12);
        }
    }

    state.itemTimer -= dt;
    if (state.itemTimer <= 0) {
        spawnRandomItem();
        state.itemTimer = 3.2 + Math.random() * 2.4;
    }
}

function updateParticles(dt) {
    for (const particle of state.particles) {
        particle.life -= dt;
        particle.x += particle.vx * dt;
        particle.y += particle.vy * dt;
        particle.vy += 320 * dt;
    }
    state.particles = state.particles.filter((particle) => particle.life > 0);
}

function updateCamera() {
    const target = state.player.x - view.w * 0.34;
    const maxCamera = Math.max(0, state.level.width - view.w);
    state.cameraX += (clamp(target, 0, maxCamera) - state.cameraX) * 0.14;
}

function updateGame(dt) {
    if (backend.connected) {
        updateBackend();
        return;
    }

    if (state.screen !== "playing") {
        return;
    }

    state.levelBannerTimer = Math.max(0, state.levelBannerTimer - dt);
    updatePlayer(dt);
    if (state.screen !== "playing") return;
    updateEnemies(dt);
    updateProjectiles(dt);
    updateFoods(dt);
    updateParticles(dt);
    updateCamera();

    if (state.levelIndex < 2 && state.player.x >= state.level.goalX) {
        const nextLevel = state.levelIndex + 1;
        const previousPlayer = state.player;
        if (nextLevel === 2) {
            showToast("La pantalla del jefe se abre.");
        }
        loadLevel(nextLevel, previousPlayer);
    }
}

function updateHud() {
    if (!state.player) {
        hud.health.textContent = "Vida: 5";
        hud.score.textContent = "Score: 0";
        hud.level.textContent = "Nivel: 1";
        hud.ability.textContent = "Habilidad: Ninguna";
        hud.boss.textContent = "";
        hud.mode.textContent = "Modo jugador";
        return;
    }

    const player = state.player;
    const ability = player.ability === "none"
        ? "Ninguna"
        : `${getAbilityName(player.ability)} ${Math.ceil(player.abilityTime)}s`;
    const boss = state.enemies.find((enemy) => enemy.active && enemy.type === "boss");

    hud.health.textContent = `Vida: ${Math.max(0, player.health)}/${player.maxHealth}`;
    hud.score.textContent = `Score: ${player.score}`;
    hud.level.textContent = `Nivel: ${state.levelIndex + 1}`;
    hud.ability.textContent = `Habilidad: ${ability}`;
    hud.boss.textContent = boss ? `Jefe: ${boss.health}/${boss.maxHealth}` : "";
    hud.mode.textContent = state.mode === "ai" ? "Modo computadora" : "Modo jugador";
}

function drawSky() {
    const gradient = ctx.createLinearGradient(0, 0, 0, view.h);
    gradient.addColorStop(0, "#67c8f5");
    gradient.addColorStop(0.42, "#b5ecf5");
    gradient.addColorStop(0.72, "#f7d2e2");
    gradient.addColorStop(1, "#f9efc9");
    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, view.w, view.h);

    ctx.fillStyle = "rgba(255, 255, 255, 0.18)";
    for (let y = 86; y < view.h; y += 96) {
        ctx.fillRect(0, y, view.w, 1.5);
    }
}

function drawParallaxHills(y, height, speed, color, ridgeColor) {
    const offset = state.cameraX * speed;
    const width = 420;
    const start = -((offset % width) + width);

    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.moveTo(0, view.h);
    for (let x = start; x <= view.w + width; x += width) {
        ctx.quadraticCurveTo(x + width * 0.25, y - height, x + width * 0.5, y);
        ctx.quadraticCurveTo(x + width * 0.75, y + height * 0.34, x + width, y);
    }
    ctx.lineTo(view.w, view.h);
    ctx.closePath();
    ctx.fill();

    ctx.strokeStyle = ridgeColor;
    ctx.lineWidth = 3;
    ctx.beginPath();
    for (let x = start; x <= view.w + width; x += width) {
        if (x === start) ctx.moveTo(x, y);
        ctx.quadraticCurveTo(x + width * 0.25, y - height, x + width * 0.5, y);
        ctx.quadraticCurveTo(x + width * 0.75, y + height * 0.34, x + width, y);
    }
    ctx.stroke();
}

function drawCloud(x, y, scale, color) {
    ctx.save();
    ctx.translate(x, y);
    ctx.scale(scale, scale);
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(0, 22, 24, 0, Math.PI * 2);
    ctx.arc(30, 10, 30, 0, Math.PI * 2);
    ctx.arc(66, 22, 26, 0, Math.PI * 2);
    ctx.arc(36, 32, 34, 0, Math.PI * 2);
    ctx.fill();
    ctx.fillRect(-8, 22, 90, 30);
    ctx.restore();
}

function drawBackground() {
    drawSky();
    const time = performance.now() / 1000;
    const cloudA = view.w - ((time * 22) % (view.w + 360));
    const cloudB = view.w - (((time * 16) + 210) % (view.w + 420));
    const cloudC = view.w - (((time * 11) + 420) % (view.w + 520));
    drawCloud(cloudA, 106, 0.8, "rgba(255, 232, 244, 0.92)");
    drawCloud(cloudB, 205, 1.05, "rgba(255, 215, 235, 0.85)");
    drawCloud(cloudC, 64, 0.58, "rgba(255, 255, 255, 0.72)");

    drawParallaxHills(view.h - 205, 58, 0.12, "rgba(123, 190, 197, 0.42)", "rgba(255, 255, 255, 0.22)");
    drawParallaxHills(view.h - 138, 44, 0.24, "rgba(115, 202, 133, 0.48)", "rgba(255, 255, 255, 0.24)");
    ctx.restore();
}

function worldX(x) {
    return x - state.cameraX;
}

function drawPlatforms() {
    for (const platform of state.level.platforms) {
        const x = worldX(platform.x);
        if (x + platform.w < -60 || x > view.w + 60) continue;

        const dirt = ctx.createLinearGradient(0, platform.y, 0, platform.y + platform.h);
        dirt.addColorStop(0, "#8a6342");
        dirt.addColorStop(0.55, "#6b4a35");
        dirt.addColorStop(1, "#523826");
        ctx.fillStyle = dirt;
        roundRect(ctx, x, platform.y, platform.w, platform.h, 4);
        ctx.fill();

        const grass = ctx.createLinearGradient(0, platform.y, 0, platform.y + 20);
        grass.addColorStop(0, "#7ee07c");
        grass.addColorStop(1, "#3da661");
        ctx.fillStyle = grass;
        roundRect(ctx, x, platform.y, platform.w, Math.min(18, platform.h), 4);
        ctx.fill();

        ctx.fillStyle = "rgba(255, 255, 255, 0.16)";
        for (let stripe = x + 28; stripe < x + platform.w; stripe += 86) {
            ctx.fillRect(stripe, platform.y + 24, 22, Math.max(8, platform.h - 34));
        }

        ctx.fillStyle = "rgba(58, 38, 25, 0.22)";
        for (let dot = x + 18; dot < x + platform.w; dot += 34) {
            ctx.fillRect(dot, platform.y + 34 + ((dot + platform.y) % 26), 5, 5);
        }

        ctx.strokeStyle = "rgba(27, 98, 50, 0.45)";
        ctx.lineWidth = 2;
        for (let blade = x + 12; blade < x + platform.w; blade += 22) {
            const top = platform.y + 3 + ((blade + state.cameraX) % 6);
            ctx.beginPath();
            ctx.moveTo(blade, platform.y + 16);
            ctx.quadraticCurveTo(blade + 4, top, blade + 9, platform.y + 16);
            ctx.stroke();
        }

        ctx.fillStyle = "#ffd166";
        for (let flower = x + 54; flower < x + platform.w; flower += 180) {
            if (platform.w < 180 || platform.h > 40) continue;
            ctx.beginPath();
            ctx.arc(flower, platform.y - 4, 3, 0, Math.PI * 2);
            ctx.fill();
        }
    }
}

function drawGoal() {
    if (state.levelIndex >= 2) return;
    const x = worldX(state.level.goalX);
    if (x < -80 || x > view.w + 80) return;

    ctx.strokeStyle = "#233044";
    ctx.lineWidth = 5;
    ctx.beginPath();
    ctx.moveTo(x, 320);
    ctx.lineTo(x, 540);
    ctx.stroke();

    ctx.fillStyle = "#ffd166";
    ctx.beginPath();
    ctx.moveTo(x + 2, 326);
    ctx.lineTo(x + 118, 356);
    ctx.lineTo(x + 2, 386);
    ctx.closePath();
    ctx.fill();
    ctx.fillStyle = "#233044";
    ctx.font = "900 18px system-ui, sans-serif";
    ctx.fillText("META", x + 22, 363);
}

function drawEntityShadow(x, y, width, alpha = 0.22) {
    ctx.save();
    ctx.fillStyle = `rgba(31, 41, 55, ${alpha})`;
    ctx.beginPath();
    ctx.ellipse(x, y, width, 7, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.restore();
}

function roundRect(context, x, y, w, h, r) {
    const radius = Math.min(r, w / 2, h / 2);
    context.beginPath();
    context.moveTo(x + radius, y);
    context.arcTo(x + w, y, x + w, y + h, radius);
    context.arcTo(x + w, y + h, x, y + h, radius);
    context.arcTo(x, y + h, x, y, radius);
    context.arcTo(x, y, x + w, y, radius);
    context.closePath();
}

function drawKirby() {
    const player = state.player;
    if (!player) return;

    const x = worldX(player.x);
    const y = player.y;
    const blink = player.invuln > 0 && Math.floor(performance.now() / 90) % 2 === 0;
    if (blink) return;

    const inflated = player.state === "floating";
    const inhale = player.state === "inhaling";
    drawEntityShadow(x + player.w / 2, player.y + player.h + 6, inflated ? 30 : 24, player.onGround ? 0.22 : 0.1);

    ctx.save();
    ctx.translate(x + player.w / 2, y + player.h / 2);
    if (player.facing < 0) ctx.scale(-1, 1);

    const speedTilt = clamp(player.vx / WALK_SPEED, -1, 1) * 0.08;
    const bob = Math.sin(performance.now() / 85) * (Math.abs(player.vx) > 45 && player.onGround ? 2.5 : 0);
    ctx.rotate(speedTilt);
    ctx.translate(0, bob);

    const bodyGradient = ctx.createRadialGradient(-10, -12, 6, 2, 2, inflated ? 38 : 31);
    bodyGradient.addColorStop(0, "#ff9acb");
    bodyGradient.addColorStop(0.55, "#f05ca8");
    bodyGradient.addColorStop(1, "#d83f8b");
    ctx.fillStyle = bodyGradient;
    ctx.strokeStyle = "#bd2b79";
    ctx.lineWidth = 3;

    ctx.beginPath();
    ctx.ellipse(0, inflated ? -2 : 0, inflated ? 32 : 27, inflated ? 28 : 24, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();

    ctx.fillStyle = "#f29ac2";
    ctx.beginPath();
    ctx.ellipse(-28, 2, 11, 8, -0.3, 0, Math.PI * 2);
    ctx.ellipse(28, 3, 11, 8, 0.3, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = "#d94888";
    ctx.beginPath();
    ctx.ellipse(-14, 24, 16, 8, -0.1, 0, Math.PI * 2);
    ctx.ellipse(16, 24, 16, 8, 0.1, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = "#233044";
    ctx.beginPath();
    ctx.ellipse(-8, -8, 4, 8, 0, 0, Math.PI * 2);
    ctx.ellipse(9, -8, 4, 8, 0, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = "white";
    ctx.beginPath();
    ctx.ellipse(-8, -11, 1.5, 3, 0, 0, Math.PI * 2);
    ctx.ellipse(9, -11, 1.5, 3, 0, 0, Math.PI * 2);
    ctx.fill();

    ctx.strokeStyle = "#233044";
    ctx.lineWidth = 3;
    if (inhale) {
        ctx.beginPath();
        ctx.arc(17, 6, 10, 0, Math.PI * 2);
        ctx.stroke();
        ctx.fillStyle = "rgba(255, 255, 255, 0.35)";
        ctx.beginPath();
        ctx.arc(58, 5, 16 + Math.sin(performance.now() / 80) * 4, 0, Math.PI * 2);
        ctx.fill();
    } else if (inflated) {
        ctx.beginPath();
        ctx.arc(1, 8, 5, 0, Math.PI * 2);
        ctx.stroke();
    } else {
        ctx.beginPath();
        ctx.arc(1, 1, 9, 0.15, Math.PI - 0.15);
        ctx.stroke();
    }

    ctx.restore();
}

function drawEnemy(enemy) {
    if (!enemy.active) return;
    if (enemy.type === "boss") {
        drawBoss(enemy);
        return;
    }

    const x = worldX(enemy.x);
    if (x < -90 || x > view.w + 90) return;

    drawEntityShadow(x + enemy.w / 2, enemy.y + enemy.h + 6, enemy.w * 0.36, enemy.onGround ? 0.18 : 0.08);

    ctx.save();
    ctx.translate(x + enemy.w / 2, enemy.y + enemy.h / 2);
    if (enemy.hurtTimer > 0) {
        ctx.globalAlpha = 0.55;
    }

    const fire = enemy.type === "fire";
    const enemyGradient = ctx.createLinearGradient(0, -enemy.h / 2, 0, enemy.h / 2);
    enemyGradient.addColorStop(0, fire ? "#ffb15f" : "#a487ff");
    enemyGradient.addColorStop(0.62, fire ? "#ff7147" : "#7c5bd6");
    enemyGradient.addColorStop(1, fire ? "#c23b2e" : "#5637a7");
    ctx.fillStyle = enemyGradient;
    ctx.strokeStyle = fire ? "#b83224" : "#4d3a9a";
    ctx.lineWidth = 3;
    roundRect(ctx, -enemy.w / 2, -enemy.h / 2, enemy.w, enemy.h, 8);
    ctx.fill();
    ctx.stroke();

    if (fire) {
        ctx.fillStyle = "#ffd166";
        ctx.beginPath();
        ctx.moveTo(-12, -23);
        ctx.lineTo(-3, -38);
        ctx.lineTo(6, -23);
        ctx.lineTo(16, -38);
        ctx.lineTo(20, -18);
        ctx.closePath();
        ctx.fill();
    }

    ctx.fillStyle = "white";
    ctx.fillRect(-14, -8, 8, 8);
    ctx.fillRect(8, -8, 8, 8);
    ctx.fillStyle = "#233044";
    ctx.fillRect(-11, -6, 4, 5);
    ctx.fillRect(11, -6, 4, 5);
    ctx.fillRect(-15, 10, 30, 4);
    ctx.restore();
}

function drawBoss(boss) {
    const x = worldX(boss.x);
    if (x < -180 || x > view.w + 180) return;

    drawEntityShadow(x + boss.w / 2, boss.y + boss.h + 10, 58, 0.24);

    ctx.save();
    ctx.translate(x + boss.w / 2, boss.y + boss.h / 2);
    if (boss.hurtTimer > 0) {
        ctx.globalAlpha = 0.65;
    }

    const bossGradient = ctx.createRadialGradient(-22, -20, 10, 0, 8, 78);
    bossGradient.addColorStop(0, "#9b83ff");
    bossGradient.addColorStop(0.58, "#5e48a8");
    bossGradient.addColorStop(1, "#33235d");
    ctx.fillStyle = bossGradient;
    ctx.strokeStyle = "#251c47";
    ctx.lineWidth = 5;
    ctx.beginPath();
    ctx.ellipse(0, 10, 60, 52, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();

    ctx.fillStyle = "#f05ca8";
    ctx.beginPath();
    ctx.ellipse(-36, 10, 20, 16, -0.2, 0, Math.PI * 2);
    ctx.ellipse(36, 10, 20, 16, 0.2, 0, Math.PI * 2);
    ctx.fill();

    ctx.fillStyle = "#ffd166";
    ctx.beginPath();
    ctx.moveTo(-32, -52);
    ctx.lineTo(-18, -78);
    ctx.lineTo(-2, -52);
    ctx.lineTo(16, -82);
    ctx.lineTo(34, -52);
    ctx.closePath();
    ctx.fill();

    ctx.fillStyle = "white";
    ctx.beginPath();
    ctx.ellipse(-18, -10, 12, 15, 0, 0, Math.PI * 2);
    ctx.ellipse(20, -10, 12, 15, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.fillStyle = "#1f2937";
    ctx.fillRect(-23, -10, 8, 12);
    ctx.fillRect(17, -10, 8, 12);
    ctx.fillRect(-26, 26, 52, 6);

    ctx.restore();
}

function drawFood(food) {
    if (!food.active) return;
    const x = worldX(food.x);
    if (x < -60 || x > view.w + 60) return;
    const y = food.y + Math.sin(food.bob) * 5;

    drawEntityShadow(x + food.w / 2, food.y + food.h + 8, 15, 0.12);

    ctx.save();
    ctx.translate(x + food.w / 2, y + food.h / 2);
    ctx.shadowColor = food.type === "coin" ? "rgba(255, 209, 102, 0.55)" : "rgba(240, 92, 168, 0.35)";
    ctx.shadowBlur = 12;

    if (food.type === "coin") {
        ctx.fillStyle = "#ffd166";
        ctx.strokeStyle = "#b78114";
        ctx.lineWidth = 3;
        ctx.beginPath();
        ctx.ellipse(0, 0, 15, 18, 0, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();
        ctx.fillStyle = "#9d6a0d";
        ctx.font = "900 18px system-ui, sans-serif";
        ctx.fillText("$", -5, 7);
    } else if (food.type === "tomato") {
        ctx.fillStyle = "#e44f54";
        ctx.strokeStyle = "#98272c";
        ctx.lineWidth = 3;
        ctx.beginPath();
        ctx.arc(0, 2, 17, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();
        ctx.fillStyle = "#53b96d";
        ctx.fillRect(-5, -20, 10, 10);
    } else {
        ctx.fillStyle = "#fff0a8";
        ctx.strokeStyle = "#ce8f2d";
        ctx.lineWidth = 3;
        roundRect(ctx, -16, -12, 32, 24, 7);
        ctx.fill();
        ctx.stroke();
        ctx.fillStyle = "#f05ca8";
        ctx.fillRect(-10, -5, 20, 6);
    }

    ctx.restore();
}

function drawProjectile(projectile) {
    const x = worldX(projectile.x);
    if (x < -80 || x > view.w + 80) return;

    ctx.save();
    ctx.translate(x + projectile.w / 2, projectile.y + projectile.h / 2);
    ctx.shadowColor = projectile.type === "fire" ? "rgba(255, 107, 53, 0.82)" : "rgba(255, 209, 102, 0.82)";
    ctx.shadowBlur = projectile.type === "fire" ? 20 : 14;

    if (projectile.type === "fire") {
        ctx.fillStyle = "#ff6b35";
        ctx.strokeStyle = "#b83224";
        ctx.lineWidth = 3;
        ctx.beginPath();
        ctx.moveTo(-16, 12);
        ctx.quadraticCurveTo(-4, -22, 18, -8);
        ctx.quadraticCurveTo(7, -2, 18, 14);
        ctx.quadraticCurveTo(0, 8, -16, 12);
        ctx.fill();
        ctx.stroke();
    } else {
        ctx.fillStyle = "#ffd166";
        ctx.strokeStyle = "#a87500";
        ctx.lineWidth = 2;
        drawStar(ctx, 0, 0, 15, 7, 5);
        ctx.fill();
        ctx.stroke();
    }

    ctx.restore();
}

function drawBossShot(shot) {
    const x = worldX(shot.x);
    ctx.save();
    ctx.translate(x + shot.w / 2, shot.y + shot.h / 2);
    ctx.shadowColor = "rgba(255, 113, 71, 0.75)";
    ctx.shadowBlur = 18;
    ctx.fillStyle = "#ff7147";
    ctx.strokeStyle = "#7c2116";
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.arc(0, 0, 14, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();
    ctx.fillStyle = "#ffd166";
    ctx.beginPath();
    ctx.arc(-4, -4, 5, 0, Math.PI * 2);
    ctx.fill();
    ctx.restore();
}

function drawStar(context, x, y, outer, inner, points) {
    context.beginPath();
    for (let i = 0; i < points * 2; i++) {
        const radius = i % 2 === 0 ? outer : inner;
        const angle = -Math.PI / 2 + i * Math.PI / points;
        const px = x + Math.cos(angle) * radius;
        const py = y + Math.sin(angle) * radius;
        if (i === 0) context.moveTo(px, py);
        else context.lineTo(px, py);
    }
    context.closePath();
}

function drawParticles() {
    for (const particle of state.particles) {
        const alpha = clamp(particle.life / particle.maxLife, 0, 1);
        ctx.globalAlpha = alpha;
        ctx.fillStyle = particle.color;
        ctx.beginPath();
        ctx.arc(worldX(particle.x), particle.y, 4 + alpha * 3, 0, Math.PI * 2);
        ctx.fill();
        ctx.globalAlpha = 1;
    }
}

function drawLevelBanner() {
    if (state.levelBannerTimer <= 0 || !state.level) return;
    const alpha = clamp(state.levelBannerTimer / 0.6, 0, 1);
    ctx.save();
    ctx.globalAlpha = Math.min(alpha, 1);
    ctx.fillStyle = "rgba(255, 255, 255, 0.9)";
    roundRect(ctx, view.w / 2 - 190, 84, 380, 62, 8);
    ctx.fill();
    ctx.strokeStyle = "rgba(240, 92, 168, 0.55)";
    ctx.lineWidth = 3;
    ctx.stroke();
    ctx.fillStyle = "#bd2b79";
    ctx.font = "900 24px system-ui, sans-serif";
    ctx.textAlign = "center";
    ctx.fillText(state.level.name, view.w / 2, 123);
    ctx.restore();
}

function drawMenuScene() {
    drawBackground();
    const fakeLevel = {
        platforms: [
            { x: 0, y: view.h - 95, w: view.w, h: 120 },
            { x: view.w * 0.14, y: view.h - 220, w: 220, h: 26 },
            { x: view.w * 0.64, y: view.h - 250, w: 230, h: 26 }
        ]
    };
    const previousLevel = state.level;
    const previousCamera = state.cameraX;
    state.level = fakeLevel;
    state.cameraX = 0;
    drawPlatforms();
    state.level = previousLevel;
    state.cameraX = previousCamera;

    ctx.save();
    ctx.translate(view.w * 0.18, view.h - 146);
    ctx.scale(1.4, 1.4);
    drawKirbyPreview();
    ctx.restore();
}

function drawKirbyPreview() {
    ctx.fillStyle = "#f05ca8";
    ctx.strokeStyle = "#bd2b79";
    ctx.lineWidth = 3;
    ctx.beginPath();
    ctx.ellipse(0, 0, 28, 24, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.stroke();
    ctx.fillStyle = "#233044";
    ctx.beginPath();
    ctx.ellipse(-8, -8, 4, 8, 0, 0, Math.PI * 2);
    ctx.ellipse(9, -8, 4, 8, 0, 0, Math.PI * 2);
    ctx.fill();
    ctx.strokeStyle = "#233044";
    ctx.beginPath();
    ctx.arc(0, 2, 9, 0.1, Math.PI - 0.1);
    ctx.stroke();
}

function drawGame() {
    drawBackground();

    if (!state.level || !state.player) {
        drawMenuScene();
        return;
    }

    drawGoal();
    drawPlatforms();
    state.foods.forEach(drawFood);
    state.enemies.forEach(drawEnemy);
    state.projectiles.forEach(drawProjectile);
    state.bossShots.forEach(drawBossShot);
    drawKirby();
    drawParticles();
    drawLevelBanner();
}

function draw() {
    ctx.clearRect(0, 0, view.w, view.h);
    if (state.screen === "menu" && !state.level) {
        drawMenuScene();
    } else {
        drawGame();
    }
    updateHud();
}

function loop(timestamp) {
    const dt = Math.min(0.033, (timestamp - lastTime) / 1000);
    lastTime = timestamp;
    updateGame(dt);
    draw();
    pressed.clear();
    requestAnimationFrame(loop);
}

detectBackend();
requestAnimationFrame(loop);
