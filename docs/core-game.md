# Modulo core: Game

Este archivo explica la parte central del juego: la clase `Game`, que vive en
`src/core/Game.cpp` y se declara en `include/Game.h`.

La forma simple de verlo es esta: `Game` es quien mantiene la partida viva. No
dibuja todo por si solo, no decide toda la fisica por si solo y no carga mapas
directamente desde cero; mas bien coordina a los demas sistemas para que todo
ocurra en orden.

## Donde empieza una partida

La partida empieza cuando el menu principal crea un objeto `Game`, llama a
`init()` y despues a `run()`.

`Game::init()` prepara el estado inicial:

- activa configuracion basica de `ncurses` para leer teclas sin bloquear;
- crea al jugador en una posicion inicial;
- carga el primer mapa;
- genera comida/items sobre el suelo;
- crea enemigos iniciales y les abre su hilo.

En otras palabras, `init()` deja el tablero listo antes de que empiece el loop
principal.

## El loop principal

`Game::run()` es el ciclo de vida de la partida. Mientras `running` sea `true`,
ejecuta siempre el mismo orden:

1. `processInput()`
2. `update()`
3. `render()`
4. una pausa corta con `usleep()`

Ese orden importa. Primero se lee lo que hizo el jugador, luego se actualiza el
mundo, despues se dibuja el resultado.

## Entrada del jugador

`Game::processInput()` se encarga de traducir teclas a acciones:

- `A` mueve a Kirby a la izquierda y lo voltea hacia ese lado.
- `D` mueve a Kirby a la derecha y lo voltea hacia ese lado.
- `W` salta o activa el flotado si ya esta en el aire.
- `J` hace que Kirby absorba hacia donde esta mirando.
- `H` detiene la absorcion.
- `K` dispara una estrella hacia donde Kirby esta mirando.
- `Q` sale de la partida actual.

El disparo no es solo visual: aqui se decide si el proyectil nace a la derecha
o a la izquierda del jugador.

## Actualizacion del mundo

`Game::update()` es la parte mas cargada del archivo. Aqui pasa casi todo lo que
cambia durante la partida.

Primero actualiza al jugador y aplica gravedad. Si Kirby cae por debajo del mapa
por un hueco sin suelo, se le resta vida y se le regresa a una posicion segura.

Despues se recorren los enemigos activos:

- los enemigos normales usan `EnemyAI` para patrullar o perseguir;
- el jefe usa su propia logica de movimiento;
- si un enemigo toca al jugador, el jugador recibe dano.

Tambien aqui se crean enemigos nuevos si el nivel necesita mantener presion.
Durante la pelea contra el jefe, este puede llamar enemigos de apoyo cuando hay
pocos activos.

## Proyectiles, comida e items

Los proyectiles se actualizan dentro de `update()`. Si chocan contra un enemigo,
le hacen dano, se desactivan y suman puntaje al jugador.

La comida y los items tambien se revisan aqui. Cuando Kirby toca uno:

- el item se desactiva;
- puede curar vida, dependiendo del tipo;
- suma puntos.

Los items no aparecen simplemente en una coordenada fija. Se generan con una X
aleatoria y luego se acomodan sobre una casilla que tenga suelo debajo. Esa parte
la hace `spawnFoodsOnGround()` apoyandose en `placeFoodsOnGround()`.

## Cambio de niveles

El cambio de nivel ocurre cuando Kirby llega cerca del final del mapa.

Si pasa el nivel 1:

- se limpia lo activo del nivel anterior;
- se carga `level2.txt`;
- Kirby vuelve a una posicion inicial;
- aparecen nuevos items.

Si pasa el nivel 2:

- se muestra una pantalla de aviso del jefe;
- se carga `boss.txt`;
- Kirby vuelve a una posicion segura;
- se crea el jefe.

Cuando el jefe muere, se muestra la pantalla de victoria y se guarda el puntaje.

## Renderizado

`Game::render()` no dibuja cada cosa manualmente. Su trabajo es llamar al
`Renderer` y al `HUD` en el orden correcto.

El `Renderer` dibuja el mapa, el fondo, Kirby, enemigos, proyectiles y comida.
El `HUD` dibuja vida, score, nivel y la vida del jefe si esta activo.

Tambien se usa un mutex del `ThreadManager` para evitar que el render se cruce
con otras partes que puedan tocar el estado del juego.

## Hilos

Los enemigos normales tienen un hilo asociado. El hilo corre mientras el enemigo
este activo y se cierra cuando muere.

El jefe no usa hilo propio. Se actualiza dentro del loop principal porque es una
entidad especial y asi se evita mezclar su movimiento con la IA normal de los
enemigos.

## Funciones auxiliares importantes

En `Game.cpp` hay varias funciones fuera de la clase que hacen tareas concretas:

- `createEnemyThread()` abre y separa el hilo de un enemigo.
- `deactivateLevelEntities()` apaga enemigos, proyectiles e items al cambiar de mapa.
- `showBossScreen()` muestra la pantalla previa al jefe.
- `findGroundY()` busca una posicion sobre suelo.
- `placeFoodsOnGround()` acomoda items para que no queden flotando.
- `spawnFoodsOnGround()` genera items y los pone sobre suelo real.
- `getActiveBoss()` busca si hay un jefe activo.
- `resetPlayerAfterFall()` quita vida y reposiciona a Kirby si cae por un hueco.

## Cosas a tener en cuenta

`Game.cpp` es el centro de coordinacion. Si algo tiene que ver con el orden de la
partida, probablemente pasa aqui.

Pero si el cambio es de dibujo puro, conviene ir a `Renderer`. Si es fisica,
conviene revisar `GravitySystem` o `CollisionSystem`. Si es comportamiento de
enemigos normales, esta en `EnemyAI` y `PathSystem`. Si es menu o pantallas, va
en `ui`.

Esa separacion ayuda a que `Game` no termine haciendo absolutamente todo.
