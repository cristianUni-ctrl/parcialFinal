#include <stdio.h>
#include <stdlib.h>

// Estructura del Nodo del Árbol AVL
struct nodo {
    int llave;
    struct nodo *izq;
    struct nodo *der;
    int altura;
};

// Función para obtener la altura de un nodo
int altura(struct nodo *N) {
    return (N == NULL) ? 0 : N->altura;
}

// Función para obtener el máximo de dos enteros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Crear un nuevo nodo
struct nodo* nuevoNodo(int llave) {
    struct nodo* node = (struct nodo*) malloc(sizeof(struct nodo));
    node->llave = llave;
    node->izq = node->der = NULL;
    node->altura = 1;
    return node;
}

// Rotación a la derecha
struct nodo* rotacionDerecha(struct nodo *y) {
    struct nodo *x = y->izq;
    struct nodo *T2 = x->der;

    x->der = y;
    y->izq = T2;

    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;

    return x;
}

// Rotación a la izquierda
struct nodo* rotacionIzquierda(struct nodo *x) {
    struct nodo *y = x->der;
    struct nodo *T2 = y->izq;

    y->izq = x;
    x->der = T2;

    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;

    return y;
}

// Obtener el factor de balance
int obtenerBalance(struct nodo *N) {
    return (N == NULL) ? 0 : altura(N->izq) - altura(N->der);
}

// Insertar nodo en el árbol AVL
struct nodo* insert(struct nodo* node, int llave) {
    if (node == NULL) return nuevoNodo(llave);

    if (llave < node->llave)
        node->izq = insert(node->izq, llave);
    else if (llave > node->llave)
        node->der = insert(node->der, llave);
    else
        return node; // Duplicados no permitidos

    node->altura = 1 + max(altura(node->izq), altura(node->der));
    int balance = obtenerBalance(node);

    // Casos de desbalance
    if (balance > 1 && llave < node->izq->llave)
        return rotacionDerecha(node);
    if (balance < -1 && llave > node->der->llave)
        return rotacionIzquierda(node);
    if (balance > 1 && llave > node->izq->llave) {
        node->izq = rotacionIzquierda(node->izq);
        return rotacionDerecha(node);
    }
    if (balance < -1 && llave < node->der->llave) {
        node->der = rotacionDerecha(node->der);
        return rotacionIzquierda(node);
    }

    return node;
}

// Buscar nodo con menor valor (sucesor)
struct nodo* minValueNode(struct nodo* node) {
    struct nodo* actual = node;
    while (actual->izq != NULL)
        actual = actual->izq;
    return actual;
}

// Eliminar un nodo
struct nodo* deleteNode(struct nodo* raiz, int llave) {
    if (raiz == NULL) return raiz;

    if (llave < raiz->llave)
        raiz->izq = deleteNode(raiz->izq, llave);
    else if (llave > raiz->llave)
        raiz->der = deleteNode(raiz->der, llave);
    else {
        if ((raiz->izq == NULL) || (raiz->der == NULL)) {
            struct nodo *temp = (raiz->izq) ? raiz->izq : raiz->der;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            struct nodo* temp = minValueNode(raiz->der);
            raiz->llave = temp->llave;
            raiz->der = deleteNode(raiz->der, temp->llave);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));
    int balance = obtenerBalance(raiz);

    if (balance > 1 && obtenerBalance(raiz->izq) >= 0)
        return rotacionDerecha(raiz);
    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }
    if (balance < -1 && obtenerBalance(raiz->der) <= 0)
        return rotacionIzquierda(raiz);
    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// Imprimir recorrido preorden
void preOrder(struct nodo *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->llave);
        preOrder(raiz->izq);
        preOrder(raiz->der);
    }
}

// Menú interactivo
int main() {
    struct nodo *raiz = NULL;
    int opcion, valor;

    do {
        printf("\n--- MENU AVL ---\n");
        printf("1. Insertar nodo\n");
        printf("2. Eliminar nodo\n");
        printf("3. Mostrar árbol en preorden\n");
        printf("4. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                printf("Ingrese valor a insertar: ");
                scanf("%d", &valor);
                raiz = insert(raiz, valor);
                break;
            case 2:
                printf("Ingrese valor a eliminar: ");
                scanf("%d", &valor);
                raiz = deleteNode(raiz, valor);
                break;
            case 3:
                printf("Árbol en preorden: ");
                preOrder(raiz);
                printf("\n");
                break;
            case 4:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción inválida\n");
        }
    } while (opcion != 4);

    return 0;
}
