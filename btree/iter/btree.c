/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) { *tree = NULL; }

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree != NULL) {
    if (tree->key == key) {
      *value = tree->value;
      return true;
    }

    if (tree->key > key) {
      tree = tree->left;

    } else {
      tree = tree->right;
    }
  }

  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *newNode = malloc(sizeof(bst_node_t));
  bst_node_t *curr = *tree;
  bst_node_t *parent = NULL;

  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;

  if (*tree == NULL) {
    *tree = newNode;
    return;
  }

  while (curr != NULL) {
    parent = curr;

    if (curr->key == key) {
      curr->value = value;
      free(newNode);
      return;
    }

    if (curr->key > key) {
      curr = curr->left;

    } else {
      curr = curr->right;
    }
  }

  if (parent->key > key) {
    parent->left = newNode;

  } else {
    parent->right = newNode;
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *curr = *tree;
  bst_node_t *parent = NULL;

  while (curr->right != NULL) {
    parent = curr;
    curr = curr->right;
  }

  target->key = curr->key;
  target->value = curr->value;

  if (parent == NULL) {
    *tree = curr->left;
  } else {
    parent->right = curr->left;
  }

  free(curr);
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *curr = *tree;
  bst_node_t *parent = NULL;

  while (curr != NULL) {
    if (curr->key == key) {
      break;
    }

    parent = curr;

    if (curr->key > key) {
      curr = curr->left;
    } else {
      curr = curr->right;
    }
  }

  if (curr == NULL) {
    return;
  }

  if (curr->left == NULL && curr->right == NULL) {
    if (parent == NULL) {
      *tree = NULL;
    } else if (parent->left == curr) {
      parent->left = NULL;
    } else {
      parent->right = NULL;
    }

    free(curr);
  } else if (curr->left == NULL) {
    if (parent == NULL) {
      *tree = curr->right;
    } else if (parent->left == curr) {
      parent->left = curr->right;
    } else {
      parent->right = curr->right;
    }

    free(curr);
  } else if (curr->right == NULL) {
    if (parent == NULL) {
      *tree = curr->left;
    } else if (parent->left == curr) {
      parent->left = curr->left;
    } else {
      parent->right = curr->left;
    }

    free(curr);
  } else {
    bst_replace_by_rightmost(curr, &curr->left);
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *curr = *tree;
  bst_node_t *parent = NULL;

  while (curr != NULL) {
    if (curr->left == NULL && curr->right == NULL) {
      if (parent == NULL) {
        *tree = NULL;
      } else if (parent->left == curr) {
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }

      free(curr);
      curr = parent;
    } else if (curr->left == NULL) {
      if (parent == NULL) {
        *tree = curr->right;
      } else if (parent->left == curr) {
        parent->left = curr->right;
      } else {
        parent->right = curr->right;
      }

      free(curr);
      curr = parent;
    } else if (curr->right == NULL) {
      if (parent == NULL) {
        *tree = curr->left;
      } else if (parent->left == curr) {
        parent->left = curr->left;
      } else {
        parent->right = curr->left;
      }

      free(curr);
      curr = parent;
    } else {
      bst_node_t *rightmost = curr->left;
      bst_node_t *rightmost_parent = curr;

      while (rightmost->right != NULL) {
        rightmost_parent = rightmost;
        rightmost = rightmost->right;
      }

      curr->key = rightmost->key;
      curr->value = rightmost->value;

      if (rightmost_parent->left == rightmost) {
        rightmost_parent->left = rightmost->left;
      } else {
        rightmost_parent->right = rightmost->left;
      }

      free(rightmost);
      curr = rightmost_parent;
    }

    if (curr != NULL) {
      parent = curr->right;
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *curr = tree;

  while (curr != NULL) {
    bst_print_node(curr);
    stack_bst_push(to_visit, curr);
    curr = curr->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  bst_leftmost_preorder(tree, &to_visit);

  while (!stack_bst_empty(&to_visit)) {
    bst_node_t *curr = stack_bst_top(&to_visit);
    stack_bst_pop(&to_visit);

    bst_leftmost_preorder(curr->right, &to_visit);
  }

  stack_bst_pop(&to_visit);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *curr = tree;

  while (curr != NULL) {
    stack_bst_push(to_visit, curr);
    curr = curr->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  bst_leftmost_inorder(tree, &to_visit);

  while (!stack_bst_empty(&to_visit)) {
    bst_node_t *curr = stack_bst_top(&to_visit);
    stack_bst_pop(&to_visit);

    bst_print_node(curr);
    bst_leftmost_inorder(curr->right, &to_visit);
  }

  stack_bst_pop(&to_visit);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  bst_node_t *curr = tree;

  while (curr != NULL) {
    stack_bst_push(to_visit, curr);
    stack_bool_push(first_visit, true);
    curr = curr->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t to_visit;
  stack_bst_init(&to_visit);

  stack_bool_t first_visit;
  stack_bool_init(&first_visit);

  bst_leftmost_postorder(tree, &to_visit, &first_visit);

  while (!stack_bst_empty(&to_visit)) {
    bst_node_t *curr = stack_bst_top(&to_visit);
    bool first = stack_bool_top(&first_visit);

    if (first) {
      stack_bool_pop(&first_visit);
      stack_bool_push(&first_visit, false);
      bst_leftmost_postorder(curr->right, &to_visit, &first_visit);
    } else {
      bst_print_node(curr);
      stack_bst_pop(&to_visit);
      stack_bool_pop(&first_visit);
    }
  }

  stack_bst_pop(&to_visit);
  stack_bool_pop(&first_visit);
}
