# Explicacion de hilos, flujo del juego y responsabilidades por archivo

Este documento explica como corre el juego por dentro. Esta pensado para apoyar
una presentacion o informe: primero se resume el flujo general, despues se
explican hilos, mutex y semaforo, y al final se detalla el papel de cada archivo
agrupado por integrante.

## 1. Flujo general del juego

El juego inicia en `src/main.cpp`. Ahi se configura `ncurses`, se activan los
colores y se muestra la pantalla de instrucciones. Despues se abre el menu
principal.

El menu principal vive en `src/ui/MainMenu.cpp`. Cuando el usuario selecciona
una modalidad, el menu crea un objeto `Game`, llama a `init()` y luego a
`run()`.

El flujo completo queda asi:

1. `main.cpp` inicializa consola y colores.
2. `InstructionScreen` muestra controles y simbolos.
3. `MainMenu` permite escoger modo jugador o modo computadora.
4. `Game::init()` prepara mapa, jugador, comidas, enemigos e hilos.
5. `Game::run()` mantiene vivo el ciclo principal.
6. `Game::update()` revisa reglas globales: vida, colisiones, items, enemigos,
   jefe y cambios de nivel.
7. `Game::render()` dibuja mapa, entidades y HUD.
8. Al perder o ganar, `GameOverScreen` pide nombre y guarda puntaje.
9. El usuario decide reiniciar o volver al menu.

La clase `Game` se separo en varios archivos para no concentrar toda la logica
en un solo `.cpp`:

| Archivo | Papel dentro de Game |
|---|---|
| `Game.cpp` | Inicio, carga de nivel, loop principal y cierre. |
| `GameActions.cpp` | Input, modo automatico, absorber, disparar y registro. |
| `GameThreads.cpp` | Funciones `void*` usadas por Pthreads. |
| `GameUpdate.cpp` | Reglas globales del mundo y colisiones. |
| `GameRender.cpp` | Conexion entre `Renderer` y `HUD`. |
| `GameWorld.cpp` | Helpers de spawn, comida sobre suelo y pantalla del jefe. |
| `GameInternals.h` | Declaraciones internas compartidas entre archivos `Game*.cpp`. |

## 2. Hilos del juego

El proyecto usa Pthreads para simular concurrencia. Hay un hilo principal y
varios hilos auxiliares.

| Hilo | Donde se crea | Que hace |
|---|---|---|
| Hilo principal | `Game::run()` | Ejecuta `update()`, llama `render()` y controla cierre. |
| Hilo de Kirby | `pthread_create(... Game::playerThreadEntry ...)` | Lee teclado o ejecuta modo computadora. |
| Hilos de enemigos | `createEnemyThread()` | Ejecutan `enemyThreadFunction()` mientras el enemigo esta activo. |
| Hilos de proyectiles | `createProjectileThread()` | Mueven cada proyectil hasta que se desactiva. |
| Hilo de eventos | `pthread_create(... Game::eventThreadEntry ...)` | Espera un semaforo y genera items cuando hacen falta. |

### 2.1 Hilo principal

El hilo principal corre en `Game::run()`. Su trabajo es mantener la partida viva:

```cpp
while (running) {
    pthread_mutex_lock(&threadManager.gameMutex);
    update();
    bool shouldRender = running;
    pthread_mutex_unlock(&threadManager.gameMutex);

    if (!shouldRender) break;

    render();
    usleep(50000);
}
```

Ese ciclo toma el mutex antes de `update()` porque ahi se modifica casi todo el
estado compartido: jugador, enemigos, proyectiles, comidas, mapa, vida, score y
nivel. Despues suelta el mutex y llama a `render()`.

### 2.2 Hilo de Kirby

El hilo de Kirby se crea en `Game::init()` con:

```cpp
pthread_create(&playerThread, NULL, Game::playerThreadEntry, this);
```

Este hilo corre mientras `playerThreadActive` y `running` sean verdaderos. Antes
de llamar `processInput()` toma el mutex porque esa funcion puede mover a Kirby,
activar absorcion, crear proyectiles o ejecutar la IA del modo automatico.

En modo normal, `processInput()` lee:

| Tecla | Accion |
|---|---|
| `A` | Mover izquierda. |
| `D` | Mover derecha. |
| `W` | Saltar o flotar. |
| `J` | Absorber enemigo. |
| `H` | Dejar de absorber. |
| `K` | Usar habilidad copiada. |
| `Q` | Salir de la partida. |

En modo computadora, casi todo el teclado se ignora. Solo `Q` permite salir. Lo
demas lo decide `processComputerInput()`.

### 2.3 Modo automatico

El modo automatico esta en `GameActions.cpp`, dentro de
`Game::processComputerInput()`.

La idea no es que Kirby juegue perfecto, sino que simule una estrategia basica:

1. Busca el enemigo activo mas cercano.
2. Busca la comida activa mas cercana.
3. Escoge una intencion cada cierto tiempo con `aiBehavior`.
4. Decide si avanza, busca comida, se acerca a un enemigo o se retira.
5. Si un enemigo esta muy cerca, decide entre absorber, disparar o escapar.
6. Si ya tiene habilidad copiada y un enemigo esta a rango, puede disparar.

Dos temporizadores evitan que el comportamiento sea demasiado repetitivo:

| Variable | Uso |
|---|---|
| `actionCooldown` | Evita absorber o disparar en cada frame. |
| `aiDecisionTimer` | Mantiene una decision por varios frames antes de cambiarla. |

Tambien usa azar con `rand() % 100`; por eso no siempre salta, retrocede o ataca
en el mismo momento.

### 2.4 Hilos de enemigos

Cada enemigo normal puede tener su propio hilo. Se crea con:

```cpp
createEnemyThread(enemy, &threadManager.gameMutex);
```

La funcion usada es `enemyThreadFunction()` en `GameThreads.cpp`.

Ese hilo:

1. Revisa si el enemigo sigue activo.
2. Toma el mutex.
3. Llama `enemy->update()`.
4. Suelta el mutex.
5. Duerme con `usleep(100000)`.

Nota honesta: el movimiento mas visible de los enemigos normales se coordina
desde `GameUpdate.cpp`, donde se llama `enemyAI.updateEnemy(enemy, player)`. El
hilo del enemigo existe como entidad concurrente, pero la IA de
persecucion/patrulla se mantiene en el update global para centralizar colisiones
y gravedad.

### 2.5 Hilos de proyectiles

Cuando Kirby usa una habilidad, `fireProjectile()` crea un `Projectile` y abre
un hilo con:

```cpp
createProjectileThread(projectile, &threadManager.gameMutex);
```

El hilo del proyectil vive en `projectileThreadFunction()`.

Ese hilo mueve el proyectil cada 50 ms:

1. Toma el mutex.
2. Llama `projectile->update()`.
3. Suelta el mutex.
4. Duerme.

La colision del proyectil no se revisa ahi; se revisa en `GameUpdate.cpp`. Esto
mantiene el dano, score y muerte de enemigos en un solo lugar.

### 2.6 Hilo de eventos

El hilo de eventos se crea en `Game::init()`:

```cpp
pthread_create(&eventThread, NULL, Game::eventThreadEntry, this);
```

Este hilo no corre todo el tiempo. Espera con:

```cpp
sem_wait(&game->threadManager.eventSemaphore);
```

Cada cierto numero de frames, `Game::update()` hace:

```cpp
sem_post(&threadManager.eventSemaphore);
```

Entonces el hilo despierta, toma el mutex y revisa si hay pocas comidas activas.
Si hay menos de 3 comidas y no se esta en el nivel del jefe, genera nuevos items
con `spawnFoodsOnGround()`.

## 3. Mutex y semaforo

### 3.1 Mutex

El mutex principal es `ThreadManager::gameMutex`. Se inicializa en
`ThreadManager::init()` con:

```cpp
pthread_mutex_init(&gameMutex, NULL);
```

Sirve como candado. Antes de tocar estado compartido, un hilo entra al candado.
Al terminar, lo suelta.

Estado compartido protegido:

- `player`
- `enemies`
- `projectiles`
- `foods`
- `map`
- `eventLog`
- `running`
- contadores de nivel, jefe y dano por contacto

Sin este mutex podria ocurrir que el hilo del proyectil mueva un ataque mientras
`Game::update()` esta revisando su colision.

### 3.2 Semaforo

El semaforo es `ThreadManager::eventSemaphore`. Se inicializa en cero:

```cpp
sem_init(&eventSemaphore, 0, 0);
```

Como empieza en cero, el hilo de eventos queda dormido. Cuando `Game::update()`
quiere activar eventos, llama `sem_post()`. Eso permite que `sem_wait()` continue.

En resumen:

- `sem_wait()` significa: "me duermo hasta que alguien me avise".
- `sem_post()` significa: "ya puedes despertar y hacer tu trabajo".

Al cerrar el juego tambien se hace `sem_post()` para desbloquear el hilo de
eventos si estaba esperando. Asi puede salir y luego `pthread_join()` lo espera.

## 4. Flujo de una partida completa

1. `MainMenu` crea `Game(false)` o `Game(true)`.
2. `Game::init()` inicializa `ThreadManager`.
3. Se configura `ncurses` en modo no bloqueante.
4. Se crea `Player`.
5. Se carga `level1.txt`.
6. Se generan comidas iniciales.
7. Se crean enemigos iniciales.
8. Por cada enemigo se abre un hilo.
9. Se abre el hilo de Kirby.
10. Se abre el hilo de eventos.
11. `Game::run()` entra al loop principal.
12. `update()` revisa fisica, colisiones, vida, score y niveles.
13. `render()` dibuja mundo y HUD.
14. Si Kirby llega al final del mapa, se carga el siguiente nivel.
15. Si pasa el nivel 2, se muestra pantalla de jefe y se carga `boss.txt`.
16. Si el jefe muere, se muestra victoria.
17. Si Kirby pierde toda la vida, se muestra derrota.
18. `ScoreManager` guarda el puntaje.
19. El usuario decide reiniciar o volver al menu.
20. `Game::run()` apaga hilos, destruye mutex/semaforo y termina.

## 5. Orellana

### `src/graphics/AnimationSystem.cpp`

Controla animaciones ASCII pequenas. No dibuja directamente; solo devuelve
strings segun un contador de frames.

- `AnimationSystem::AnimationSystem()` inicia `frameCounter`.
- `update()` aumenta el contador y lo reinicia al pasar de 60.
- `getEnemySprite()` alterna `[0_0]`, `[-_-]` o `[x_x]`.
- `getItemSprite()` alterna el sprite de item.

Se conecta con `Renderer::render()`, que llama `animationSystem.update()` en
cada dibujo. No crea hilos; se ejecuta durante renderizado bajo el mutex de
`Game::render()`.

### `src/graphics/Camera.cpp`

Maneja el scroll horizontal.

- `Camera::Camera()` inicia `offsetX` en cero.
- `update(int playerX)` calcula `offsetX = playerX - 30`.
- `getOffsetX()` devuelve el desplazamiento.

`GameUpdate.cpp` actualiza la camara con la X de Kirby. `Renderer.cpp` usa el
offset para recortar cada fila del mapa y mostrar solo la ventana visible.

### `src/graphics/HUD.cpp`

Dibuja la informacion superior:

- vida
- score
- nivel
- habilidad activa
- vida del jefe
- enemigos activos
- proyectiles activos
- registro del modo computadora

Si `eventLog` esta vacio, muestra HUD limpio. Si tiene datos, muestra coordenadas
y registro. No crea hilos; lee estado compartido desde `GameRender.cpp`, dentro
del mutex.

### `src/graphics/Renderer.cpp`

Dibuja el mundo visible:

- limpia pantalla con `erase()`;
- calcula el offset de camara;
- dibuja nubes ASCII moviles;
- dibuja mapa y meta;
- dibuja Kirby;
- dibuja enemigos normales, enemigos de fuego y jefe;
- dibuja comidas;
- dibuja proyectiles.

Tambien usa colores de `ncurses` para separar visualmente elementos. No crea
hilos, pero se ejecuta protegido porque lee vectores que otros hilos pueden
modificar.

### `src/ai/EnemyAI.cpp`

Decide si un enemigo patrulla o persigue.

- Calcula distancia horizontal con `PathSystem`.
- Calcula diferencia vertical con Kirby.
- Si Kirby esta cerca y a una altura parecida, cambia a `CHASE`.
- Si no, usa `PATROL`.

Se llama desde `GameUpdate.cpp` para enemigos que no son jefe.

### `src/ai/PathSystem.cpp`

Implementa el movimiento concreto de enemigos normales.

- `moveTowardsTarget()` mueve un enemigo hacia una X objetivo.
- `patrolMovement()` lo mueve alrededor de su posicion inicial.
- `calculateDistance()` calcula distancia horizontal.

Usa mapas globales por puntero de enemigo para recordar donde nacio y hacia que
lado patrulla.

### `src/core/GameRender.cpp`

Separa la parte visual de `Game`.

`Game::render()`:

1. toma `gameMutex`;
2. decide si muestra o esconde el log;
3. llama `renderer.render(...)`;
4. llama `hud.render(...)`;
5. hace `refresh()`;
6. suelta el mutex.

Esto evita dibujar mientras otro hilo mueve entidades.

### `src/core/GameThreads.cpp`

Archivo central de concurrencia.

Contiene:

- `enemyThreadFunction()`
- `projectileThreadFunction()`
- `createEnemyThread()`
- `createProjectileThread()`
- `Game::playerThreadEntry()`
- `Game::eventThreadEntry()`

Todos los hilos que tocan estado compartido usan `gameMutex`. El hilo de eventos
usa ademas `eventSemaphore`, esperando con `sem_wait()` hasta que `GameUpdate`
lo despierte con `sem_post()`.

### `src/core/GameUpdate.cpp`

Contiene las reglas globales:

- actualiza a Kirby;
- despierta eventos cada cierto tiempo;
- aplica gravedad;
- actualiza enemigos y jefe;
- revisa dano por contacto;
- genera apoyo del jefe;
- repone enemigos en niveles normales;
- valida impactos de proyectiles;
- detecta victoria;
- procesa items;
- actualiza camara;
- detecta game over;
- cambia de nivel.

Se ejecuta desde el hilo principal, dentro del mutex.

## 6. Chejino

### `src/world/LevelManager.cpp`

Decide que archivo de nivel cargar.

- Nivel 1: `assets/levels/level1.txt`
- Nivel 2: `assets/levels/level2.txt`
- Nivel 3: `assets/levels/boss.txt`

Limpia el `TileMap`, lee el archivo linea por linea y agrega cada fila al mapa.

### `src/world/Platform.cpp`

Implementa una plataforma rectangular basica con `x`, `y` y `width`.

`isOnPlatform()` revisa si una entidad esta justo sobre la plataforma. Aunque la
fisica actual usa principalmente `TileMap::isSolid()`, esta clase sirve como
base para plataformas explicitas.

### `src/world/SpawnSystem.cpp`

Crea entidades nuevas.

- `spawnEnemies()` crea enemigos normales o de fuego.
- `spawnFood()` crea comidas/items aleatorios.

Las comidas nacen con posicion inicial aleatoria y luego `GameWorld.cpp` las
acomoda sobre suelo real.

### `src/world/TileMap.cpp`

Guarda el mapa ASCII como vector de strings.

- `clearMap()` borra el mapa.
- `addRow()` agrega filas y actualiza ancho/alto.
- `isSolid()` responde si una celda es pared/suelo.
- `getGrid()` entrega la grilla al renderer.

Si una coordenada esta fuera del mapa, `isSolid()` devuelve `true`. Eso funciona
como pared invisible y evita errores por salirse de la matriz.

### `src/ui/GameOverScreen.cpp`

Muestra derrota o victoria y guarda puntaje.

- `askPlayerName()` pide nombre.
- `askRestartChoice()` permite reiniciar o volver al menu.
- `show()` muestra derrota.
- `showVictory()` muestra victoria contra jefe.

Usa `ScoreManager::saveScore()` para persistir el resultado.

### `src/ui/InstructionScreen.cpp`

Muestra objetivo, controles y simbologia:

- Kirby;
- enemigo normal;
- enemigo de fuego;
- proyectiles;
- items;
- meta.

Tambien indica que cualquier tecla vuelve al menu.

### `src/ui/MainMenu.cpp`

Muestra el menu principal y lanza la modalidad seleccionada.

- `Game(false)` inicia modo jugador.
- `Game(true)` inicia modo computadora.
- Tambien abre instrucciones y puntajes.

La funcion `launchGame()` repite la partida si `Game::run()` devuelve `true`.

### `src/ui/PauseMenu.cpp`

Muestra una pausa simple con:

- continuar partida;
- salir al menu principal.

Retorna `true` para continuar y `false` para salir.

### `src/ui/ScoreScreen.cpp`

Muestra puntajes persistidos.

- Carga datos con `ScoreManager::loadTopScores()`.
- Muestra hasta 10 filas.
- Resalta el primer puesto.
- Espera una tecla para volver.

### `src/core/GameWorld.cpp`

Agrupa helpers del mundo.

- `createRandomEnemy()` crea enemigo normal o de fuego.
- `deactivateLevelEntities()` apaga entidades al cambiar nivel.
- `showBossScreen()` muestra pantalla previa al jefe.
- `findGroundY()` busca suelo debajo de una X.
- `placeFoodsOnGround()` mueve comida a suelo real.
- `spawnFoodsOnGround()` genera comida y acomoda solo las nuevas.
- `getActiveBoss()` busca jefe activo.
- `resetPlayerAfterFall()` quita vida y reposiciona a Kirby.

No borra punteros al limpiar niveles porque puede haber hilos terminando; solo
desactiva entidades.

### `src/core/ThreadManager.cpp`

Inicializa y destruye sincronizacion.

- `init()` crea `gameMutex` y `eventSemaphore`.
- `destroy()` los libera.

El mutex protege estado compartido. El semaforo duerme/despierta el hilo de
eventos.

### `src/core/Timer.cpp`

Implementa un temporizador simple:

- `start()` guarda el instante inicial.
- `stop()` detiene la medicion.
- `getElapsedSeconds()` devuelve segundos transcurridos.

Actualmente el loop usa contadores y `usleep`, pero `Timer` queda listo para
eventos temporizados futuros.

## 7. Altan

### `src/save/ScoreManager.cpp`

Maneja persistencia de puntajes.

- Lee `assets/scores.json`.
- Puede cargar `assets/scores.txt` como respaldo viejo.
- Parsea nombres y scores.
- Ordena de mayor a menor.
- Conserva los mejores 10.
- Escribe de nuevo el JSON.

Lo usan `GameOverScreen` para guardar y `ScoreScreen` para mostrar.

### `src/physics/CollisionSystem.cpp`

Implementa colisiones AABB. Recibe dos rectangulos y devuelve si se traslapan.

Se usa para:

- Kirby contra enemigos;
- Kirby contra comidas;
- proyectiles contra enemigos.

### `src/physics/GravitySystem.cpp`

Aplica gravedad y corrige penetracion con suelo.

- `overlapsSolid()` revisa si la caja del personaje toca un tile solido.
- `hasGroundBelow()` revisa si hay suelo justo debajo.
- `applyGravity()` saca al personaje del suelo si quedo metido y actualiza
  `grounded`.

Kirby maneja parte de su movimiento vertical en `Player::update()`, mientras
`GravitySystem` corrige contacto con el mapa. Para enemigos, la gravedad se
fuerza aqui.

### `src/physics/PhysicsBody.cpp`

Base fisica reusable.

- `applyGravity()` aumenta velocidad vertical.
- `applyFriction()` reduce velocidad horizontal.
- `enforceBoundaries()` impide salir del mapa.
- Tiene getters para masa, gravedad y friccion.

El juego actual usa principalmente `GravitySystem`, pero `PhysicsBody` sirve
para extender fisicas mas completas despues.

### `src/input/InputManager.cpp`

Encapsula lectura de teclado.

`getInput()` llama `getch()`. Como `nodelay(stdscr, TRUE)` esta activo, no
bloquea el juego cuando no hay tecla.

### `src/core/Game.cpp`

Base del ciclo de vida.

- Constructor: inicializa modo, banderas y contadores.
- `init()`: inicializa sincronizacion, consola, mapa, jugador, comidas, enemigos
  e hilos.
- `loadLevel()`: convierte ruta logica en numero de nivel.
- `run()`: ejecuta loop principal y cierra hilos.
- `isRunning()`: informa si la partida sigue activa.

Al cerrar:

1. apaga banderas de hilos;
2. despierta el semaforo;
3. desactiva entidades;
4. espera `playerThread` y `eventThread`;
5. destruye mutex/semaforo.

### `src/core/GameActions.cpp`

Contiene acciones directas.

- `logEvent()` agrega lineas al registro.
- `countActiveEnemies()` y `countActiveProjectiles()` alimentan HUD.
- `fireProjectile()` crea un proyectil y abre su hilo.
- `tryAbsorbEnemy()` copia habilidad del enemigo al frente.
- `processComputerInput()` ejecuta modo automatico.
- `processInput()` decide entre teclado normal o IA automatica.

`processInput()` se ejecuta desde el hilo de Kirby. Como puede cambiar estado,
ese hilo toma el mutex antes de llamarla.

### `src/core/GameInternals.h`

Comparte helpers internos del modulo `Game`.

Incluye:

- variables internas globales;
- funciones de creacion de hilos;
- helpers de spawn;
- limpieza de entidades;
- comida sobre suelo;
- jefe activo;
- reset por caida.

Existe para mantener `Game.h` limpio y evitar declarar helpers repetidos en cada
archivo `.cpp`.

## 8. Resumen para defensa oral

Una forma corta de explicarlo:

> El juego usa un loop principal para reglas globales y render, pero separa
> comportamientos activos con Pthreads. Kirby tiene un hilo para input o modo
> automatico, los enemigos y proyectiles tienen hilos propios, y los eventos del
> escenario usan un hilo que despierta mediante semaforo. Para que esos hilos no
> escriban el mismo estado al mismo tiempo, se usa un mutex global. El semaforo
> se usa especificamente para no tener al hilo de eventos revisando sin parar:
> se duerme con `sem_wait()` y el update lo despierta con `sem_post()` cuando
> toca generar items.
