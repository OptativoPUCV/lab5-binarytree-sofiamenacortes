#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
        TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
        if(new){
            new->root = NULL;
            new->current = NULL;
            new->lower_than = lower_than;
        }
        return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) {
        // Error al crear el nodo, no se puede insertar
        return;
    }

    // Si el árbol está vacío, el nuevo nodo se convierte en la raíz
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode; // Actualizar el puntero current al nuevo nodo
        return;
    }

    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    // Encontrar la posición adecuada para insertar el nuevo nodo
    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else if (tree->lower_than(current->pair->key, key)) {
            current = current->right;
        } else {
            // La clave ya existe en el árbol, no se permite la inserción de claves duplicadas
            free(newNode->pair); // Liberar la memoria del par clave-valor
            free(newNode); // Liberar la memoria del nodo
            return;
        }
    }

    // Enlazar el nuevo nodo al árbol
    newNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key)) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // Actualizar el puntero current al nuevo nodo
    tree->current = newNode;
   
}

TreeNode * minimum(TreeNode * x){
    if(x->left==NULL) return x;
    return minimum(x->left);
    

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* current = tree->root;
    while (current != NULL){
        if (tree->lower_than(key, current->pair->key)){
            current = current->left;
        }else{
            if (tree->lower_than(current->pair->key, key)){
                current = current->right;
            } else{
                tree->current = current;
                return current->pair;
            }
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) { //Encuentra el primer par cuya llave sea mayor que la clave dada
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode * aux = tree->root;

    while(aux->left != NULL){
        aux = aux->left;
    }

    tree->current = aux;
    return aux->pair;    
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode * aux = tree->current;
    if(aux->right != NULL){
        aux = aux->right;
        while(aux->left != NULL){
            aux = aux->left;
        }
    }else {
        TreeNode* parent = aux->parent;
        while(parent != NULL && aux == parent->right){
            aux = parent;
            parent = parent->parent;
        }
        aux = parent;
    }
    tree->current = aux;

    return (aux != NULL) ? aux->pair : NULL;
}
