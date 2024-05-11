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


void insertTreeMap(TreeMap * tree, void* key, void * value) { //inserta un nuevo nodo en el arbol
    //si inserto un nuevo par clave-valor se necesita un nuevo nodo
    TreeNode* newNode = createTreeNode(key, value); 
    if(newNode == NULL){
        return;
    }

    //si el arbol esta vacio, el nuevo nodo sera la raiz    
    if(tree->root == NULL){
        tree->root = newNode;
        tree->current = newNode;
        return;
    }
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;
    
    //encontrar la posicion donde insertar el nodo
    while(current != NULL){
        parent = current;    
        if(tree->lower_than(key, current->pair->key)){
            current = current->left;
            
        }else if(tree->lower_than(current->pair->key, key)){
            current = current->right;
            
        }else{
            free(newNode->pair);
            free(newNode);
            return;
        }        
    }

    //conectar el nuevo nodo al arbol   
    newNode->parent = parent;
    if(tree->lower_than(key, parent->pair->key)){
        parent->left = newNode;
        
    }else{
        parent->right = newNode;
        
    }
    tree->current = newNode;
   
}


TreeNode * minimum(TreeNode * x){
    if(x->left==NULL) return x;
    return minimum(x->left);
    
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) { //elimina un nodo especifico (no tiene hijos, uno solo y sin hijos)
    //Nodo sin hijos
    if(node->left == NULL && node->right == NULL){
        if(node->parent == NULL){
            tree->root = NULL;
        }else if(node->parent->left == node){
            node->parent->left = NULL;
        }else{
            node->parent->right = NULL;
        }
        free(node->pair);
        free(node);

    }else if(node->left != NULL && node->right == NULL){
        //Nodo tiene un hijo izquierdo
        TreeNode* aux = node->left;
        if(node->parent == NULL){
            tree->root = aux;
        }else if(node->parent->left == node){
            node->parent->left = aux;
        }else{
            node->parent->right = aux;
        }
        aux->parent = node->parent;
        free(node->pair);
        free(node);
        
    }else if(node->left == NULL && node->right != NULL){
        //Nodo tiene un hijo derecho
        TreeNode* aux = node->right;
        if(node->parent == NULL){
            tree->root = aux;
        }else if(node->parent->left == node){
            node->parent->left = aux;
        }else{
            node->parent->right = aux;
        }
        aux->parent = node->parent;
        free(node->pair);
        free(node);
        
    }else{
        //Nodo con dos hijos
        TreeNode* siguenteNum = minimum(node->right);
        node->pair->key = siguenteNum->pair->key;
        node->pair->value = siguenteNum->pair->value;
        removeNode(tree, siguenteNum);
    }    
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
    if (tree == NULL || tree->root == NULL){
        return NULL;
    } 
    
    TreeNode* current = tree->root;
    TreeNode* ub = NULL;

    while (current != NULL){
        if (!tree->lower_than(current->pair->key, key)){
            ub = current;
            current = current->left;
            
        }else{
            current = current->right;
        }
        
    }
    return (ub != NULL) ? ub->pair : NULL;
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
