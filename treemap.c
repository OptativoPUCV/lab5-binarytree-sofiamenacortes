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

    TreeNode * aux = tree->root;
    while(aux != NULL){
        int cmp = tree->lower_than(key, aux->pair->key);
        if(cmp == 0){
            tree->current = aux;
            return aux->pair;
        }else if (cmp < 0) {
            aux = aux->left;  
        }else {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
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
