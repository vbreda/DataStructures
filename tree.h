#ifndef TREE_H_
#define TREE_H_

typedef struct tree_node *tree;

typedef enum { RED, BLACK } tree_colour;

typedef enum tree_e { BST, RBT } tree_t;

extern tree tree_free(tree t);
extern void tree_inorder(tree t, void f(int freq, char *str));
extern tree tree_insert(tree t, char *str);
extern tree tree_new(tree_t type);
extern void tree_preorder(tree t, void f(int freq, char *str));
extern int tree_search(tree t, char *str);
extern void tree_output_dot(tree t, FILE *out);

#endif
