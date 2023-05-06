#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct node
{
  int data;
  struct node *left;
  struct node *right;
  int color;
} ArvLLRB;

void preOrder(ArvLLRB *root)
{
  if (root != NULL)
  {
    printf("%d %s\n", root->data, root->color == BLACK ? "preto" : "vermelho");
    preOrder(root->left);
    preOrder(root->right);
  }
}

int color(ArvLLRB *no)
{
  if (no == NULL)
  {
    return BLACK;
  }

  else
  {
    return no->color;
  }
}

void changeColor(ArvLLRB *no)
{
  no->color = !no->color;

  if (no->left != NULL)
  {
    no->left->color = !no->left->color;
  }

  if (no->right != NULL)
  {
    no->right->color = !no->right->color;
  }
}

ArvLLRB *rotateLeft(ArvLLRB *no)
{
  ArvLLRB *newNode = no->right;
  no->right = newNode->left;
  newNode->left = no;
  newNode->color = no->color;
  no->color = RED;
  return newNode;
}

ArvLLRB *rotateRight(ArvLLRB *no)
{
  ArvLLRB *newNode = no->left;
  no->left = newNode->right;
  newNode->right = no;
  newNode->color = no->color;
  no->color = RED;
  return newNode;
}

ArvLLRB *insertNode(ArvLLRB *no, int value, int *check)
{
  if (no == NULL)
  {
    ArvLLRB *newNode;
    newNode = (ArvLLRB *)malloc(sizeof(ArvLLRB));

    if (newNode == NULL)
    {
      *check = 0;
      return NULL;
    }

    newNode->data = value;
    newNode->color = RED;
    newNode->right = NULL;
    newNode->left = NULL;
    *check = 1;
    return newNode;
  }

  if (value == no->data)
  {
    *check = 0; // O valor está duplicado na árvore
  }

  else
  {
    if (value < no->data)
    {
      no->left = insertNode(no->left, value, check);
    }
    else
    {
      no->right = insertNode(no->right, value, check);
    }
  }

  if (color(no->right) == RED && color(no->left) == BLACK)
  {
    no = rotateLeft(no);
  }

  // Verifica se o filho da esquerda e o neto da esquerda são vermelhos, se sim rotaciona pra direita
  if (no->left != NULL && color(no->left) == RED && color(no->left->left) == RED)
  {
    no = rotateRight(no);
  }

  // Verifica se os dois filhos são vermelhos, se sim muda as cores
  if (color(no->left) == RED && color(no->right) == RED)
  {
    changeColor(no);
  }

  return no;
}

int insert_ArvLLRB(ArvLLRB **root, int value)
{
  int check;
  *root = insertNode(*root, value, &check);

  if (*root != NULL)
  {
    (*root)->color = BLACK;
  }

  return check;
}

ArvLLRB *moveToLeftRED(ArvLLRB *no)
{
  changeColor(no);
  if (color(no->right->left) == RED)
  {
    no->right = rotateRight(no->right);
    no = rotateLeft(no);
    changeColor(no);
  }

  return no;
}

ArvLLRB *moveToRightRED(ArvLLRB *no)
{
  changeColor(no);

  if (color(no->left->left) == RED)
  {
    no = rotateRight(no);
    changeColor(no);
  }

  return no;
}

ArvLLRB *balance(ArvLLRB *no)
{
  // Verifica se o filho à direita é RED, se sim rotaciona pra esquerda
  if (color(no->right) == RED)
  {
    no = rotateLeft(no);
  }

  // Verifica se o filho da esquerda e o neto da esquerda são vermelhos, se sim rotaciona pra direita
  if (no->left != NULL && color(no->left) == RED && color(no->left->left) == RED)
  {
    no = rotateRight(no);
  }

  // Verifica se os dois filhos são vermelhos, se sim muda as cores
  if (color(no->left) == RED && color(no->right) == RED)
  {
    changeColor(no);
  }

  return no;
}

ArvLLRB *removeMin(ArvLLRB *no)
{
  if (no->left == NULL)
  {
    free(no);
    return NULL;
  }

  if (color(no->left) == BLACK && color(no->left->left) == BLACK)
  {
    no = moveToLeftRED(no);
  }

  no->left = removeMin(no->left);
  return balance(no);
}

ArvLLRB *searchMin(ArvLLRB *current)
{
  ArvLLRB *newNode1 = current;
  ArvLLRB *newNode2 = current->left;

  while (newNode2 != NULL)
  {
    newNode1 = newNode2;
    newNode2 = newNode2->left;
  }

  return newNode1;
}

int search_ArvLLRB(ArvLLRB *root, int value)
{
  if (root == NULL)
  {
    return 0;
  }

  ArvLLRB *current = root;
  while (current != NULL)
  {
    if (value == current->data)
    {
      return 1;
    }

    if (value > current->data)
    {
      current = current->right;
    }

    else
    {
      current = current->left;
    }
  }

  return 0;
}

ArvLLRB *removeNode(ArvLLRB *no, int value)
{
  if (value < no->data)
  {
    if (color(no->left) == BLACK && color(no->left->left) == BLACK)
    {
      no = moveToLeftRED(no);
    }

    no->left = removeNode(no->left, value);
  }

  else
  {
    if (color(no->left) == RED)
    {
      no = rotateRight(no);
    }

    if (value == no->data && (no->right == NULL))
    {
      free(no);
      return NULL;
    }

    if (color(no->right) == BLACK && color(no->right->left) == BLACK)
    {
      no = moveToRightRED(no);
    }

    if (value == no->data)
    {
      ArvLLRB *min = searchMin(no->right);
      no->data = min->data;
      no->right = removeMin(no->right);
    }

    else
    {
      no->right = removeNode(no->right, value);
    }
  }

  return balance(no);
}

int remove_ArvLLRB(ArvLLRB *root, int value)
{
  if (search_ArvLLRB(root, value))
  {
    ArvLLRB *no = root;
    root = removeNode(no, value);
    if (root != NULL)
    {
      root->color = BLACK;
    }

    return 1;
  }

  else
  {
    return 0;
  }
}

int main()
{
  ArvLLRB *root = NULL;

  int num, i;
  for (i = 0; i < 4; i++)
  {
    scanf("%d", &num);
    insert_ArvLLRB(&root, num);
  }

  printf("\nArvore em Pre-Ordem:\n");
  preOrder(root);
  printf("\n");

  remove_ArvLLRB(root, 15);
  preOrder(root);

  return 0;
}