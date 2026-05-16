# 🎮 Kirby Console Engine

Juego de plataformas 2D inspirado en **Kirby's Adventure**, desarrollado en **C++**, utilizando:

- **ncurses** para renderizado en consola
- **POSIX Threads (pthread)** para concurrencia
- Arquitectura modular orientada a objetos
- ASCII-Art para representación gráfica

El proyecto implementa:

✅ Menú interactivo  
✅ Mapas dinámicos  
✅ Scroll horizontal  
✅ Enemigos concurrentes  
✅ Física básica  
✅ Sistema de colisiones  
✅ HUD en tiempo real  
✅ Sistema de puntajes  
✅ Cambio de niveles  

---

# 📸 Características

## 🎮 Gameplay
- Movimiento lateral
- Salto y gravedad
- Proyectiles
- Enemigos dinámicos
- Sistema de vidas
- Cambio de mapas

---

## 🧠 Concurrencia
- Threads independientes
- Mutex para sincronización
- Actualización simultánea de entidades

---

## 🖥️ Renderizado
- Renderizado ASCII-Art
- Cámara con scrolling
- HUD dinámico
- Interfaz ncurses

---

# 📁 Estructura del Proyecto

```text
kirby/
│
├── src/
│
│   ├── main.cpp
│
│   ├── core/
│   │   ├── Game.cpp
│   │   ├── ThreadManager.cpp
│   │   └── Timer.cpp
│
│   ├── graphics/
│   │   ├── Renderer.cpp
│   │   ├── Camera.cpp
│   │   ├── HUD.cpp
│   │   └── AnimationSystem.cpp
│
│   ├── input/
│   │   └── InputManager.cpp
│
│   ├── physics/
│   │   ├── CollisionSystem.cpp
│   │   ├── GravitySystem.cpp
│   │   └── PhysicsBody.cpp
│
│   ├── world/
│   │   ├── LevelManager.cpp
│   │   ├── TileMap.cpp
│   │   ├── Platform.cpp
│   │   └── SpawnSystem.cpp
│
│   ├── entities/
│   │   ├── Entity.cpp
│   │   ├── Character.cpp
│   │   ├── Player.cpp
│   │   ├── Enemy.cpp
│   │   ├── Boss.cpp
│   │   ├── Projectile.cpp
│   │   ├── Item.cpp
│   │   └── Food.cpp
│
│   ├── ai/
│   │   ├── EnemyAI.cpp
│   │   └── PathSystem.cpp
│
│   ├── ui/
│   │   ├── MainMenu.cpp
│   │   ├── PauseMenu.cpp
│   │   ├── InstructionScreen.cpp
│   │   ├── ScoreScreen.cpp
│   │   └── GameOverScreen.cpp
│
│   └── save/
│       └── ScoreManager.cpp
│
├── include/
│   └── (*.h)
│
├── assets/
│   ├── levels/
│   │   ├── level1.txt
│   │   ├── level2.txt
│   │   └── boss.txt
│   │
│   ├── ascii/
│   │   ├── kirby.txt
│   │   ├── enemy.txt
│   │   └── boss.txt
│   │
│   └── config/
│       └── game.conf
│
├── Makefile
└── README.md
```

---

# 🧠 Arquitectura General

## 🔹 Core
Control principal del juego.

| Clase | Función |
|---|---|
| Game | Game loop principal |
| ThreadManager | Manejo de pthreads |
| Timer | Control de tiempo |

---

## 🔹 Graphics
Renderizado en consola.

| Clase | Función |
|---|---|
| Renderer | Dibujar entidades/mapa |
| Camera | Scroll horizontal |
| HUD | Información del jugador |
| AnimationSystem | Animaciones ASCII |

---

## 🔹 Physics
Sistema físico.

| Clase | Función |
|---|---|
| GravitySystem | Gravedad |
| CollisionSystem | Colisiones |
| PhysicsBody | Velocidad/fricción |

---

## 🔹 Entities
Objetos del juego.

| Clase | Función |
|---|---|
| Player | Kirby |
| Enemy | Enemigos |
| Boss | Jefe final |
| Projectile | Disparos |
| Food | Curación |

---

## 🔹 World
Gestión del entorno.

| Clase | Función |
|---|---|
| TileMap | Lectura del mapa |
| Platform | Plataformas físicas |
| SpawnSystem | Spawn dinámico |

---

# ⚙️ Requisitos

## Linux / Ubuntu / WSL

Instalar:

```bash
sudo apt update
sudo apt install g++
sudo apt install make
sudo apt install libncurses5-dev libncursesw5-dev
```

---

# 🚀 Clonación del Proyecto

## 1. Clonar repositorio

```bash
git clone https://github.com/USUARIO/kirby.git
```

---

## 2. Entrar al proyecto

```bash
cd kirby
```

---

# 🔨 Compilación

El proyecto utiliza un `Makefile`.

## Compilar

```bash
make
```

---

## Ejecutar

```bash
make run
```

o

```bash
./kirby
```

---

## Limpiar binarios

```bash
make clean
```

---

# 🧩 Makefile

```make
CXX=g++

CXXFLAGS=-Wall -Iinclude

LIBS=-lncurses -lpthread

SRC=$(wildcard src/**/*.cpp)

OUT=kirby

all:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(LIBS)

run:
	./$(OUT)

clean:
	rm -f $(OUT)
```

---

# 🎮 Controles

| Tecla | Acción |
|---|---|
| A | Mover izquierda |
| D | Mover derecha |
| W | Saltar |
| K | Disparar |
| ESC | Pausa |

---

# 🖥️ Ejemplo de Juego

```text
############################################################
#                                                          #
#          K                                               #
#                                                          #
#                         E                                #
#                                                          #
#                    #######                               #
#                                                          #
#                                   EXIT                   #
############################################################

Vida: 3    Score: 1200
```

---

# 🧠 Tecnologías Utilizadas

| Tecnología | Uso |
|---|---|
| C++ | Lenguaje principal |
| ncurses | Renderizado consola |
| pthread | Concurrencia |
| Makefile | Build system |
| Git | Control de versiones |

---

# 🧵 Concurrencia

El proyecto implementa concurrencia mediante POSIX Threads:

- Player Thread
- Enemy Threads
- Physics Thread
- Render Thread

Utilizando:

```cpp
pthread_create()
pthread_mutex_lock()
pthread_mutex_unlock()
```

---

# 🛠️ Desarrollo Colaborativo

## Crear ramas

```bash
git checkout -b physics
git checkout -b graphics
git checkout -b ai
```

---

## Guardar cambios

```bash
git add .
git commit -m "Descripción"
```

---

## Subir cambios

```bash
git push origin physics
```

---

# ⚠️ Problemas Comunes

## ❌ `ncurses.h not found`

Instalar:

```bash
sudo apt install libncurses5-dev
```

---

## ❌ `undefined reference pthread`

Agregar:

```bash
-lpthread
```

---

## ❌ Pantalla parpadea

Usar:

```cpp
refresh();
```

en lugar de `cout`.

---

# 🏆 Objetivos del Proyecto

- Implementar concurrencia real
- Simular un platformer 2D
- Utilizar programación modular
- Aplicar sincronización con mutex
- Desarrollar interfaz gráfica ASCII

---

# 👨‍💻 Integrantes

| Nombre | Rol |
|---|---|
| Carlos Altán | Render/Física |
| Cristian Orellana | UI/Game Design |
| Sergio Tepaz | Concurrencia/IA |

---

# 📚 Referencias

- NCURSES Programming HOWTO :contentReference[oaicite:0]{index=0}
- pthreads tutorial :contentReference[oaicite:1]{index=1}
- Makefile best practices :contentReference[oaicite:2]{index=2}
- ncurses multithreading example :contentReference[oaicite:3]{index=3}