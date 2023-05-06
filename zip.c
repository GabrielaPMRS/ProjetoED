#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_node list_node;
struct list_node
{
    int valor;
    list_node *next;
};

typedef struct linked_list linked_list;
struct linked_list
{
    list_node *head;
    list_node *tail;
    int size;
};

typedef struct byte_info byte_info;
struct byte_info
{
    unsigned char byte;
    int frequencia;
    linked_list *bits;
};

typedef struct Huff_node Huff_node;
struct Huff_node
{
    int frequencia;
    // unsigned char byte;
    void *byte;
    Huff_node *next;
    Huff_node *left;
    Huff_node *right;
};

typedef struct Huff Huff;
struct Huff
{
    Huff_node *head;
};

linked_list *create_linked_list()
{
    linked_list *list = (linked_list *)malloc(sizeof(linked_list));
    list->head = NULL;
    list->size = 0;
    return list;
}

void add_linked_list(linked_list *list, int valor)
{
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    new_node->valor = valor;
    new_node->next = list->head;
    list->size += 1;
    list->head = new_node;
}

void add_linked_list_tail(linked_list *list, int valor)
{
    list_node *new_node = (list_node *)malloc(sizeof(list_node));
    new_node->valor = valor;
    list_node *current = list->head;
    list->size += 1;
    if (list->head == NULL)
    {
        list->head = new_node;
        new_node->next = NULL;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
        new_node->next = NULL;
    }
}

list_node *remove_linked_node(linked_list *bits_list)
{
    list_node *removed_node = bits_list->head;
    bits_list->head = bits_list->head->next;
    free(removed_node);
}

void copy_list(linked_list *aux_list, linked_list *final_list)
{
    list_node *aux = aux_list->head;
    while (aux != NULL)
    {
        add_linked_list(final_list, aux->valor);
        aux = aux->next;
    }
}

void inicializar(byte_info sequencia_bytes[])
{
    int i;
    for (i = 0; i < 256; i++)
    {
        sequencia_bytes[i].byte = (unsigned char)i;
        sequencia_bytes[i].frequencia = 0;
        sequencia_bytes[i].bits = NULL;
    }
}

void analizar_frequencias(byte_info sequencia_bytes[], char filename[])
{
    unsigned char x;
    FILE *arquivo_origem = fopen(filename, "rb");

    while (fread(&x, sizeof(unsigned char), 1, arquivo_origem) > 0)
    {
        sequencia_bytes[x].frequencia++;
    }
    fclose(arquivo_origem);
}

int is_empty(Huff *huff)
{
    return huff->head == NULL;
}

Huff *create_huff_queue()
{
    Huff *new_queue = (Huff *)malloc(sizeof(Huff));
    new_queue->head = NULL;
    return new_queue;
}

void enqueue(Huff *huff, void *valor, int frequencia, Huff_node *left, Huff_node *right)
{
    Huff_node *new_node = (Huff_node *)malloc(sizeof(Huff_node));
    new_node->byte = valor;
    new_node->frequencia = frequencia;
    new_node->left = left;
    new_node->right = right;

    if (is_empty(huff) || frequencia <= huff->head->frequencia)
    {
        new_node->next = huff->head;
        huff->head = new_node;
    }
    else
    {
        Huff_node *current = huff->head;
        while (current->next != NULL && current->next->frequencia < frequencia)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

Huff_node *dequeue(Huff *huff)
{
    if (is_empty(huff))
    {
        printf("priority queue overflow\n");
    }
    else
    {
        Huff_node *node = huff->head;
        huff->head = huff->head->next;
        node->next = NULL;
        return node;
    }
}

void populate_huff_queue(Huff *huff, byte_info sequencia_bytes[])
{
    int i;
    for (i = 0; i < 256; i++)
    {
        if (sequencia_bytes[i].frequencia != 0)
        {
            enqueue(huff, (void *)&sequencia_bytes[i].byte, sequencia_bytes[i].frequencia, NULL, NULL);
        }
    }
}

void print_queue(Huff *huff)
{
    Huff_node *current = huff->head;
    while (current != NULL)
    {
        printf("byte: %c | freq: %d \n", *((unsigned char *)current->byte), current->frequencia);
        current = current->next;
    }
    printf("\n");
}

Huff_node *create_huff_tree(Huff *huff)
{
    while (huff->head->next != NULL)
    {
        int soma_freq = huff->head->frequencia + huff->head->next->frequencia;

        Huff_node *left = dequeue(huff);
        Huff_node *right = dequeue(huff);

        // NULL representa o * dos nodes internos da arvore
        enqueue(huff, NULL, soma_freq, left, right);
    }
    return huff->head;
}

list_node *saveTreeInList(Huff_node *tree, linked_list *PreOrderTree)
{
    if (tree != NULL)
    {
        if (tree->byte == NULL)
        {
            add_linked_list_tail(PreOrderTree, 42);
        }
        else if (*((unsigned char *)tree->byte) == '*')
        {
            add_linked_list_tail(PreOrderTree, 92);
            add_linked_list_tail(PreOrderTree, 42);
        }
        else if (*((unsigned char *)tree->byte) == '\\')
        {
            add_linked_list_tail(PreOrderTree, 92);
            add_linked_list_tail(PreOrderTree, 92);
        }
        else
        {
            add_linked_list_tail(PreOrderTree, *((unsigned char *)tree->byte));
        }

        saveTreeInList(tree->left, PreOrderTree);
        saveTreeInList(tree->right, PreOrderTree);
    }
    return PreOrderTree->head;
}

void print_pre_order(Huff_node *tree)
{
    if (tree != NULL)
    {
        if (tree->byte == NULL)
        {
            printf("%c ", '*');
        }
        else if (*((unsigned char *)tree->byte) == '*')
        {
            printf("\\*");
        }
        else if (*((unsigned char *)tree->byte) == '\\')
        {
            printf("\\\\");
        }
        else
        {
            printf("%c ", *((unsigned char *)tree->byte));
        }
        print_pre_order(tree->left);
        print_pre_order(tree->right);
    }
}

void get_bytes_bits_list(Huff_node *tree, linked_list *aux_bits_list, byte_info sequencia_bytes[])
{
    if (tree->left == NULL && tree->right == NULL)
    {
        linked_list *final_bits_list = create_linked_list();
        copy_list(aux_bits_list, final_bits_list);
        sequencia_bytes[*((unsigned char *)tree->byte)].bits = final_bits_list;
    }
    else
    {
        add_linked_list(aux_bits_list, 0);
        get_bytes_bits_list(tree->left, aux_bits_list, sequencia_bytes);
        remove_linked_node(aux_bits_list);

        add_linked_list(aux_bits_list, 1);
        get_bytes_bits_list(tree->right, aux_bits_list, sequencia_bytes);
        remove_linked_node(aux_bits_list);
    }
}

unsigned char set_bit(unsigned char c, int i)
{
    unsigned char mask = 1 << i;
    return mask | c;
}

int zip_tmp_file(char filename[], byte_info sequencia_bytes[])
{
    FILE *original = fopen(filename, "rb");
    FILE *temp = fopen("temporario", "wb");

    unsigned char byte;
    unsigned char compressed_byte = 0;
    int i = 7;

    while (fread(&byte, sizeof(unsigned char), 1, original) > 0)
    {
        list_node *bits = sequencia_bytes[byte].bits->head;

        while (bits != NULL)
        {
            if (i < 0)
            {
                fwrite(&compressed_byte, sizeof(unsigned char), 1, temp);
                compressed_byte = 0;
                i = 7;
            }

            if (bits->valor == 1)
            {
                compressed_byte = set_bit(compressed_byte, i);
            }

            i--;
            bits = bits->next;
        }
    }
    fwrite(&compressed_byte, sizeof(unsigned char), 1, temp);

    int trash_size = i + 1;

    fclose(original);
    fclose(temp);

    return trash_size;
}

void header(char filename[], byte_info sequencia_bytes[], linked_list *PreOrderTree)
{
    int trash_size = zip_tmp_file(filename, sequencia_bytes);
    int tree_size = PreOrderTree->size;
    unsigned char header[2] = {0};

    int i = 5;

    while (trash_size != 0)
    {
        if (trash_size % 2 == 1)
        {
            header[0] = set_bit(header[0], i);
        }

        i++;
        trash_size = trash_size / 2;
    }

    i = 0;

    while (tree_size != 0)
    {
        if (i > 7)
        {
            break;
        }
        if (tree_size % 2 == 1)
        {
            header[1] = set_bit(header[1], i);
        }

        i++;
        tree_size = tree_size / 2;
    }

    if (tree_size > 0)
    {
        i = 0;
        while (tree_size != 0)
        {
            if (i > 4)
            {
                break;
            }
            if (tree_size % 2 == 1)
            {
                header[0] = set_bit(header[0], i);
            }
            i++;
            tree_size = tree_size / 2;
        }
    }

    FILE *hd = fopen("output.huff", "wb");
    fwrite(header, sizeof(unsigned char), 2, hd);

    list_node *aux = PreOrderTree->head;
    while (aux != NULL)
    {
        unsigned char byte = (unsigned char)aux->valor;
        fwrite(&byte, sizeof(unsigned char), 1, hd);
        aux = aux->next;
    }

    FILE *temp = fopen("temporario", "rb");

    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, temp) > 0)
    {
        fwrite(&byte, sizeof(unsigned char), 1, hd);
    }
    fclose(hd);
}

int main()
{
    char file_name[100];
    printf("Entre com o nome do arquivo a ser comprimido: ");
    scanf("%s", file_name);

    byte_info sequencia_bytes[256];
    inicializar(sequencia_bytes);
    analizar_frequencias(sequencia_bytes, file_name);

    Huff *huff_queue = create_huff_queue();
    populate_huff_queue(huff_queue, sequencia_bytes);

    Huff_node *huff_tree = create_huff_tree(huff_queue);

    linked_list *aux_bit_list = create_linked_list();
    get_bytes_bits_list(huff_tree, aux_bit_list, sequencia_bytes);

    int trash_size = zip_tmp_file(file_name, sequencia_bytes);

    linked_list *PreOrderTree = create_linked_list();
    saveTreeInList(huff_tree, PreOrderTree);

    header(file_name, sequencia_bytes, PreOrderTree);

    printf("arquivo comprimido em 'output.huff'\n");
}