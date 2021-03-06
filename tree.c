/**
 * File: htable.c
 * @author: Vivian Breda, Josh King, Abinaya Saravanapavan.
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mylib.h"
#include "tree.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type;

static tree root_node = NULL;

/**
 * Struct: tree_node
 * Purpose: declares the variables for the tree node.
 */
struct tree_node {
  char *key;
  tree_colour colour;
  tree left;
  tree right;
  int frequency;
};

/**
 * Function: root_fix
 * Purpose: changes the root colour to black. 
 *
 * @param t is the tree.
 * @return t the fixed tree. 
 */
tree root_fix(tree t){

  t->colour = BLACK;
  return t;
}

/**
 * Function: right_rotate
 * Purpose: rotates the tree branches from the left to the right
 * around the root of given subtree. 
 *
 * @param t is the tree.
 * @return t the rotated tree. 
 */
static tree right_rotate(tree t){
  tree temp;
  int root_flag = 0;

    
  if (t == root_node){
    root_flag = 1;
  }
  temp = t;

  t = t->left;
  temp->left = t->right;

  t->right = temp;

  if (root_flag == 1){
    root_node = t;
  }

  return t;
}

/**
 * Function: left_rotate
 * Purpose: rotates the tree branches from the right to the left
 * around the root of given subtree.
 *
 * @param t is the tree.
 * @return t the rotated tree. 
 */
static tree left_rotate(tree t){
  tree  temp;
  int root_flag = 0;

  if (t == root_node){
    root_flag = 1;
  }
  temp = t;

  t = t->right;
  temp->right = t->left;

  t->left = temp;

  if (root_flag == 1){
    root_node = t;
  }
    
  return t;
}

/**
 * Function: tree_fix
 * Purpose: updates the colours of the branches of the tree,
 * and makes sure the tree is balanced. 
 *
 * @param t is the tree.
 * @return t the fixed tree. 
 */
static tree tree_fix(tree t){
    
  if (IS_RED(t->left) && IS_RED(t->left->left)){
    if (IS_RED(t->right)){
      t->colour = RED;
      t->left->colour = BLACK;
      t->right->colour = BLACK;
    } else if (IS_BLACK(t->right)){
      t = right_rotate(t);
      t->colour = BLACK;
      t->right->colour = RED;
    }
  } else if (IS_RED(t->left) && IS_RED(t->left->right)){
    if (IS_RED(t->right)){
      t->colour = RED;
      t->left->colour = BLACK;
      t->right->colour = BLACK;
    } else if (IS_BLACK(t->right)){
      t->left = left_rotate(t->left);
      t = right_rotate(t);
      t->colour = BLACK;
      t->right->colour = RED;
    }
  } else if (IS_RED(t->right) && IS_RED(t->right->left)){
    if (IS_RED(t->left)){
      t->colour = RED;
      t->left->colour = BLACK;
      t->right->colour = BLACK;
    } else if (IS_BLACK(t->left)){
      t->right = right_rotate(t->right);
      t = left_rotate(t);
      t->colour = BLACK;
      t->left->colour = RED;
    }
  } else if (IS_RED(t->right) && IS_RED(t->right->right)){
    if (IS_RED(t->left)){
      t->colour = RED;
      t->left->colour = BLACK;
      t->right->colour = BLACK;
    } else if (IS_BLACK(t->left)){ 
      t = left_rotate(t);
      t->colour = BLACK;
      t->left->colour = RED;
    }
  }
    
  root_node = root_fix(root_node);
  return t;
}

/**
 * Function: tree_free
 * Purpose: frees all the memory allocated to the tree. 
 *
 * @param t is the tree.
 * @return t the freed tree. 
 */
tree tree_free(tree t){

  if (t == NULL){
    return t;
  } else {
    tree_free(t->left);
    tree_free(t->right);
    free(t->key);
    free(t);
    return t;
  }
}

/**
 * Function: tree_inorder
 * Purpose: traverses through the tree in sorted order and performs the
 * given function on the data in each node.
 *
 * @param t is the tree.
 * @param f another function passed in with parameters freq and str.
 * @param freq frequencies of the string.
 * @param str the string to search. 
 */
void tree_inorder(tree t, void f(int freq, char *str)){

  if (t == NULL){
    return;
  }
  tree_inorder(t->left, f);
  f(t->frequency, t->key);
  tree_inorder(t->right, f);
}

/**
 * Function: tree_insert
 * Purpose: inserts a string into the tree. 
 *
 * @param t is the tree.
 * @param str is a pointer to the string to be inserted.
 * @return t the new tree with inserted pointer to string. 
 */
tree tree_insert(tree t, char *str){

  if (t == NULL || t->key == NULL){

    if (t == NULL){
      t = tree_new(tree_type);
    }
    t->key = emalloc(strlen(str) * sizeof str[0] + 1);
    if (tree_type == RBT){
      t->colour = RED;
    }
    strcpy(t->key, str);
    t->frequency = 1;
  } else if (strcmp(t->key, str) == 0){
    t->frequency++;
  } else if (strcmp(str, t->key) > 0){
    t->right = tree_insert(t->right, str);
  } else {
    t->left = tree_insert(t->left, str);
  }

  if (root_node == NULL){
    root_node = t;
  }
   
  if (tree_type == RBT){
    t = tree_fix(t);
  }
    
  return t;
}

/**
 * Function: tree_new
 * Purpose: creates a new tree. 
 *
 * @param type determines whether the tree is an ordinary bst or balanced rbt.
 * @return t the created tree. 
 */
tree tree_new(tree_t type){

  tree t;
    
  tree_type = type;
  
  t = emalloc(sizeof *t);
  t->key = NULL;
  t->colour = RED;
  t->left = NULL;
  t->right = NULL;
  t->frequency = 0;

  return t;

}

/**
 * Function: tree_preorder
 * Purpose: traverses through the tree in preorder and performs the
 * given function on the data in each node.
 *
 * @param t is the tree.
 * @param f another function passed in with parameters freq and str.
 * @param freq frequencies of the string.
 * @param str the string to search. 
 */
void tree_preorder(tree t, void f(int freq, char *str)){

  if (t == NULL){
    return;
  }
  f(t->frequency, t->key);
  tree_preorder(t->left, f);
  tree_preorder(t->right, f);
}

/**
 * Function: tree_search
 * Purpose: searches through the tree for a given string. 
 *
 * @param t is the tree.
 * @param str is a pointer to the string to be inserted.
 * @return 1 if the string exists in the tree, 0 if not.  
 */
int tree_search(tree t, char *str){

  if (t == NULL){
    return 0;
  } else if (strcmp(str, t->key) == 0){
    return 1;
  } else if (strcmp(str, t->key) > 0){
    return tree_search(t->right, str);
  } else {
    return tree_search(t->left, str);
  }
}


/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
  if(t->key != NULL) {
    fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
	    t->key, t->key, t->frequency,
	    (RBT == tree_type && RED == t->colour) ? "red":"black");
  }
  if(t->left != NULL) {
    tree_output_dot_aux(t->left, out);
    fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
  }
  if(t->right != NULL) {
    tree_output_dot_aux(t->right, out);
    fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
  }
}

/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
  fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
  tree_output_dot_aux(t, out);
  fprintf(out, "}\n");
}


