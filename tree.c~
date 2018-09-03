#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mylib.h"
#include "rbt.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))


struct rbt_node {
    char *key;
    rbt_colour colour;
    rbt left;
    rbt right;
};

rbt right_rotate(rbt b){
    rbt root, temp;

    root = b;
    temp = root;

    root = b->left;
    temp->left = root->right;

    root->right = temp;

    return root;
}

rbt left_rotate(rbt b){
    rbt root, temp;

    root = b;
    temp = root;

    root = b->right;
    temp->right = root->left;

    root->left = temp;

    return root;
}

rbt rbt_fix(rbt b){

    if(IS_RED(b->left) && IS_RED(b->left->left)){
        if (IS_RED(b->right)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }

    else if(IS_RED(b->left) && IS_RED(b->left->right)){
        if (IS_RED(b->right)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b->left = left_rotate(b->left);
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }

    else if(IS_RED(b->right) && IS_RED(b->right->left)){
        if (IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b->right = right_rotate(b->right);
            b = left_rotate(b);
            b->colour = BLACK;
            b->right->colour= RED;
        }
    }

    else if (IS_RED(b->right) && IS_RED(b->right->right)){
        if (IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }else{
            b = left_rotate(b);
            b->colour = BLACK;
            b->left->colour = RED;
        }
    }

    return b;
}

rbt rbt_delete(rbt b, char *str){

    rbt temp;
    
    if (b == NULL){
        return b;
    }
    else if (strcmp(b->key, str) == 0){
        if (b->left == NULL && b->right == NULL){
            free(b->key);
            free(b);
            return NULL;
        }
        else if(b->left != NULL && b->right == NULL){
            free(b->key);
            free(b);
            return b->left;
        }
        else if(b->right != NULL && b->left == NULL){
            free(b->key);
            free(b);
            return b->right;
        }
        else{
            temp = b->right;
            while (temp->left != NULL){
                temp = temp->left;
            }
            strcpy(b->key, temp->key);
            b->right = rbt_delete(b->right, temp->key);
            return b;
        }

    }
    else if(b->left != NULL && strcmp(b->key, str) > 0){
        b->left = rbt_delete(b->left, str);
    }

    else if(b->right != NULL && strcmp(b->key, str) < 0){
        b->right = rbt_delete(b->right, str);
    }

    return b;


}

rbt rbt_free(rbt b){

    if (b == NULL){
        return b;
    }else{
        rbt_free(b->left);
        rbt_free(b->right);
        free(b->key);
        free(b);
        return b;
    }
}

void rbt_inorder(rbt b, void f(char *str, rbt_colour c)){

    if (b == NULL){
        return;
    }
    rbt_inorder(b->left, f);
    f(b->key, b->colour);
    rbt_inorder(b->right, f);
}

rbt rbt_insert(rbt b, char *str){

    if (b == NULL){
        
        b = emalloc(sizeof *b);
        b->key = emalloc(strlen(str) * sizeof str[0] + 1);
        b->colour = RED;
        strcpy(b->key, str);
    }
    else if (strcmp(b->key, str) == 0){
    }
    else if (strcmp(str, b->key) > 0){
        b->right = rbt_insert(b->right, str);
    }
    else{
        b->left = rbt_insert(b->left, str);
    }

    b = rbt_fix(b);
    return b;
}

rbt rbt_new(){
    return NULL;
    
}

void rbt_preorder(rbt b, void f(char *str, rbt_colour c)){

    if (b == NULL){
        return;
    }
    f(b->key, b->colour);
    rbt_preorder(b->left, f);
    rbt_preorder(b->right, f);
}

int rbt_search(rbt b, char *str){

    if (b == NULL){
        return 0;
    }
    else if (strcmp(str, b->key) == 0){
        return 1;
    }
    else if (strcmp(str, b->key) > 0){
        return rbt_search(b->right, str);
    }
    else{
        return rbt_search(b->left, str);
    }
}
