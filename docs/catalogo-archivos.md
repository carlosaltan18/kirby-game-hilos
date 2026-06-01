# Catalogo de archivos del proyecto

Este catalogo explica para que sirve cada archivo visible del proyecto. La idea
no es repetir el codigo linea por linea, sino dejar una guia rapida para saber
donde buscar cada responsabilidad.

Nota: los mapas, ASCII art y JSON se documentan aqui y no con comentarios dentro
del archivo, porque agregar texto ahi puede cambiar el nivel, romper el sprite o
invalidar el JSON.

## Raiz

| Archivo | Proposito |
|---|---|
| `.gitignore` | Define archivos que Git debe ignorar, como binarios y salidas locales. |
| `Makefile` | Compila el proyecto con `g++`, enlazando `ncurses` y `pthread`. |
| `README.md` | Presenta el proyecto, estructura, compilacion, controles y enlaces a docs. |
| `kirby` | Binario compilado del juego. No se edita a mano; sale de `make`. |

## Assets

| Archivo | Proposito |
|---|---|
| `assets/ascii/boss.txt` | Arte ASCII base del jefe final. |
| `assets/ascii/enemy.txt` | Referencia ASCII para enemigos. |
| `assets/ascii/kirby.txt` | Referencia ASCII para Kirby. |
| `assets/config/game.conf` | Archivo de configuracion del proyecto, reservado para ajustes externos. |
| `assets/levels/boss.txt` | Mapa de la pelea final contra el jefe. |
| `assets/levels/level1.txt` | Primer nivel jugable, con plataformas, huecos y meta. |
| `assets/levels/level2.txt` | Segundo nivel jugable, antes de pasar al jefe. |
| `assets/scores.json` | Persistencia de puntajes; se actualiza al finalizar partidas. |

## Documentacion

| Archivo | Proposito |
|---|---|
| `docs/catalogo-archivos.md` | Este catalogo general de archivos del proyecto. |
| `docs/core-game.md` | Explica el modulo `Game`, su separacion interna y el flujo de partida. |
| `docs/informe-final-borrador.md` | Borrador base del informe UVG para convertir a PDF. |
| `docs/requisitos-kirby.md` | Lista de cumplimiento contra los requisitos del enunciado. |

## Headers publicos

| Archivo | Proposito |
|---|---|
| `include/AnimationSystem.h` | Declara el sistema de animaciones ASCII simples. |
| `include/Boss.h` | Declara el jefe final y su sprite multilinea. |
| `include/Camera.h` | Declara la camara horizontal que sigue a Kirby. |
| `include/Character.h` | Declara la base para entidades con vida y estado de suelo. |
| `include/CollisionSystem.h` | Declara utilidades de colision AABB. |
| `include/Enemy.h` | Declara enemigos absorbibles y la habilidad que entregan. |
| `include/EnemyAI.h` | Declara la IA de enemigos normales. |
| `include/Entity.h` | Declara la base comun de posicion, tamano y actividad. |
| `include/FireEnemy.h` | Declara el enemigo que otorga la habilidad Fuego. |
| `include/Food.h` | Declara items consumibles que curan o dan puntos. |
| `include/Game.h` | Declara el coordinador principal de una partida. |
| `include/GameOverScreen.h` | Declara pantallas de derrota/victoria y guardado de puntaje. |
| `include/GravitySystem.h` | Declara la aplicacion de gravedad y ajuste contra suelo. |
| `include/HUD.h` | Declara la capa de informacion en pantalla. |
| `include/InputManager.h` | Declara lectura de teclas con `ncurses`. |
| `include/InstructionScreen.h` | Declara la pantalla inicial de instrucciones. |
| `include/Item.h` | Declara la base para objetos recolectables. |
| `include/KirbyAbility.h` | Enumera habilidades copiadas por Kirby. |
| `include/LevelManager.h` | Declara carga de niveles hacia `TileMap`. |
| `include/MainMenu.h` | Declara el menu principal y seleccion de modo. |
| `include/PathSystem.h` | Declara movimientos de patrulla/persecucion para enemigos. |
| `include/PauseMenu.h` | Declara el menu de pausa. |
| `include/PhysicsBody.h` | Declara un cuerpo fisico sencillo con velocidades. |
| `include/Platform.h` | Declara plataformas basicas del escenario. |
| `include/Player.h` | Declara a Kirby, su estado, vida, score y habilidad activa. |
| `include/Projectile.h` | Declara ataques especiales disparados por Kirby. |
| `include/Renderer.h` | Declara el renderer ASCII del mapa y entidades. |
| `include/ScoreManager.h` | Declara lectura/escritura de puntajes en JSON simple. |
| `include/ScoreScreen.h` | Declara la pantalla de puntajes guardados. |
| `include/SpawnSystem.h` | Declara creacion de enemigos e items. |
| `include/ThreadManager.h` | Declara mutex y semaforo usados por los hilos. |
| `include/TileMap.h` | Declara el mapa ASCII y consultas de solidez. |
| `include/Timer.h` | Declara un temporizador basico para medir intervalos. |

## Source: AI

| Archivo | Proposito |
|---|---|
| `src/ai/EnemyAI.cpp` | Implementa decisiones simples de enemigos segun distancia a Kirby. |
| `src/ai/PathSystem.cpp` | Implementa patrullaje, persecucion y calculo de distancia. |

## Source: core

| Archivo | Proposito |
|---|---|
| `src/core/Game.cpp` | Inicializa la partida, carga niveles, corre el loop y cierra hilos. |
| `src/core/GameActions.cpp` | Maneja input, modo computadora, absorcion, disparos y registro de eventos. |
| `src/core/GameInternals.h` | Comparte helpers internos entre archivos `Game*.cpp`. |
| `src/core/GameRender.cpp` | Conecta `Renderer` y `HUD` bajo el mutex del juego. |
| `src/core/GameThreads.cpp` | Implementa funciones `void*` para hilos de jugador, enemigos, proyectiles y eventos. |
| `src/core/GameUpdate.cpp` | Aplica reglas globales: colisiones, vida, items, jefe y cambio de nivel. |
| `src/core/GameWorld.cpp` | Implementa helpers de spawn, comida sobre suelo, limpieza y pantalla del jefe. |
| `src/core/ThreadManager.cpp` | Inicializa y destruye mutex/semaforo compartidos. |
| `src/core/Timer.cpp` | Implementa inicio, parada y lectura del temporizador. |

## Source: entities

| Archivo | Proposito |
|---|---|
| `src/entities/Boss.cpp` | Implementa movimiento, vida y sprite del jefe final. |
| `src/entities/Character.cpp` | Implementa dano, vida y estado de suelo de actores. |
| `src/entities/Enemy.cpp` | Implementa enemigo normal que otorga Estrella al absorberse. |
| `src/entities/Entity.cpp` | Implementa getters/setters basicos de posicion y actividad. |
| `src/entities/FireEnemy.cpp` | Implementa enemigo de fuego y la habilidad que copia Kirby. |
| `src/entities/Food.cpp` | Implementa tipos de comida/item, curacion, score y sprite. |
| `src/entities/Item.cpp` | Implementa base de items recolectables. |
| `src/entities/Player.cpp` | Implementa movimiento, salto, flotacion, habilidades y sprite de Kirby. |
| `src/entities/Projectile.cpp` | Implementa movimiento, dano y sprite de proyectiles por habilidad. |

## Source: graphics

| Archivo | Proposito |
|---|---|
| `src/graphics/AnimationSystem.cpp` | Alterna sprites ASCII de enemigos e items. |
| `src/graphics/Camera.cpp` | Calcula desplazamiento horizontal para seguir al jugador. |
| `src/graphics/HUD.cpp` | Dibuja vida, score, nivel, habilidad, jefe y registro. |
| `src/graphics/Renderer.cpp` | Dibuja fondo, mapa, Kirby, enemigos, proyectiles e items. |

## Source: input

| Archivo | Proposito |
|---|---|
| `src/input/InputManager.cpp` | Lee una tecla desde `ncurses` sin bloquear el juego. |

## Source: physics

| Archivo | Proposito |
|---|---|
| `src/physics/CollisionSystem.cpp` | Implementa colisiones rectangulares AABB. |
| `src/physics/GravitySystem.cpp` | Aplica gravedad y evita que actores atraviesen plataformas. |
| `src/physics/PhysicsBody.cpp` | Implementa velocidades basicas para cuerpos fisicos simples. |

## Source: save

| Archivo | Proposito |
|---|---|
| `src/save/ScoreManager.cpp` | Carga, ordena y guarda puntajes en `assets/scores.json`. |

## Source: ui

| Archivo | Proposito |
|---|---|
| `src/ui/GameOverScreen.cpp` | Muestra derrota/victoria, pide nombre y guarda puntaje. |
| `src/ui/InstructionScreen.cpp` | Muestra objetivo, controles y simbologia del juego. |
| `src/ui/MainMenu.cpp` | Muestra menu principal y lanza modo jugador/computadora. |
| `src/ui/PauseMenu.cpp` | Muestra menu de pausa y devuelve continuar/salir. |
| `src/ui/ScoreScreen.cpp` | Muestra tabla de puntajes persistidos. |

## Source: world

| Archivo | Proposito |
|---|---|
| `src/world/LevelManager.cpp` | Decide que archivo de nivel cargar segun el numero de nivel. |
| `src/world/Platform.cpp` | Implementa una plataforma rectangular basica. |
| `src/world/SpawnSystem.cpp` | Crea enemigos normales/de fuego e items aleatorios. |
| `src/world/TileMap.cpp` | Carga mapas ASCII, guarda grilla y responde si una celda es solida. |

## Entrada principal

| Archivo | Proposito |
|---|---|
| `src/main.cpp` | Inicializa `ncurses`, colores, instrucciones y menu principal. |
