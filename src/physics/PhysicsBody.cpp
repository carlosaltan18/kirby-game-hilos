#include "../../include/PhysicsBody.h"


PhysicsBody::PhysicsBody() {
    mass = 1.0f;
    gravity = 1.0f;
    friction = 0.1f;
    maxFallSpeed = 3;
}

/**
 *Aplica la aceleración de la gravedad a la velocidad vertical.
 * 
 * Suma la gravedad a la velocidad actual en Y. Si el personaje alcanza
 * la velocidad máxima de caída (terminal velocity), limita la velocidad
 * para evitar que caiga a velocidades incontrolables.
 * 
 */
void PhysicsBody::applyGravity(int &velocityY) {
    velocityY += gravity;
    
    // Limita la velocidad vertical al máximo permitido
    if(velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }
}

/**
 * Aplica una fuerza de fricción para desacelerar el movimiento horizontal.
 * 
 * Reduce la velocidad en el eje X gradualmente hasta llegar a 0, simulando
 * el rozamiento con el suelo.
*/
void PhysicsBody::applyFriction(int &velocityX) {
    if(velocityX > 0) {
        velocityX -= friction;
        // Previene que el objeto empiece a moverse hacia atrás por exceso de fricción
        if(velocityX < 0) velocityX = 0; 
        
    } else if(velocityX < 0) {
        velocityX += friction;
        // Previene que el objeto empiece a moverse hacia adelante
        if(velocityX > 0) velocityX = 0;
    }
}

/**
 * Mantiene al cuerpo físico dentro de los límites del mapa.
 * 
 * Corrige la posición (X, Y) si el objeto intenta salirse de las dimensiones 
 * establecidas, basándose en su propia anchura y altura.
 */
void PhysicsBody::enforceBoundaries(int &x, int &y, int width, int height, int mapWidth, int mapHeight) {
    // Límite Izquierdo
    if (x < 0) x = 0;
    // Límite Derecho (restando el ancho del personaje para que no se salga la mitad de su cuerpo)
    if (x + width >= mapWidth) x = mapWidth - width;
    
    // Límite Superior (techo)
    if (y < 0) y = 0;
    
    // Límite Inferior (suelo)
    // Si toca el fondo del mapa (vacío), debería morir, pero esto evita que rompa la consola o la matriz.
    if (y + height >= mapHeight) {
        y = mapHeight - height;
    }
}